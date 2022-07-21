# Entrega 2

## Instrucciones de uso

```shell
$ make
$ .mmu <paginas> <frames> <archivo de secuencias>
```

### Ejemplo

```shell
$ make
gcc -c mmu_go.c
gcc -o mmu mmu_go.c

$ ./mmu 5 5 cases/ex3.txt
Virtual pages: 5
Frames: 5
Reading from file: cases/ex3.txt
Page V U D Frame
0    0 1 0 -1
1    0 1 1 -1
2    1 1 0 2
3    0 1 1 -1
4    0 1 0 -1
Total page faults: 5
```
