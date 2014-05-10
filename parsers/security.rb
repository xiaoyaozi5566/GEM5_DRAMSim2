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
        p = {cpu: cpu, scheme: scheme, p0: p0}
        times = conf[:bench].inject({}) do |hsh,p1|
            time, found = findTime( stdo_file( p.merge({p1:p1}) ) )
            hsh[p1] = time if found
            hsh
        end
        conf[:bench].product( conf[:bench] ).each do |p1i,p1j|
            if times[p1i] != times[p1j]
                fi = stdo_file( p.merge( {p1:p1i } ) )
                fj = stdo_file( p.merge( {p1:p1j } ) )
                differing << [fi,fj]
                yield( times[p1i], times[p1j] ) if block_given?
            end
        end
    end
    differing
end

def avg_difference conf
    differences = []
    compare_etime( conf ){|t1,t2|
        differences << (t1-t2).abs unless t1.nil? || t2.nil?
    }
    differences.inject(:+)/differences.length
end

def avg_percent conf
    diff_percents = []
    compare_etime( conf ){|t1,t2|
        unless t1.nil? || t2.nil?
            high = ( t1>=t2 && t1 ) || ( true && t2 ) 
            low  = ( t1>=t2 && t2 ) || ( true && t1 )
            diff_percents << (high-low)/high * 100
        end
    }
    diff_percents.inject(:+)/diff_percents.length
end

if __FILE__ == $0
    result_dir = ARGV[0].to_s

    %w[ tp ].each do |scheme|
        conf = {
            schemes: [scheme],
            bench: $specint-%w[bzip2] }
        differing = compare_etime conf
        f = File.new( result_dir+"/etime_diff_#{scheme}.out", 'w' )
        differing.each{|f1,f2|f.puts "#{f1} differs from #{f2}"}
        puts "Avg Cycle Difference:   " + ("%E" % (avg_difference conf) ).magenta
        puts "Avg Percent Difference: " + (avg_percent conf).to_s.magenta
        f.close()
    end


end
