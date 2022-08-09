# Memory Management Unit

> "El objetivo de la tarea es escribir un simulador de una Memory Management Unit para una memoria paginada. El simulador se puede escribir en cualquier lenguaje de programación, pero debe respetar las especificaciones explicadas más adelante. El simulador debe funcionar en un sistema operativo Linux (no desarrollarlo en Windows)."

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

# Explicación

Para realizar la MMU se deben conocer las funciones básicas que se van a implementar.

1. Cada vez que que se lea o escriba en una página se debe cambiar el bit V a 1 en caso de no estarlo
2. Cuando se cambia el bit V a 1, los bits U y D se inicializan en 0
3. Cada vez que se lea o escriba en una paǵina el bit U cambiará a 1
4. Cada vez que se escriba en una página, su bit D cambiará a 1
5. Puede liberar un frame con la función free

Hay que aclarar que frame es diferente a página. Las páginas se alojan en los frames, así que cuando se libera con free(3) se libera la página que esté en el frame 3. No necesariamente la página 3 estará en el frame 3.

## Estructuras de datos
