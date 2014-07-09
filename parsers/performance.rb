#!/usr/bin/ruby
require 'colored'
require_relative 'parsers'
include Parsers

def perf_compare_prelim opts
    perf_compare( opts ){|p0,other| {p0: p0, p1: other } }
end

def perf_compare_scalability opts
    one_tc    = perf_compare( opts ){|p0,other| { p0: p0, p1: nil} }
    two_tcs   = perf_compare( opts ){|p0,other| { p0: p0, p1: other } }
    three_tcs = perf_compare( opts ){|p0,other|
        { p0: p0, p1: other, p2: other }
    }
    four_tcs  = perf_compare( opts ){|p0,other|
        { p0: p0, p1: other, p2: other, p3: other}
    }
    [
        one_tc,
        two_tcs,
        three_tcs,
        four_tcs
    ]
end

def perf_compare_breakdown opts
    rr_bus    = perf_compare( opts ){|p0,other|
        {p0: p0, p1: other, nametag: "rrbus" }
    }
    setpart   = perf_compare( opts ){|p0,other|
        {p0: p0, p1: other, nametag: "setpart" }
    }
    splitmshr = perf_compare( opts ){|p0,other|
        {p0: p0, p1: other, nametag: "splitmshr" }
    }
    splitrport = perf_compare( opts ){|p0,other|
        {p0: p0, p1: other, nametag: "splitrport" }
    }
    tp = perf_compare( opts ){|p0,other|
        { p0: p0, p1: other }
    }
    {
        "rr_bus      " => rr_bus,
        "setpart     " => setpart,
        "splitmshr   " => splitmshr,
        "splitrport  " => splitrport,
        "tp          " => tp,
    }
end

def results_to_stdout results
    results.each do |k,v|
        puts "#{k}, #{v}"
    end
end

if __FILE__ == $0
    # Preliminary Results
    results_to_stdout perf_compare_prelim(
        dir: "results_preliminary",
        otherbench: %w[ astar ],
        use_base_avg: true,
    )

    # Scalability
    (1..4).each do |num_tcs|
        puts "#{num_tcs} TCs"
        opts = {
            nametag: "coordinated",
            dir: "results_scalability",
            otherbench: %w[ astar ],
            use_base_avg: true,
        }
        results_to_stdout(
            perf_compare_scalability( opts )[num_tcs-1]
        )
    end

    # Performance Breakdown 
    results_to_tdout perf_compare_breakdown(
       dir: "results_breakdown",
       otherbench: %w[ astar ],
       use_base_avg: true,
    )
end
