#!/usr/bin/ruby
require 'fileutils'

$cpus = %w[timing detailed]
$l3configs = %w[ shared private ]
$specint = %w[
    astar
    bzip2
    gcc
    gobmk
    h264ref
    hmmer
    libquantum
    mcf
    sjeng
    xalan
]
$result_directory = 'results/'

def get_filename( arg={} )
    arg= {
        scheme: "none",
        cpu: "detailed",
        p0: "astar",
        tl0: 6,
        tl1: 6,
        tl2: 6,
        tl3: 6,
        cacheSize: 4,
        l3config: "shared"
    }

    filename  = "#{scheme}_#{cpu}_#{p0}tl#{tl0}"
    filename += "_#{p1}tl#{tl1}" unless p1.nil?
    filename += "_#{p2}tl#{tl2}" unless p2.nil?
    filename += "_#{p3}tl#{tl3}" unless p3.nil?
    filename += "_c#{cacheSize}MB"
    filename += "_l3#{l3config}"

end

def verify_cache_traces
    $schemes.product( $cpus, $l3configs ).map{|scheme,cpu,l3config|
        $specint.combination(2).each do |p0,p1|

        end
    }
end
