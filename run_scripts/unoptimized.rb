#------------------------------------------------------------------------------
# Unoptimized Runs
#------------------------------------------------------------------------------
require_relative './runscripts.rb'
include RunScripts

module RunScripts
# Single Benchmark
def single_benchmark
    $specint.each do |p0|
        opts = {
            maxinsts: 10**4,
            fastforward: 0,
            runmode: :local
        }
        sav_script( "detailed", "none", p0, opts )
    end
end

#Cache Sweeping
def cache_sweeping
    $cpus.product( $schemes,  $cacheSizes ).each do |cpu, scheme, cacheSize|
        $specint.product( $specint ).each do|p0, p1|
            sav_script( cpu, scheme, p0, p1: p1 )
        end
    end
end

#Turn Length Sweeping
def turn_length_sweeping
    $cpus.product( $turnlengths ).each do |cpu, turnlength|
        $specint.product( $specint ).each do |p0,p1|
            sav_script( cpu, "tp", p0, tl0: turnlength, p1: p1 )
        end
        $specint.each do |p0|
            sav_script( cpu, "tp", p0, tl0: turnlength )
        end
    end
end

#Thread Scaling
def thread_scaling
    $cpus.product( $schemes ).each do |cpu, scheme|
        $specint.product( $specint ).each do |p0,other|
            sav_script( cpu, scheme, p0, p1: other, p2: other )
            sav_script( cpu, scheme, p0, p1: other, p2: other, p3:other )
        end
    end
end
end
