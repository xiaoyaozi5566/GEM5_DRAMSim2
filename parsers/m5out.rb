require_relative 'parsers'
require_relative 'graph'
include Parsers

module Parsers
MEMLATENCY = /system.l3.overall_avg_miss_latency::total\s*(\d*.\d*)/
$l3_latency  = /system.l20.overall_avg_miss_latency::total\s*(\d*.\d*)/
$l3_miss     = /system.l3.overall_misses::total[\s]*(\d*)/

def get_stats( conf = {} )
    conf = {
        bench: $specint,
        scheme: "none",
        cpu: "detailed",
    }.merge conf
    conf[:otherbench] = conf[:otherbench] || conf[:bench]

    conf[:bench].inject({}) {|hsh,p0|
        hsh[p0] = avg_arr(
            conf[:otherbench].inject([]) {|p0stats,other|
                bench_specifier = block_given? ?
                    yield(p0, other) :
                    { p0: p0, p1: other }
                datum, found = get_datum(
                    m5out_file( conf.merge( bench_specifier ) ),
                    conf[:regex]
                )
                p0stats << datum.to_f if found
                p0stats
            }
        ); hsh
    }
end

def compare_stats( conf = {} )
    conf = {
        bench: $specint,
        cpu: "detailed",
        scheme: "tp",
        base_scheme: "none",
    }.merge conf
    conf[:otherbench] = conf[:otherbench] || conf[:bench]

    conf[:bench].inject({}) {|hsh,p0|
        hsh[p0] = avg_arr(
            conf[:otherbench].inject([]) {|p0stats,other|
                bench_specifier = block_given? ?
                    yield(p0, other) :
                    { p0: p0, p1: other }
                p = conf.merge( bench_specifier )
                datum, found = get_datum(
                    m5out_file( p ),
                    conf[:regex]
                )
                datum_base, found_base = get_datum(
                    m5out_file(
                        p.merge(
                            scheme: conf[:base_scheme],
                            nametag: conf[:base_nametag],
                            tl0: 6, tl1: 6, tl2: 6, tl3: 6
                        )
                    ),
                    conf[:regex]
                )
                p0stats << overhead(datum.to_f, datum_base.to_f,
                                   ) if found && found_base
                p0stats
            }
        ) ; hsh
    }
end

end

if __FILE__ == $0
    input_dir = ARGV[0].to_s
    result_dir = ARGV[1].to_s
    
    FileUtils.mkdir_p( result_dir ) unless File.directory?( result_dir )
   
    # MPKI For most spec benchmarks 
    mpki = get_stats(
      dir: input_dir,
      regex: $l3_miss,
      otherbench: %w[astar],
      bench: $specint - %w[mcf libquantum]
    ).to_a.inject({}){|hsh,kv|
        hsh[kv[0]] = kv[1] * 1000 / 10**9
        hsh
    }
    puts "Baseline memory requests per thousand instructions: \n" + mpki.to_s
    g = simple_bar(
      mpki.values,
      w: COLUMN_W*9/11.0,
      x_title: "",
      x_labels: $specint - %w[mcf libquantum]
    )
    string_to_f g, "#{result_dir}/mpki_most.svg"
    hash_to_csv( mpki, result_dir+"/mpki_most.csv" )

    #MPKI For libquantum and mcf
    mpki = get_stats(
      dir: input_dir,
      regex: $l3_miss,
      otherbench: %w[astar],
      bench: %w[mcf libquantum]
    ).to_a.inject({}){|hsh,kv|
        hsh[kv[0]] = kv[1] * 1000 / 10**9
        hsh
    }
    puts "Baseline memory requests per thousand instructions: \n" + mpki.to_s
    g = simple_bar(
      mpki.values,
      w: COLUMN_W*2/11.0,
      x_title: "MPKI",
      x_labels: %w[mcf libquantum]
    )
    string_to_f g, "#{result_dir}/mpki_ml.svg"
    hash_to_csv( mpki, result_dir+"/mpki_ml.csv" )
    # baseline_latency = get_stats( scheme: "none", dir: input_dir, regex: MEMLATENCY )
    # puts "Baseline Memory Latency: \n" + baseline_latency.to_s
    # hash_to_csv( baseline_latency, result_dir+"/baseline_latency.csv" )

    # secure_latency = get_stats(
    #     scheme: "tp",
    #     dir: input_dir,
    #     tl0: 64,
    #     tl1: 64,
    #     regex: MEMLATENCY,
    #     otherbench: %w[astar]
    # )
    # puts "Secure Baseline Memory Latency: \n" + secure_latency.to_s
    # hash_to_csv(  secure_latency, result_dir+"/secure_latency.csv" )

    # latency_overhead = compare_stats(
    #     dir: input_dir,
    #     tl0: 64,
    #     tl1: 64,
    #     regex: MEMLATENCY,
    #     otherbench: %w[astar]
    # )
    # puts "Secure Baseline Latency Overhead: \n" + latency_overhead.to_s
    # hash_to_csv( latency_overhead, result_dir+"/sec_baseline_overhead.csv" )

    # latency_overhead = compare_stats( dir: input_dir,
    #                                  nametag: "bad_coordinated",
    #                                  regex: MEMLATENCY,
    #                                  otherbench: %w[astar] )
    # puts "Bad Coordinated Latency Overhead: \n" + latency_overhead.to_s
    # hash_to_csv( latency_overhead, result_dir+"/bad_coordinated_overhead.csv" )

    # latency_overhead = compare_stats( dir: input_dir,
    #                                  nametag: "coordinated",
    #                                  regex: MEMLATENCY,
    #                                  otherbench: %w[astar] )
    # puts "Coordinated Latency Overhead: \n" + latency_overhead.to_s
    # hash_to_csv( latency_overhead, result_dir+"/coordinated_overhead.csv" )
end
