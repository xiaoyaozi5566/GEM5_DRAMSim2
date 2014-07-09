#!/usr/bin/ruby
require_relative 'runscripts'
include RunScripts

module RunScripts
def rerun
    gem5home = Dir.new(Dir.pwd)
    %w[
        run_l2l3resp_32_tp_detailed_libquantumtl6_astartl6_astartl6_astartl6_c4MB
        run_tp_detailed_libquantumtl0_astartl0_astartl0_astartl0_c4MB
        run_tp_detailed_libquantumtl64_astartl64_astartl64_astartl64_c4MB
        run_tp_detailed_libquantumtl96_astartl96_astartl96_astartl96_c4MB
        run_tp_detailed_libquantumtl128_astartl128_astartl128_astartl128_c4MB
        run_tp_detailed_libquantumtl192_astartl192_astartl192_c4MB
        run_tp_detailed_libquantumtl192_astartl192_astartl192_astartl192_c4MB
        run_tp_detailed_libquantumtl256_astartl256_astartl256_c4MB
        run_tp_detailed_libquantumtl256_astartl256_astartl256_astartl256_c4MB
        run_fa_detailed_libquantumtl0_astartl0_astartl0_astartl0_c4MB
        run_fa_detailed_mcftl64_astartl64_astartl64_astartl64_c4MB
        run_fa_detailed_libquantumtl64_astartl64_astartl64_astartl64_c4MB
        run_fa_detailed_gcctl96_astartl96_astartl96_astartl96_c4MB
        run_fa_detailed_mcftl96_astartl96_astartl96_c4MB
        run_fa_detailed_libquantumtl96_astartl96_astartl96_c4MB
        run_fa_detailed_mcftl96_astartl96_astartl96_astartl96_c4MB
        run_fa_detailed_libquantumtl96_astartl96_astartl96_astartl96_c4MB
        run_fa_detailed_libquantumtl128_astartl128_astartl128_c4MB
        run_fa_detailed_mcftl128_astartl128_astartl128_c4MB
        run_fa_detailed_libquantumtl128_astartl128_astartl128_astartl128_c4MB
        run_fa_detailed_mcftl128_astartl128_astartl128_astartl128_c4MB
        run_fa_detailed_xalantl128_astartl128_astartl128_astartl128_c4MB
        run_fa_detailed_bzip2tl192_astartl192_astartl192_astartl192_c4MB
        run_fa_detailed_astartl192_astartl192_astartl192_astartl192_c4MB
        run_fa_detailed_libquantumtl192_astartl192_c4MB
        run_fa_detailed_mcftl192_astartl192_astartl192_c4MB
        run_fa_detailed_libquantumtl192_astartl192_astartl192_c4MB
        run_fa_detailed_mcftl192_astartl192_astartl192_astartl192_c4MB
        run_fa_detailed_libquantumtl192_astartl192_astartl192_astartl192_c4MB
        run_fa_detailed_xalantl192_astartl192_astartl192_astartl192_c4MB
        run_fa_detailed_bzip2tl256_astartl256_astartl256_astartl256_c4MB
        run_fa_detailed_gcctl256_astartl256_astartl256_astartl256_c4MB
        run_fa_detailed_libquantumtl256_astartl256_c4MB
        run_fa_detailed_mcftl256_astartl256_astartl256_c4MB
        run_fa_detailed_libquantumtl256_astartl256_astartl256_c4MB
        run_fa_detailed_hmmertl256_astartl256_astartl256_astartl256_c4MB
        run_fa_detailed_mcftl256_astartl256_astartl256_astartl256_c4MB
        run_fa_detailed_libquantumtl256_astartl256_astartl256_astartl256_c4MB
        run_fa_detailed_xalantl256_astartl256_astartl256_astartl256_c4MB
        run_fa_detailed_sjengtl256_astartl256_astartl256_astartl256_c4MB
    ].each do |experiment|
        File.open(Dir.pwd+"/scriptgen/"+experiment) {|file|
            exp_abspath = File.expand_path file
            system "qsub -wd #{gem5home.path} -e stderr/ -o stdout/ #{exp_abspath}"
        }
    end
end
end
