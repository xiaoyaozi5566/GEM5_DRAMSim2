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
            hsh[p1]=findTime( stdo_file( p.merge({p1:p1}) ) )
            hsh
        end
        conf[:bench].product( conf[:bench] ).each do |p1i,p1j|
            if times[p1i] != times[p1j]
                fi = stdo_file( p.merge( {p1:p1i } ) )
                fj = stdo_file( p.merge( {p1:p1j } ) )
                differing << [fi,fj]
            end
        end
    end
    differing
end

if __FILE__ == $0
    result_dir = ARGV[0].to_s

    %w[ none tp ].each do |scheme|
        differing = compare_etime( {
            schemes: [scheme],
            bench: $specint-%w[bzip2] } )
        f = File.new( result_dir+"/etime_diff_#{scheme}.out", 'w' )
        differing.each{|f1,f2|f.puts "#{f1} differs from #{f2}"}
        f.close()
    end

end
