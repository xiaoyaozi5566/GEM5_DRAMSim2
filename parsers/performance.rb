#!/usr/bin/ruby
require 'colored'
require_relative 'parsers'
include Parsers

def perf_compare( conf = {} )
    conf = {
        cpu: "detailed",
        scheme: "tp",
        bench: $specint - %w[ bzip2 ],
        use_base_avg: false,
    }.merge conf

    conf[:otherbench]=conf[:bench]  if conf[:otherbench].nil?
    use_base_avg = conf[:use_base_avg]

    results = {}
    conf[:bench].each do |p0|
        overheads = []
        (conf[:otherbench]).each do |other|
            fopts = ( block_given? && conf.merge(yield(p0,other)) ) ||
                conf.merge({ p0:p0, p1: other })
            overhead = ( use_base_avg && diff_from_base_avg( fopts ) ) ||
                       ( percent_overhead fopts )
            overheads << overhead unless overhead.nil?
        end
        results[p0] = avg_arr overheads
    end
    results
end

def base_avg( conf = {} )
    conf = {
        cpu: "detailed",
        scheme: "tp",
        bench: $specint - %w[ bzip2 ]
    }.merge conf

    conf[:otherbench] = conf[:bench] if conf[:otherbench].nil?

    conf[:bench].inject({}){|h,p0|
        overheads = conf[:otherbench].inject([]){|o,other|
            yieldopts = conf.merge( ( block_given? && yield(p0,other) ) ||
                {p0: p0, p1: other}
            )
            fopts = yieldopts.merge({ nametag:nil, scheme: "none" })
            time,found = findTime( stdo_file( fopts ), fopts )
            puts "#{stdo_file fopts} time not found!" unless found
            o << time if found; o
        }
        h[p0] = avg_arr overheads; h
    }
end

def diff_from_base_avg( opts = {} )
    time, found = findTime( stdo_file( opts ), opts )
    basetime = base_avg( opts )[ opts[:p0] ]
    return nil unless found
    (time-basetime)/basetime * 100
end

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

def results_to_stdout results
    results.each do |k,v|
        puts "#{k}, #{v}"
    end
end


if __FILE__ == $0
    results_to_stdout perf_compare_prelim(
        dir: "results_preliminary",
        otherbench: %w[ astar ],
        use_base_avg: true,
    )

    (1..4).each do |num_tcs|
        puts "#{num_tcs} TCs"
        opts = {
            dir: "results_scalability",
            otherbench: %w[ astar ],
            use_base_avg: true,
        }
        results_to_stdout(
            perf_compare_scalability( opts )[num_tcs-1]
        )
    end
    
end
