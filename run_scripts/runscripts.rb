#!/usr/bin/ruby

require 'fileutils'
require 'colored'
require 'parallel'

module RunScripts
#directories
$gem5home = Dir.new(Dir.pwd)
$specint_dir = (Dir.pwd+"/benchmarks/spec2k6bin/specint")
$scriptgen_dir = Dir.new(Dir.pwd+"/scriptgen")

#Gem5 options
$fastforward = 10**9
$maxinsts = 10**8
$maxtick = 2*10**15 
$cpus = %w[detailed timing]
$cacheSizes = [0,1,2,4]
$p0periods = [64,96,128,192,256]
$l3configs = %w[shared private]

#dramsim options
$device = "DDR3_micron_16M_8B_x8_sg15.ini"
$schemes = %w[tp none fa]
$turnlengths = [0] + (7..9).to_a
$p0periods = [64,96,128,192,256]

#benchmarks
$specinvoke = { 
   #"perlbench"  => "'#{$specint_dir}/perlbench -I#{$specint_dir}/perldepends -I#{$specint_dir}/lib #{$specint_dir} pack.pl'",
    "bzip2"      => "'#{$specint_dir}/bzip2 #{$specint_dir}/input.source 280'",
    "gcc"        => "'#{$specint_dir}/gcc #{$specint_dir}/200.in -o results/200.s'",
    "mcf"        => "'#{$specint_dir}/mcf #{$specint_dir}/inp.in'",
    "gobmk"      => "'#{$specint_dir}/gobmk --quiet --mode gtp --gtp-input #{$specint_dir}/13x13.tst'",
    "hmmer"      => "'#{$specint_dir}/hmmer #{$specint_dir}/nph3.hmm #{$specint_dir}/swiss41'",
    "sjeng"      => "'#{$specint_dir}/sjeng #{$specint_dir}/ref.txt'",
    "libquantum" => "'#{$specint_dir}/libquantum 1397 8'",
    "h264ref"    => "'#{$specint_dir}/h264ref -d #{$specint_dir}/foreman_ref_encoder_baseline.cfg'",
   #"omnetpp"    => "'#{$specint_dir}/omnetpp #{$specint_dir}/omnetpp.ini'",
    "astar"      => "'#{$specint_dir}/astar #{$specint_dir}/BigLakes2048.cfg'",
    "xalan"      => "'#{$specint_dir}/Xalan -v #{$specint_dir}/t5.xml #{$specint_dir}/xalanc.xsl'"  
}
$specint = $specinvoke.keys.sort

$synthinvoke = {
    "hardstride1" => "#{$synthbench_dir}hardstride -d #{$duration} -p 1",
    "randmem1"    => "#{$synthbench_dir}randmem -d #{$duration} -p 1",
    #"randmem10"   => "#{$synthbench_dir}randmem -d #{$duration} -p 10",
    #"randmem100"  => "#{$synthbench_dir}randmem -d #{$duration} -p 100",
    "nothing"     => "#{$synthbench_dir}nothing"
}
$synthb = $synthinvoke.keys.sort

def invoke( name )
    $specinvoke[name] || $synthinvoke[name]
end

