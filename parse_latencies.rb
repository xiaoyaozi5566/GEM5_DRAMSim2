#!/usr/bin/ruby
require "csv"

# Makes a histogram based on the differences between the
# delay at the address in the results compared to the delay
# at the same address in the trace file.
class DelayHist < Hash
    def initialize(results,trace)
        super(0)
        results.keys.each do |key|
            delay = results[key] - trace[key]
            self[delay]+=1
        end
    end

    # Saves a hash to a CSV file
    def saveResults(outfile)
        #do self.sort.to_a to save with the delays ordered
        hist = self.sort.to_a
        CSV.open(outfile,"wb") do |csv|
            hist.each {|datum| csv << datum}
        end
    end

    # Print the arithmetic mean of delays from histogram
    def getMean
        numerator=0
        denominator=0
        self.keys.each do |key|
            numerator+=key*self[key]
            denominator+=self[key]
        end
        numerator/denominator.to_f
    end
end

class AddrDelayHist < Hash
    def initialize(results,trace)
        super(nil)
        results.keys.each do |key|
            delay = results[key] - trace[key]
            self[key]=delay
        end
    end

    # Saves a hash to a CSV file
    def saveResults(outfile)
        #sort the histogram by delay (value).
        hist = self.sort_by{|key,value| value}.to_a
        CSV.open(outfile,"wb") do |csv|
            hist.each {|datum| csv << datum}
        end
    end   
end

# Makes a hash of the form Address=>Time from the file using
# the specified regex
class TimeHash < Hash
    def initialize(filename,regex)
        File.open(filename,'r').each_line do |rf_line|
            m =rf_line.match regex
            address = m[1]
            time = m[2].to_i
            if (m.length <4 || m[3].to_i==0)
                self[address]=time
            end
        end
    end
end

def makeHists (filename)
    latency_regex = /Address:\s?(\w*)\s*?Latency:\s?(\d*)\s?Thread:\s?(\d*)/

    hists = [Hash.new(0), Hash.new(0)]
    File.open(filename,'r').each_line do |line|
        m=line.match latency_regex
        unless m == nil
            next if m.length <4
            address = m[1].to_i
            latency = m[2].to_i
            thread  = m[3].to_i
            hists[thread][latency]+=1
        end
    end

    hists
end

def saveResults(hist,outfile)
    #calculate arithmetic mean
    numerator=0
    denominator=0
    hist.keys.each do |key|
        numerator+=key*hist[key]
        denominator+=hist[key]
    end
    avg = numerator/denominator.to_f

    #save hist as csv
    arr= hist.sort.to_a
    CSV.open(outfile,"wb") do |csv|
        arr.each {|datum| csv << datum}
    end

    #add a line with the average
    outFile =File.open(outfile,"a")
    outFile.puts("avg,#{avg}")
    outFile.close()

end

# Print the arithmetic mean of delays from histogram

if __FILE__ == $0
    inputFile = ARGV[0]
    t0out     = ARGV[1]
    t1out     = ARGV[2]

    t0Hist, t1Hist = makeHists(inputFile)
    saveResults(t0Hist, t0out)
    saveResults(t1Hist,t1out)

end
