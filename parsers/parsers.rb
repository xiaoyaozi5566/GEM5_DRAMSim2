#!/usr/bin/ruby

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

def findTime(filename)
    return [nil, false] unless File.exists? filename
    time = nil
    foundtime = false
    File.open(filename,'r'){|f|
        switchregex = /Switched CPUS/
        timingregex = /Exiting @ tick (\d*)\w* because a\w*/
        foundcpuswitch = false
        f.each_line do |line|
            if !foundcpuswitch && line.match(switchregex)!=nil
                foundcpuswitch = true
            end
            unless line.match(timingregex)== nil || !foundcpuswitch
                time =(line.match timingregex)[1]
                time = time.to_f
                foundtime=true
                break
            end
        end
    }
    [time,foundtime]
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
end

def stdo_file( p={} )
    "results/stdout_#{filename p}.out"
end

def bench_swap_file( p={} )
    filename( p.merge{ p0:p[:p1], p1:[p:p0] } )
end

end