def sav_script( cpu, scheme, p0, options = {} ) 

    options = {
        cacheSize: 4,
        tl0: 6,
        tl1: 6,
        tl2: 6,
        tl3: 6,
        l3config: "shared",
        runmode: :qsub,
        maxinsts: $maxinsts,
        fastforward: $fastforward,
        result_dir: "results"
    }.merge options

    cacheSize  = options[:cacheSize]
    # workloads to run on p1-p3
    p1         = options[:p1]
    p2         = options[:p2]
    p3         = options[:p3]
    # turn length for p0-p3. Assumed equal unless diffperiod is supplied. The 
    # turn length is 2**arg unless diffperiod is supplied, otherwise it is arg.
    tl0        = options[:tl0]
    tl1        = options[:tl1]
    tl2        = options[:tl2]
    tl3        = options[:tl3]
    # Results directory
    result_dir = options[:result_dir]
    # allows the turn lengths for p0-p3 to differ
    diffperiod = options[:diffperiod]
    # shared or private l3
    l3config   = options[:l3config]
    # runmode can be qsub: to submit jobs, local: to run the test locally, or 
    # none: to generate the scripts without running them
    runmode    = options[:runmode]
    # maximum number of instructions
    maxinsts   = options[:maxinsts]
    # number of instructions to fastforward,
    # 0 removes --fastforward from the script
    fastforward= options[:fastforward]
    # Should L3 be set partitioned?
    use_set_part = options[:setpart]
    # Should L3 be way partitioned?
    use_way_part = options[:waypart]
    # Use a round robin noncoherent bus
    rr_nc        = options[:rr_nc]
    # Determines if trace files for security should be saved
    savetraces = options[:savetraces]
    # Determines l3 trace file output.
    l3tracefile= options[:l3tracefile]
    # Produce Gem5 builtin cache traces
    cacheDebug = options[:cacheDebug]
    # Produce Gem5 builtin MMU traces
    mmuDebug   = options[:mmuDebug]
    # Use gem5.debug instead of gem5.fast
    debug = options[:debug] ||
        options[:cacheDebug] ||
        options[:gdb] ||
        options[:memdebug] ||
        options[:mmuDebug]
    

    filename  = "#{scheme}_#{cpu}_#{p0}tl#{tl0}"
    filename += "_#{p1}tl#{tl1}" unless p1.nil?
    filename += "_#{p2}tl#{tl2}" unless p2.nil?
    filename += "_#{p3}tl#{tl3}" unless p3.nil?
    filename += "_c#{cacheSize}MB"

    filename = "#{options[:nametag]}_"+filename if options[:nametag]
   
    numpids = [p0,p1,p2,p3].inject(0){|sum,i| ( i.nil? && sum ) || sum+1}

    FileUtils.mkdir_p( result_dir ) unless File.directory?( result_dir )

    script = File.new($scriptgen_dir.path+"/run_#{filename}","w+")
    script.puts("#!/bin/bash")
    script.puts("build/ARM/gem5.fast \\") unless debug
    script.puts("build/ARM/gem5.debug \\") if debug 
    script.puts("--debug-flags=Cache \\") if cacheDebug
    script.puts("--debug-flags=MMU \\") if mmuDebug
    script.puts("--debug-flags=Bus,MMU,Cache\\") if options[:memdebug]
    script.puts("    --stats-file=#{filename}_stats.txt \\")
    script.puts("    configs/dramsim2/dramsim2_se.py \\")
    script.puts("    --cpu-type=#{cpu} \\")
    script.puts("    --caches \\")
    script.puts("    --l2cache \\")
    unless cacheSize == 0
        script.puts("    --l3cache \\")
        script.puts("    --l3_size=#{cacheSize}MB\\")
        script.puts("    --l3config=#{l3config} \\")
    end
    script.puts("    --fast-forward=#{fastforward} \\") unless fastforward == 0
    script.puts("    --maxinsts=#{maxinsts} \\")
    script.puts("    --maxtick=#{$maxtick} \\")

    #Protection Mechanisms
    script.puts("   --fixaddr\\") if scheme == "fa" || options[:addrpar]
    script.puts("    --rr_nc\\" ) if rr_nc
    script.puts("    --use_set_part\\" ) if use_set_part
    script.puts("    --use_way_part\\" ) if use_way_part
    script.puts("    --split_mshr\\"   ) if options[:split_mshr]
    script.puts("    --split_rport\\"   ) if options[:split_rport]

    #Time Quanta and Offsets
    # Assumes all or none are passed. Default if none are passed.
    unless options[:l2l3req_tl].nil?
        script.puts("    --l2l3req_tl #{options[:l2l3req_tl]}\\")
        script.puts("    --l2l3req_offset #{options[:l2l3req_offset]}\\")
        script.puts("    --l2l3resp_tl #{options[:l2l3resp_tl]}\\")
        script.puts("    --l2l3resp_offset #{options[:l2l3resp_offset]}\\")
        script.puts("    --membusreq_tl #{options[:membusreq_tl]}\\")
        script.puts("    --membusreq_offset #{options[:membusreq_offset]}\\")
        script.puts("    --membusresp_tl #{options[:membusresp_tl]}\\")
        script.puts("    --membusresp_offset #{options[:membusresp_offset]}\\")
        script.puts("    --dramoffset #{options[:dramoffset]}\\")
    end

    #Trace Options
    script.puts("    --do_cache_trace \\") if options[:do_cache_trace]
    l3tracefile  = l3tracefile || "#{result_dir}/l3trace_#{filename}.txt"
    script.puts("    --l3tracefile #{l3tracefile}\\") if options[:do_cache_trace]
    script.puts("    --do_bus_trace \\"  ) if options[:do_bus_trace]
    membustracefile = options[:bus_trace_file] || "#{result_dir}/membustrace_#{filename}.txt"
    l2l3bustracefile = options[:bus_trace_file] || "#{result_dir}/l2l3bustrace_#{filename}.txt"
    script.puts("    --membustracefile #{membustracefile}\\") if options[:do_bus_trace]
    script.puts("    --l2l3bustracefile #{l2l3bustracefile}\\") if options[:do_bus_trace]
    script.puts("    --do_mem_trace \\"  ) if options[:do_mem_trace]
    memtracefile = options[:mem_trace_file] || "#{result_dir}/memtrace_#{filename}.txt"
    script.puts("    --mem_trace_file #{memtracefile}\\") if options[:do_mem_trace]
    l2tracefile  = options[:l2tracefile] || "#{result_dir}/l2trace_#{filename}.txt"
    script.puts("    --l2tracefile #{l2tracefile}\\") if options[:do_cache_trace]

    script.puts("    --dramsim2 \\")
    script.puts("    --tpturnlength=#{tl0} \\") unless tl0==0 || diffperiod
    script.puts("    --devicecfg="+
                "./ext/DRAMSim2/ini/#{$device} \\")
    if tl0== 0
        script.puts("    --systemcfg=./ext/DRAMSim2/system_ft.ini \\")
    else
        script.puts("    --systemcfg=./ext/DRAMSim2/system_#{scheme}.ini \\")
    end
    script.puts("    --outputfile=/dev/null \\")
    script.puts("    --numpids=#{numpids} \\")
    script.puts("    --p0=#{invoke(p0)}\\")
    script.puts("    --p1=#{invoke(p1)}\\") unless p1.nil?
    script.puts("    --p2=#{invoke(p2)}\\") unless p2.nil?
    script.puts("    --p3=#{invoke(p3)}\\") unless p3.nil?
    if diffperiod
        script.puts("    --diffperiod \\")
        script.puts("    --p0period=#{tl0} \\")
        script.puts("    --p1period=#{tl1} \\")
    end
    script.puts("    > #{result_dir}/stdout_#{filename}.out")
    script_abspath = File.expand_path(script.path)
    script.close
    if runmode == :qsub
        success = system "qsub -wd #{$gem5home.path} -e stderr/ -o stdout/ #{script_abspath}"
    end
    puts "#{filename}".magenta if runmode == :local
    success = system "sh #{script_abspath}" if runmode == :local
    [success,filename]
