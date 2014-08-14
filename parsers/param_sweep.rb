#!/usr/bin/ruby
require 'colored'
require_relative 'parsers'
require_relative 'm5out'
require_relative 'graph'
include Parsers

def param_sweeping opts
    range = opts[:range] || []

    #build a hash from parameter values to the performance comparison with the
    #baseline
    add_avgs range.inject({}) { |h, v|
        #get a hash of the form: parameter->benchmark->overhead

        h[v] = opts[:regex].nil? ?
            perf_compare( opts ) {|p0, other|
                block_given? ? yield(p0, other, v) : {}
            } :
            compare_stats( opts ){|p0, other|
                block_given? ? yield(p0, other, v): {}
            }
        h
    }
end

# Take a hash of the form parameter->benchmark->overhead and return one that 
# includes an entry for the average across all benchmarks for each parameter
def add_avgs in_h 
  in_h.inject({}) { |out_h, (param, b_to_o)|
    nl = b_to_o.values.inject([]) { |vals, v|
      vals << v unless v == -1
      vals
    }
    avg = avg_arr nl
    out_h[param] = in_h[param].merge( 'avg' => avg )
    out_h
  }
end

def l2l3_sweep opts={}
    opts = {
        scheme: "none",
        range: [1,4,8,9,12,17,18]
    }.merge opts
    two_threads = param_sweeping(opts) do |p0, other, v|
        { p0: p0, p1: other, nametag: "l2l3resp_#{v}" }
    end
    three_threads = param_sweeping(opts) do |p0, other, v|
        { p0: p0, p1: other, p2: other, nametag: "l2l3resp_#{v}" }
    end
    four_threads = param_sweeping(opts) do |p0, other, v|
        { p0: p0, p1: other, p2: other, p3: other,
          nametag: "l2l3resp_#{v}" }
    end
    [two_threads, three_threads, four_threads ]
end

def membus_sweep opts={}
    opts = {
        scheme: "none",
        range: [1,4,8,9,12,17,18]
    }.merge opts
    two_threads = param_sweeping(opts) do |p0, other, v|
        { p0: p0, p1: other, nametag: "membusresp_#{v}" }
    end
    three_threads = param_sweeping(opts) do |p0, other, v|
        { p0: p0, p1: other, p2: other,
          nametag: "membusresp_#{v}" }
    end
    four_threads = param_sweeping(opts) do |p0, other, v|
        { p0: p0, p1: other, p2: other, p3: other,
          nametag: "membusresp_#{v}" }
    end
    [ two_threads, three_threads, four_threads ]
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
    three_threads = param_sweeping(opts) do |p0, other, v|
        { p0: p0, p1: other, p2: other,
          tl0: v, tl1: v, tl2: v }
    end
    four_threads = param_sweeping(opts) do |p0, other, v|
        { p0: p0, p1: other, p2: other, p3: other,
          tl0: v, tl1: v, tl2: v, tl3: v }
    end
    [ two_threads, three_threads, four_threads ]
end 

def fa_sweep opts = {}
    tp_sweep opts.merge( scheme: "fa" )
end

def results_to_s results
  #print heach benchmark
  r = $specint.inject("") { |s, b| s+=b+", " }[0..-2] +"\n"
  #for each paramter value, print the overhead
  results.each do |k,v|
    r += "#{k}" + v.inject("") { |s,pair| s+=", "+pair[1].to_s }
  end
  r
end

def results_to_s_t results
  #print pramater values
  ret = "%-14s" % ("bench,") + results.keys.inject("") { |s, v|
    s += "%-14s" % (v.to_s + ", ")
  } + "\n"
  ($specint + %w[avg]).each do |b|
    ret += "%-14s" % (b + ",") + results.values.inject("") { |s, r|
      s+= "%-14s" % (("%.4f" % r[b]) + ",")
    } + "\n"
  end
  ret
end

def results_to_a results, benchmarks=$specint+%w[avg]
  # the outer index is the benchmark, and the column index is the parameter 
  # value
  benchmarks.inject([]) do |return_val,b|
    return_val << results.values.inject([]) { |row, param_hash|
      row << param_hash[b]
      row
    }
    return_val
  end

end

if __FILE__ == $0
    out = ARGV[0].to_s
    FileUtils.mkdir_p(out) unless File.directory?(out)

    # Output results
    main = lambda do |opts|
      [2,3,4].each do |num_tcids|
        puts "="*80
        puts "   #{num_tcids} TCIDS"
        puts "="*80

        g_opts_bus = { legend: [1,4,8,9,12,17,18],
                       x_labels: $specint + %w[Mean] }
        g_opts_mem = { legend: [0,64,96,128,192,256],
                       x_labels: $specint + %w[Mean] }
        
        puts "l2l3 #{opts[:label]}:"
        r = l2l3_sweep(
          opts.merge(regex: opts[:regex].nil? ? nil : $l3_latency)
        )[num_tcids-2]
        rs = results_to_s_t r
        puts rs
        string_to_f rs, out+"/l2l3_#{opts[:label]}_#{num_tcids}.csv"
        rg = grouped_bar (results_to_a r), g_opts_bus
        string_to_f rg, out+"/l2l3_#{opts[:label]}_#{num_tcids}.svg"
        puts ""

        puts "membus #{opts[:label]}:"
        r = membus_sweep(opts)[num_tcids-2]
        rs = results_to_s_t r
        puts rs
        string_to_f rs, out+"/membus_#{opts[:label]}_#{num_tcids}.csv"
        rg = grouped_bar (results_to_a r), g_opts_bus
        string_to_f rg, out+"/membus_#{opts[:label]}_#{num_tcids}.svg"
        puts ""

        puts "tp #{opts[:label]}:"
        r = tp_sweep(opts)[num_tcids-2]
        rs = results_to_s_t r
        puts rs
        string_to_f rs, out+"/tp_#{opts[:label]}_#{num_tcids}.csv"
        rg = grouped_bar (results_to_a r), g_opts_mem
        string_to_f rg, out+"/tp_#{opts[:label]}_#{num_tcids}.svg"
        puts ""

        puts "fa #{opts[:label]}:"
        r = fa_sweep(opts)[num_tcids-2]
        rs = results_to_s_t r
        puts rs
        string_to_f rs, out+"/fa_#{opts[:label]}_#{num_tcids}.csv"
        rg = grouped_bar (results_to_a r), g_opts_mem
        string_to_f rg, out+"/fa_#{opts[:label]}_#{num_tcids}.svg"
        puts ""
      end
    end

    #Execution Time Overhead
    main.call(otherbench: %w[mcf], label: "execution")

    # L3 Miss Latency Overhead
    main.call(otherbench: %w[astar],
              regex: $mem_latency,
              label: "latency_overhead")

end
