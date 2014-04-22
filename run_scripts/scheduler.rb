#------------------------------------------------------------------------------
# FCFS vs FR-FCFS
#------------------------------------------------------------------------------
require_relative './runscripts.rb'
include RunScripts

module RunScripts

#Cache Sweeping
def fcfs_vs_frfcfs
    cpus.product( schemes,  $cacheSizes ).each do |cpu, scheme, cacheSize|
        $specint.product( $specint ).each do|p0, p1|
            sav_script( cpu, scheme, p0, {p1: p1} )
        end
    end
end

end
