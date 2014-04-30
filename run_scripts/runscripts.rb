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
        p1: nil,
        p2: nil,
        p3: nil,
        tl0: 6,
        tl1: 6,
        tl2: 6,
        tl3: 6,
        diffperiod: false,
        l3config: "shared",
        runmode: :qsub,
        savetraces: false,
        maxinsts: $maxinsts,
        fastforward: $fastforward
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
    # allows the turn lengths for p0-p3 to differ
    diffperiod = options[:diffperiod]
    l3config   = options[:l3config]
    # runmode can be qsub: to submit jobs, local: to run the test locally, or 
    # none: to generate the scripts without running them
    runmode    = options[:runmode]
    maxinsts   = options[:maxinsts]
    fastforward= options[:fastforward]
    # Determines if trace files for security should be saved
    savetraces = options[:savetraces]
    # Determines l3 trace file output. nil by default to allow short-circuiting
    l3tracefile= options[:l3tracefile]
    

    filename  = "#{scheme}_#{cpu}_#{p0}tl#{tl0}"
    filename += "_#{p1}tl#{tl1}" unless p1.nil?
    filename += "_#{p2}tl#{tl2}" unless p2.nil?
    filename += "_#{p3}tl#{tl3}" unless p3.nil?
    filename += "_c#{cacheSize}MB"
   
    numpids = [p0,p1,p2,p3].inject(0){|sum,i| ( i.nil? && sum ) || sum+1}

    script = File.new($scriptgen_dir.path+"/run_#{filename}","w+")
    script.puts("#!/bin/bash")
    script.puts("build/ARM/gem5.fast \\")
    script.puts("    --stats-file=#{filename}_stats.txt \\")
    script.puts("    configs/dramsim2/dramsim2_se.py \\")
    script.puts("    --cpu-type=#{cpu} \\")
    script.puts("    --caches \\")
    script.puts("    --l2cache \\")
    unless cacheSize == 0
        script.puts("    --l3cache \\")
        script.puts("    --l3_size=#{cacheSize}MB\\")
    end
    script.puts("   --fixaddr\\") if scheme == "fa"
    script.puts("    --fast-forward=#{fastforward} \\") unless fastforward == 0
    script.puts("    --fixaddr\\") if scheme == "fa"
    script.puts("    --maxinsts=#{maxinsts} \\")
    script.puts("    --maxtick=#{$maxtick} \\")
    script.puts("    --dramsim2 \\")
    script.puts("    --savetraces \\") if savetraces
    l3tracefile = l3tracefile || "results/l3trace_#{filename}.txt"
    script.puts("    --l3tracefile #{l3tracefile}\\") if savetraces
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
    script.puts("    > results/stdout_#{filename}.out")
    script_abspath = File.expand_path(script.path)
    script.close
    system "qsub -wd #{$gem5home.path} #{script_abspath}" if runmode == :qsub
    puts "#{filename}".magenta if runmode == :local
    system "sh #{script_abspath}" if runmode == :local

end

end
