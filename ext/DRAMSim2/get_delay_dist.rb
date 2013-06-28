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
            self[address]=time
        end
    end
end

if __FILE__ == $0
    fileone=ARGV[0]
    filetwo=ARGV[1]
    outDelayHist=ARGV[2]
    outAddrDelayHist=ARGV[3]

    result_regex = /Address:\s?(\w*)\s*?Return time:\s?(\d*)/
    trace_regex  = /0x(\w*)\s?\w*\s?(\d*)/

    resultsHash=TimeHash.new(fileone,result_regex)
    traceHash=TimeHash.new(filetwo,trace_regex)

    delayHist = DelayHist.new(resultsHash,traceHash)
    delayHist.saveResults(outDelayHist)
    puts "the mean is #{delayHist.getMean}"

    addrDH = AddrDelayHist.new(resultsHash,traceHash)
    addrDH.saveResults(outAddrDelayHist)

    #save_results(delayHist.sort,outfile)

    #puts "the mean is #{print_mean(delayHist)}"

end
