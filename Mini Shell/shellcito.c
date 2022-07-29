#include "shellcito.h"

int execute_command(char *command, char *arguments[1024], int argument_count, int background)
{
    if (access(command, F_OK) != 0)
    {
        printf("%s command not found\n", command);
        return 1;
    }

    int pid, status;
    pid = fork();
    if (pid == 0)
    {
        if (background == 0)
        {
            execvp(command, arguments);
        }
        else
        {
            execvp(command, arguments);
            wait(&status);
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

        // Remove the newline to the last token
        tokens[token_count - 1][strlen(tokens[token_count - 1]) - 1] = '\0';

        char *command = tokens[0];
        char *arguments[1024];
        arguments[0] = command;
        int argument_count = 1;
        int background = tokens[token_count - 1][0] == '&';
        for (int i = 1; i < token_count - 1 - background; i++)
        {
            arguments[argument_count] = tokens[i];
            argument_count++;
        }

        int state = execute_command(command, arguments, argument_count, background);
    }
    return 0;
}