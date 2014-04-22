#------------------------------------------------------------------------------
# Optimized Runs
#------------------------------------------------------------------------------
require_relative './runscripts.rb'
include RunScripts

module RunScripts
# This is a Hash of the form scheme -> model -> cache size -> benchmark -> mpki 
# saved to a file. It was generated using parsers/parse_mpki_spec
def mpkidata
    data = Marshal.load( File.open $gem5home.path+"/mpki.rubyobj" )
    data["none"]["detailed"][0]
end


def tester 
    [
        lambda { |r| r <= 1/4.0               },
        lambda { |r| r <= 1/3.0 && r >= 1/4.0 },
        lambda { |r| r <= 1/2.0 && r >= 1/3.0 },
        lambda { |r| r <= 2/3.0 && r >= 1/2.0 },
        lambda { |r| r <= 3/2.0 && r >= 2/3.0 },
        lambda { |r| r <=   2.0 && r >= 3/2.0 },
        lambda { |r| r <=   3.0 && r >=   2.0 },
        lambda { |r| r <=   4.0 && r >=   3.0 },
        lambda { |r|               r >=   4.0 },
    ]
end

def configs  
    {
        0 => [ 64,256],  
        1 => [ 64,192], 
        2 => [ 64,128], 
        3 => [ 64,96 ],  
        4 => [ 64,64 ],  
        5 => [ 96,64 ],  
        6 => [128,64 ],  
        7 => [192,64 ],  
        8 => [256,64 ],  
    }
end

# This function takes two benchmarks and determines the best configuration of 
# periods based on the ratio of the Memory Accesses per Thousand Instructions 
# in p0 to that value in p1.
#
# First, the mpki of each benchmark is found from the loaded hash map. Then, 
# the ratio is computed and stored as a floating point value. The $tester.map 
# computes a new array of booleans where each element is the result of applying 
# the mpkir to the input of the original (lambda function) element in $tester. 
# The index function returns the index of the first true element which is used 
# as the index to configs. Configs is set up to return the appropriate 
# configuration as a pair [p0_turnlength, p1turnlength].
def get_turns( p0,p1 )
        mpki0 = mpkidata[p0]["mpki"]
        mpki1 = mpkidata[p1]["mpki"]
        mpkir = mpki0 / mpki1
        configs[
            tester.map{|test|
                test.call(mpkir)
            }.index(true)
        ]
end

#Thread Scaling
def optimized_thread_scaling
    $cpus.product( $schemes ).each do |cpu, scheme|
        $specint.product( $specint ).each do |p0,other|
            tlp0, tlp1 = get_turns( p0, other )
            sav_script( cpu, scheme, p0,
               {diffperiod: true, tl0: tlp0, p1: other, tl1: tlp1 }
            )
            sav_script( cpu, scheme, p0,
               {diffperiod: true, tl0: tlp0, 
                p1: other, tl1: tlp1, p2: other, tl2: tlp1}
            )
            sav_script( cpu, scheme, p0,
               {diffperiod: true, tl0: tlp0, 
                p1: other, tl1: tlp1, p2: other, tl2: tlp1, p3: other, tl3: tlp1}
            )
        end
    end
end

#Thread Scaling (Just FA)
def fa_thread_scaling
    $cpus.product( %w[fa] ).each do |cpu, scheme|
        $specint.product( $specint ).each do |p0,other|
            sav_script( cpu, scheme, p0, {p1: other} )
            sav_script( cpu, scheme, p0, {p1: other, p2: other} )
            sav_script( cpu, scheme, p0, {p1: other, p2: other, p3: other} )
        end
    end
end

end
