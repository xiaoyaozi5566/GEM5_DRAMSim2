#------------------------------------------------------------------------------
# Security Tests
#------------------------------------------------------------------------------
require_relative "runscripts"
require 'parallel'
require 'colored'
include RunScripts

module RunScripts
    def test_sanity_ opts
        cpus = $cpus
        schemes = %w[ none tp ]
        benchmarks = $specint
        opts = {
            maxinsts: 10**3,
            fastforward: 100,
            runmode: :local,
            debug: true,
        }

        failed = []

        cpus.product( schemes ).each do |cpu, scheme|
            Parallel.each( benchmarks.combination( 2 ),
                          in_threads: 4 ) do |p0, p1|
                iteropts = { p1: p1 }
                r = sav_script( cpu, scheme, p0, opts.merge( iteropts ) )
                failed << r[1] unless r[0] 
            end
        end
        failed.each{|f| puts f.red}
    end

    def test_sanity
        opts = {
            maxinsts: 10**3,
            fastforward: 100,
            runmode: :local,
            debug: true,
        }
        test_sanity_ opts
    end

    def test_sanity_deep_ opts
        cpus = %w[ detailed ]
        schemes = %w[ fa none tp ]
        benchmarks = %w[ mcf sjeng ]

        failed = []

        cpus.product( schemes ).each do |cpu, scheme|
            Parallel.each( benchmarks.combination( 2 ),
                          in_threads: 4 ) do |p0, p1|
                iteropts = { p1: p1 }
                r = sav_script( cpu, scheme, p0, opts.merge( iteropts ) )
                failed << r[1] unless r[0]
            end
        end

        failed.each{|f| puts f.red}
    end

    def test_sanity_deep
        opts = {
            maxinsts: 10**6,
            fastforward: 100,
            runmode: :local,
            debug: true,
        }
        test_sanity_deep_ opts
    end

    def test_set_part
        opts = {
            maxinsts: 10**3,
            fastforward: 100,
            runmode: :local,
            debug: true,
            setpart: true,
        }
        test_sanity_ opts
    end

    def test_set_part_deep
        opts = {
            maxinsts: 10**6,
            fastforward: 100,
            runmode: :local,
            debug: true,
            setpart: true,
        }
        test_sanity_deep_ opts
    end

end
