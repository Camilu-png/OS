#ifndef SHELLCITO_H
#define SHELLCITO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int execute_command(char *command, char *arguments[1024], int argument_count, int background);

#endif