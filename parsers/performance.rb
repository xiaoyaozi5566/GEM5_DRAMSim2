#!/usr/bin/ruby
require 'colored'
require_relative 'parsers'
require_relative 'graph'
include Parsers

def perf_compare_prelim opts
    perf_compare( opts ){|p0,other| {p0: p0, p1: other } }
end

def perf_compare_scalability opts
    two_tcs   = perf_compare( opts ){|p0,other| { p0: p0, p1: other } }
    three_tcs = perf_compare( opts ){|p0,other|
        { p0: p0, p1: other, p2: other }
    }
    four_tcs  = perf_compare( opts ){|p0,other|
        { p0: p0, p1: other, p2: other, p3: other}
    }
    [
        two_tcs,
        three_tcs,
        four_tcs
    ]
end

def scalability_wmean opts
  r = perf_compare_scalability opts
  r_full = perf_compare_scalability opts.merge(
    bench: $specint
  )
  r.map.with_index { |i,index|
    #puts avg_arr( r_full[index].values )
    i.values << avg_arr(r_full[index].values)
  }
end

def perf_compare_breakdown opts
    l2l3    = perf_compare( opts ){|p0,other|
        {p0: p0, p1: other, nametag: "only_l2l3" }
    }
    waypart = perf_compare( opts ){|p0,other|
        {p0: p0, p1: other, nametag: "only_waypart" }
    }
    membus = perf_compare( opts ){|p0,other|
        {p0: p0, p1: other, nametag: "only_membus" }
    }
    mc     = perf_compare( opts ){|p0,other|
        {p0: p0, p1: other, nametag: "only_mc" }
    }
    [
      # l2l3,
      # membus,
      #waypart,
      (1..10).inject({}){|h,i| h[i]=1;h},
      (1..10).inject({}){|h,i| h[i]=1;h},
      (1..10).inject({}){|h,i| h[i]=3;h},
      mc
    ]
end

def results_to_stdout results
    results.each do |k,v|
        puts "#{k}, #{v}"
    end
end

if __FILE__ == $0
  in_dir  = ARGV[0].to_s
  out_dir = ARGV[1].to_s
  FileUtils.mkdir_p(out_dir) unless File.directory?(out_dir)

  # Scalability Results
  r = scalability_wmean(
    dir: in_dir,
    #otherbench: %w[ astar ],
    use_base_avg: true,
  )
  gb = grouped_bar r.transpose, legend: [2,3,4], x_labels: $specint + %w[mean]
  string_to_f gb, "#{out_dir}/scalability.svg"

  # Scalability w/o libquantum
  r = scalability_wmean(
    dir: in_dir,
    bench: $specint - %w[libquantum],
    otherbench: $specint,
    use_base_avg: true,
  )
  gb = grouped_bar r.transpose, legend: [2,3,4],
    x_labels: $specint - %w[libquantum] + %w[mean],
    w: COLUMN_W * 10/11.0
  string_to_f gb, "#{out_dir}/scalability_nolibq.svg"

  # Scalability libquantum
  r = perf_compare_scalability(
    dir: in_dir,
    bench: %w[libquantum],
    otherbench: $specint,
    use_base_avg: true,
  ).map { |i| i.values }
  gb = grouped_bar r.transpose,
    x_labels: %w[libquantum],
    w: COLUMN_W * 1/11.0
  string_to_f gb, "#{out_dir}/scalability_libq.svg"

  #Breakdown Results
  r = perf_compare_breakdown(
    dir: in_dir,
    use_base_avg: true
  ).map { |i| i.values }
  sb = stacked_bar r, legend: %w[l2l3 membus waypart mc],
    x_labels: $specint, w: COLUMN_W, h: COLUMN_H
  string_to_f sb, "#{out_dir}/breakdown.svg"

end
