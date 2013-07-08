TARGET=attacker_L
CCFLAGS=-O2 -DUMC -std=c99

${TARGET}: ${TARGET}.c
	arm-linux-gnueabi-gcc -DUNIX ${CCFLAGS} ${TARGET}.c -o ${TARGET} --static
	arm-linux-gnueabi-objdump -dS ${TARGET} > ${TARGET}.obj

clean:
	rm -f *.arm *.obj

