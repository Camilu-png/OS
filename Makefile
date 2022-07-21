all: exe

exe: mmu_go.o
	gcc -o mmu mmu_go.c


mmu_go.o: mmu_go.c
	gcc -c mmu_go.c