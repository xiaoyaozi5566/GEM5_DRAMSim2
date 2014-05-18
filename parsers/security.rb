#!/usr/bin/ruby
require 'colored'
require_relative 'parsers'
include Parsers

def compare_etime( conf = {} )
    conf = {
        cpus: $cpus,
        schemes: $schemes,
        bench: $specint,
    }.merge conf
    differing = []
    config = conf[:cpus].product( conf[:schemes], conf[:bench] )
    config.each do |cpu, scheme, p0|
        p = {cpu: cpu, scheme: scheme, p0: p0, dir: conf[:dir] }
        times = conf[:bench].inject({}) do |hsh,p1|
            time, found = findTime( stdo_file( p.merge({p1:p1}) ),
                                   no_ff: true )
            hsh[p1] = time if found
            hsh
        end
        conf[:bench].combination(2) do |p1i,p1j|
            if times[p1i] != times[p1j]
                fi = stdo_file( p.merge( { p1:p1i } ) )
                fj = stdo_file( p.merge( { p1:p1j } ) )
                differing << [fi,fj]
                yield( fi, times[p1i], fj, times[p1j] ) if block_given?
            end
        end
    end
    differing
end

def avg_arr arr
    (arr.length != 0 && arr.inject(:+)/arr.length) || 0
end

def avg_difference conf
    differences = []
    compare_etime( conf ){|fi,t1,fj,t2|
        differences << (t1-t2).abs unless t1.nil? || t2.nil?
    }
    avg_arr differences
end

def percent_diff(t1,t2)
        unless t1.nil? || t2.nil?
            high = ( t1>=t2 && t1 ) || ( true && t2 ) 
            low  = ( t1>=t2 && t2 ) || ( true && t1 )
            (high-low)/((high+low)/2) * 100
        end
end

def avg_percent conf
    diff_percents = []
    compare_etime( conf ){|fi,t1,fj,t2|
            diff = method(:percent_diff).call(t1,t2)
            diff_percents << diff unless diff.nil?
    }
    avg_arr diff_percents.sort.reverse
end

def percent_diff_dist conf
    dist=Hash.new(0)
    tests = [  ->(x){x <= 1 } ]
    (1..11).each{|i| tests << ->(x){ x >= i && x< i+1} }
    tests << ->(x){ x >= 12 }
    big_diff = []
    compare_etime( conf ){|fi,t1,fj,t2|
        diff = method(:percent_diff).call(t1,t2)
        unless diff.nil?
            big_diff << [fi,fj] if tests.map{|l| l.call( diff )}.index(true) == 6
        end
        dist[tests.map{|l| l.call( diff ) }.index(true)]+=1 unless diff.nil?
    }
    [dist,big_diff]
end

def count_differences conf
    diffs = 0
    compare_etime( conf ){|fi,t1,fj,t2| diffs+=1 }
    diffs
end

if __FILE__ == $0
    result_dir = ARGV[0].to_s

    %w[ tp ].product($cpus).each do |scheme,cpu|
        conf = {
            dir: "results_private",
            schemes: [scheme],
            cpus: [cpu],
            bench: $specint - %w[bzip2] }
        differing = compare_etime conf
        f = File.new( result_dir+"/etime_diff_#{scheme}_#{cpu}.out", 'w' )
        differing.each{|f1,f2|f.puts "#{f1} differs from #{f2}"}
         puts "#{cpu} #{scheme} Avg Cycle Difference:   " +
             ("%E" % (avg_difference conf) ).magenta
         puts "#{cpu} #{scheme} Avg Percent Difference: " +
             (avg_percent conf).to_s.magenta
        puts "#{cpu} #{scheme} Number of Differences:  " +
            (count_differences conf).to_s.magenta
        dist, big_diff = percent_diff_dist conf
        puts "#{cpu} #{scheme} distribution:           " +
            dist.to_s.magenta
        puts "#{cpu} #{scheme} big difference files:"
        big_diff.each{|f1,f2| puts "#{f1} != #{f2}"}
        f.close()
    end

end
