#include "shellcito.h"

int print_arguments(char *arguments[], int argument_count)
{
    int i;
    for (i = 0; i < argument_count; i++)
    {
        printf("%s ", arguments[i]);
    }
    printf("\n");
    return 0;
}

int execute_command(char *command, char *arguments[], int argument_count, int background)
{
    int pid, status;
    pid = fork();
    if (pid == 0)
    {
        if (background == 0)
        {
            if (execvp(command, arguments) == -1)
            {
                perror("Error");
                exit(EXIT_FAILURE);
            };
            exit(EXIT_SUCCESS);
        }
        else
        {
            if (execvp(command, arguments) == -1)
            {
                perror("Error");
                exit(EXIT_FAILURE);
            };
            wait(&status);
            exit(EXIT_SUCCESS);
        }
    }
    else
    {
        if (background == 0)
        {
            wait(&status);
        }
    }
    return 0;
}

int main()
{

    while (1)
    {

        printf("$ ");

        char input[1024];
        fgets(input, 1024, stdin);

        char *tokens[1024];
        int token_count = 0;
        char *token = strtok(input, " ");
        while (token != NULL)
        {
            tokens[token_count] = token;
            token_count++;
            token = strtok(NULL, " ");
        }

        char *command = tokens[0];
        char *arguments[1024];

        int argument_count = 0;
        int background = tokens[token_count - 1][0] == '&';

        for (int i = 0; i < token_count - background; i++)
        {
            arguments[argument_count] = tokens[i];
            argument_count++;
        }

        // Remove newline from last argument
        if (argument_count > 0 && background == 0)
            arguments[argument_count - 1][strlen(arguments[argument_count - 1]) - 1] = '\0';

        // If is a empty command, continue
        if (strcmp(command, "") == 0)
            continue;

        execute_command(command, arguments, argument_count, background);
    }
    return 0;
}