#------------------------------------------------------------------------------
# Synthetic Benchmark Runs
#------------------------------------------------------------------------------

require_relative './runscripts.rb'
include RunScripts

module RunScripts

def synthetic_asym_period
    p0periods = [64, 96, 128, 192, 256]
    p0periods.product($schemes).each do |tl0,scheme|
        $tests.keys.product($tests.keys).each do |p0,p1|
            sav_script( "detailed", scheme, p0,
                       {p1: p1, tl0: tl0, diffperiod: true} )
        end
    end
end

end
