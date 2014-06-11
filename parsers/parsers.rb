#!/usr/bin/ruby

require 'csv'
require 'fileutils'

module Parsers

$cpus = %w[timing detailed]
$specint = [
    'astar',
    'bzip2',
    'gcc',
    'gobmk',
    'h264ref',
    'hmmer',
    'libquantum',
    'mcf',
    #    'omnetpp',
    #    'perlbench',
    'sjeng',
    'xalan',
]
$schemes = %w[ none tp ]

def findTime(filename, opts = {} )
    return [nil, false] unless File.exists? filename
    time = nil
    foundtime = false 
    File.open(filename,'r'){|f|
        switchregex = /Switched CPUS/
        timingregex = /Exiting @ tick (\d*)\w* because a\w*/
        foundcpuswitch = (true && opts[:no_ff]) || false 
        f.each_line do |line|
            if !foundcpuswitch && line.match(switchregex)!=nil
                foundcpuswitch = true
            end
            if line.match(timingregex)!= nil && foundcpuswitch
                time =(line.match timingregex)[1]
                time = time.to_f
                foundtime=true
                break
            end
        end
    }
    [time,foundtime]
end

def get_datum( filename, regex )
    return [nil, false] unless File.exists? filename
    File.open(filename, 'r'){|f|
        f.each_line do |line|
            return [line.match(regex)[1],true] if line =~ regex
        end
    }
    [nil, false]
end

def overhead( t1, t2 , p={} )
    unless t1.nil? || t2.nil?
        ( p[:X] && t1/t2 ) || (t1-t2)/t2 * 100
    end
end

def percent_diff(t1,t2)
     unless t1.nil? || t2.nil?
         high = ( t1>=t2 && t1 ) || ( true && t2 )
         low  = ( t1>=t2 && t2 ) || ( true && t1 )
         (high-low)/((high+low)/2) * 100
     end
end

def avg_arr arr
    (arr.length != 0 && arr.inject(:+)/arr.length) || -1
end

def filename( p={} )
    p = { 
        tl0: 6,
        tl1: 6,
        tl2: 6,
        tl3: 6,
        cacheSize: 4,
    }.merge p
    filename  = "#{p[:scheme]}_#{p[:cpu]}_#{p[:p0]}tl#{p[:tl0]}"
    filename += "_#{p[:p1]}tl#{p[:tl1]}" unless p[:p1].nil?
    filename += "_#{p[:p2]}tl#{p[:tl2]}" unless p[:p2].nil?
    filename += "_#{p[:p3]}tl#{p[:tl3]}" unless p[:p3].nil?
    filename += "_c#{p[:cacheSize]}MB"
    filename = "#{p[:nametag]}_" + filename unless p[:nametag].nil?
    filename
end

def stdo_file( p={} )
    p={dir: "results"}.merge p
    "#{p[:dir]}/stdout_#{filename p}.out"
end

def m5out_file( p={} )
    p = {dir: "m5out"}.merge p
    "#{p[:dir]}/#{filename p}_stats.txt"
end

def bench_swap_file( p={} )
    filename( p.merge{ p0:p[:p1], p1:[p:p0] } )
end

def hash_to_csv( hash, filename, p={} )
    CSV.open( filename, 'w' ) do |csv_object|
        hash.to_a.each{|row| csv_object << row}
    end
end

end