end

def parallel_local opts
    opts = {
        cpus: $cpus,
        schemes: $schemes,
        benchmarks: $specint,
        runmode: :local,
    }.merge opts

    opts[:otherbench] = opts[:benchmarks] if opts[:otherbench].nil?

    failed = []

    opts[:cpus].product( opts[:schemes] ).each do |cpu, scheme|
        Parallel.each( opts[:benchmarks].product( opts[:otherbench] ),
                      in_threads: 4 ) do |p0, p1|
            iteropts = { p1: p1 }
            r = sav_script( cpu, scheme, p0, opts.merge( iteropts ) )
            failed << r[1] unless r[0] 
        end
    end
    failed.each{|f| puts f.red}
end

def qsub_fast opts
    opts = {
        cpus: $cpus,
        schemes: $schemes,
        benchmarks: $specint,
        runmode: :qsub,
    }.merge opts
    opts[:otherbench] = opts[:benchmarks] if opts[:otherbench].nil?

    opts[:cpus].product( opts[:schemes] ).each do |cpu, scheme|
        opts[:benchmarks].product( opts[:otherbench] ).each do |p0, p1|
            iteropts = { p1: p1 }
            sav_script( cpu, scheme, p0, opts.merge( iteropts ) )
        end
    end

end

def qsub_fast_scaling opts
    opts = {
        cpus: $cpus,
        schemes: $schemes,
        benchmarks: $specint,
        runmode: :qsub,
    }.merge opts

    opts[:otherbench] = opts[:benchmarks] if opts[:otherbench].nil?

    opts[:cpus].product( opts[:schemes] ).each do |cpu, scheme|
        opts[:benchmarks].product( opts[:otherbench] ).each do |p0, other|
            sav_script( cpu, scheme, p0, 
                        opts.merge(p1: other)  )
            sav_script( cpu, scheme, p0,
                        opts.merge( p1: other, p2: other ) )
            sav_script( cpu, scheme, p0,
                        opts.merge( p1: other, p2: other, p3: other ) )
        end
    end
end


def parallel_local_scaling opts
    opts = {
        cpus: $cpus,
        schemes: $schemes,
        benchmarks: $specint,
        runmode: :local,
    }.merge opts

    opts[:otherbench] = opts[:benchmarks] if opts[:otherbench].nil?

    failed = []

    opts[:cpus].product( opts[:schemes] ).each do |cpu, scheme|
        Parallel.each( opts[:benchmarks].product( opts[:otherbench] ),
                      in_threads: 4 ) do |p0, other|
            r = sav_script( cpu, scheme, p0, opts )
            failed << r[1] unless r[0] 
            r = sav_script( cpu, scheme, p0,
                           opts.merge( p1: other ) )
            failed << r[1] unless r[0] 
            r = sav_script( cpu, scheme, p0,
                           opts.merge( p1: other, p2: other) )
            failed << r[1] unless r[0] 
            r = sav_script( cpu, scheme, p0,
                           opts.merge( p1: other, p2: other, p3:other ) )
            failed << r[1] unless r[0] 
        end
    end
    failed.each{|f| puts f.red}
end

end
