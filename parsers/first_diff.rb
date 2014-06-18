#!/bin/ruby


traceregex = /0x(\w*)\s*@ (\d*)\s*\(([RW])\)\s*\[(\w*)\s*\]\s*(\w*)/

if __FILE__ == $0

    # Lazy enumerators that get (upon request only) the next line of files 
    # specified from the command.
    f1 = File.open( ARGV[0].to_s )
    f2 = File.open( ARGV[1].to_s )
    file1enum = f1.each_line.lazy
    file2enum = f2.each_line.lazy

    # Co-iterate through the files line by line
    loop do
        l1 = file1enum.next
        l2 = file2enum.next

        ist0 = ->(x){ x.match(traceregex)[2].to_i==0 }
        next if ist0.call( l1 ) || ist0.call( l2 )

        # lambdas that take a trace line and get its address or time
        addr = ->(x){ x.match(traceregex)[1] }
        time = ->(x){ x.match(traceregex)[2] }

        # lambda that takes a lambda, applies lines from each file to it,
        # and exits with a print if they don't match
        compare = ->(x){
            ( puts "#{l1}!=\n#{l2} "; true ) if x.call( l1 ) != x.call( l2 )
        }

        break if compare.call( time )
        break if compare.call( addr )
    end

    f1.close
    f2.close

end
