all: victim attacker_L attacker_M attacker_H
CCFLAGS=-O2 -DUMC -std=c99

victim: victim.c
	arm-linux-gnueabi-gcc -DUNIX ${CCFLAGS} victim.c -o victim.arm --static
	arm-linux-gnueabi-objdump -dS victim.arm > victim.obj

attacker_L: attacker_L.c
	arm-linux-gnueabi-gcc -DUNIX ${CCFLAGS} attacker_L.c -o attacker_L.arm --static
	arm-linux-gnueabi-objdump -dS attacker_L.arm > attacker_L.obj

attacker_M: attacker_M.c
	arm-linux-gnueabi-gcc -DUNIX ${CCFLAGS} attacker_M.c -o attacker_M.arm --static
	arm-linux-gnueabi-objdump -dS attacker_M.arm > attacker_M.obj

attacker_H: attacker_H.c
	arm-linux-gnueabi-gcc -DUNIX ${CCFLAGS} attacker_H.c -o attacker_H.arm --static
	arm-linux-gnueabi-objdump -dS attacker_H.arm > attacker_H.obj


clean:
	rm -f *.arm *.obj

