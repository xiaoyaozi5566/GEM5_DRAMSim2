#!/usr/bin/ruby

require 'fileutils'
require 'colored'

module RunScripts
#directories
$gem5home = Dir.new(Dir.pwd)
$specint_dir = (Dir.pwd+"/benchmarks/spec2k6bin/specint")
$scriptgen_dir = Dir.new(Dir.pwd+"/scriptgen")

#Gem5 options
$fastforward = 10**9
$maxinsts = 10**8
$maxtick = 2*10**15 
$cpus = %w[detailed] #timing is also available
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
    script.puts("    --fixaddr \\")       if scheme == "fa" || options[:addrpar]
    script.puts("    --rr_nc \\" )        if rr_nc
    script.puts("    --rr_l2l3 \\")       if options[:rr_l2l3]
    script.puts("    --rr_mem \\")        if options[:rr_mem]
    script.puts("    --use_set_part \\" ) if use_set_part
    script.puts("    --use_way_part \\")  if use_way_part
    script.puts("    --split_mshr \\")    if options[:split_mshr]
    script.puts("    --split_rport \\")   if options[:split_rport]

    #Time Quanta and Offsets
    [
      :l2l3req_tl,
      :l2l3req_offset,
      :l2l3resp_tl,
      :l2l3resp_offset,
      :membusreq_tl,
      :membusreq_offset,
      :membusresp_tl,
      :membusresp_offset,
      :dramoffset
    ].each do |param|
      unless options[param].nil?
       script.puts("    --#{param.to_s} #{options[param]} \\")
      end
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


    FileUtils.mkdir_p( "stderr" ) unless File.directory?( "stderr" )
    FileUtils.mkdir_p( "stdout" ) unless File.directory?( "stdout" )

    if runmode == :qsub
        success = system "qsub -wd #{$gem5home.path} -e stderr/ -o stdout/ #{script_abspath}"
    end
    puts "#{filename}".magenta if runmode == :local
    success = system "sh #{script_abspath}" if runmode == :local
    [success,filename]
end

def iterate_and_submit opts={}, &block
    o = {
        cpus: %w[detailed],
        schemes: $schemes,
        benchmarks: $specint,
        runmode: :local,
        threads: 4
    }.merge opts

    o[:otherbench] = o[:benchmarks] if o[:otherbench].nil?

    f = []

    submit = block_given? ?
      block :
      ( lambda do |cpu, scheme, param, p0, other|
          r = sav_script(cpu, scheme, p0, param.merge(p1: other))
          (r[0] && [] ) || r[1]
        end
      )
      
    o[:cpus].product(o[:schemes]).each do |cpu, scheme|
      o[:benchmarks].product(o[:otherbench]).each_slice(o[:threads]) do |i|
        threads=[]
        i.each do |p0,other|
          threads << Thread.new { f << submit.call(cpu, scheme, o, p0, other).flatten }
        end
        threads.each { |t| t.join }
      end
    end
    puts f.flatten.to_s.red
end

def parallel_local opts={}
  iterate_and_submit opts
end

def qsub_fast opts={}
  iterate_and_submit opts.merge(runmode: :qsub)
end

def single opts={}
    o = {
        cpus: %w[detailed],
        schemes: $schemes,
        benchmarks: $specint,
        runmode: :local,
        threads: 4
    }.merge opts

    f = []

    o[:cpus].product(o[:schemes], o[:benchmarks]).each_slice(o[:threads]) do |i|
      t={}
      i.each do |cpu,scheme,p0|
        t[i] = Thread.new do
          r=sav_script(cpu, scheme, p0, o)
          f << r[1] unless r[0]
        end
      end
      t.each { |_,v| v.join }
    end
    puts f.flatten.to_s.red
end

def single_qsub opts={}
  single opts.merge(runmode: :qsub)
end

def parallel_local_scaling opts={}
  opts = {otherbench: %w[astar]}.merge opts
  iterate_and_submit(opts) do |cpu, scheme, param, p0, other|
    f = []
    #2
    p = param.merge(p1: other)
    r = sav_script(cpu, scheme, p0, p)
    f << r[1] unless r[0]
    #3
    p = p.merge(p2: other)
    r = sav_script(cpu, scheme, p0, p)
    f << r[1] unless r[0]
    #4
    p = p.merge(p3: other)
    r = sav_script(cpu, scheme, p0, p)
    f << r[1] unless r[0]
    f
  end
end

def qsub_scaling opts = {}
  parallel_local_scaling opts.merge(runmode: :qsub)
end

end
