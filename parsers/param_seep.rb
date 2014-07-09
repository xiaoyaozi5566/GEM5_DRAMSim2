#!/usr/bin/ruby
require 'colored'
require_relative 'parsers'
include Parsers

def param_sweeping opts
    range = opts[:range] || []

    #build a hash from parameter values to the performance comparison with the
    #baseline
    range.inject({}) { |h, v|
        #get a hash to specify the name
        h[v] = perf_compare( opts ) do |p0, other|
            block_given? ? yield(p0, other, v) : {}
        end
        h
    }
end

def l2l3_sweep opts={}
    opts = {
        scheme: "none",
        range: [1,4,8,9,16,17,25,32]
    }.merge opts
    two_threads = param_sweeping(opts) do |p0, other, v|
        { p0: p0, p1: other, nametag: "l2l3resp_#{v}" }
    end
    # three_threads = param_sweeping(opts) do |p0, other, v|
    #     { p0: p0, p1: other, p2: other, nametag: "l2l3resp_#{v}" }
    # end
    # four_threads = param_sweeping(opts) do |p0, other, v|
    #     { p0: p0, p1: other, p2: other, p3: other,
    #       nametag: "l2l3resp_#{v}" }
    # end
    #[ two_threads, three_threads, four_threads #]
end

def membus_sweep opts={}
    opts = {
        scheme: "none",
        range: [1,4,8,9,16,17,25,32]
    }.merge opts
    two_threads = param_sweeping(opts) do |p0, other, v|
        { p0: p0, p1: other, nametag: "membusresp_#{v}" }
    end
    # three_threads = param_sweeping(opts) do |p0, other, v|
    #     { p0: p0, p1: other, p2: other,
    #       nametag: "membusresp_#{v}" }
    # end
    # four_threads = param_sweeping(opts) do |p0, other, v|
    #     { p0: p0, p1: other, p2: other, p3: other,
    #       nametag: "membusresp_#{v}" }
    # end
    # [ two_threads, three_threads, four_threads ]
end 

def tp_sweep opts={}
    opts = {
        scheme: "tp",
        range: [0,64,96,128,192,256]
    }.merge opts
    two_threads = param_sweeping(opts) do |p0, other, v|
        { p0: p0, p1: other,
          tl0: v, tl1: v }
    end
    # three_threads = param_sweeping(opts) do |p0, other, v|
    #     { p0: p0, p1: other, p2: other,
    #       tl0: v, tl1: v, tl2: v }
    # end
    # four_threads = param_sweeping(opts) do |p0, other, v|
    #     { p0: p0, p1: other, p2: other, p3: other,
    #       tl0: v, tl1: v, tl2: v }
    # end
    # [ two_threads, three_threads, four_threads ]
end 

def fa_sweep opts = {}
    tp_sweep opts.merge( scheme: "fa" )
end

def results_to_stdout results
    puts $specint.inject("") { |s, b| s+=b+", "; s }
    results.each do |k,v|
        puts "#{k}" + v.inject("") { |s,pair| s+=", "+pair[1].to_s; s }
    end
end

if __FILE__ == $0
    #puts "l2l3 sweeping:"
    #results_to_stdout l2l3_sweep(
    #    otherbench: %w[astar],
    #    scheme: "tp"
    #)
    #puts ""

    puts "fa sweeping:"
    results_to_stdout fa_sweep(
        #otherbench: %w[astar]
    )
end
