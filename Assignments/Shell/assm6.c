#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
// #include <sys/wait.h>
#include <string.h>

int tokenizer(char *line, char *tokens[5])
{
    line[strcspn(line, "\n\r")] = 0;
    char str[80], *token;
    int tokenCount = 0;
    strcpy(str, line);
    token = strtok(str, " ");

    while (token != NULL)
    {
        tokens[tokenCount] = (char *)malloc(sizeof(char) * 80);
        strcpy(tokens[tokenCount], token);
        token = strtok(NULL, " ");
        tokenCount++;
    }
    return tokenCount;
}

int main(int argc, char *argv[])
{
    char *tokens[5], cmd[30];
    int tokenCount, pid, e;
    
    do
    {
        printf("\nMyShell$ ");
        fgets(cmd, 30, stdin);
        tokenCount = tokenizer(cmd, tokens);

        switch (tokenCount)
        {
        case 1:
            if(strcmp("q", tokens[0]) == 0)
                return 0;
            pid = fork();
            if (pid == 0)
            {
                e = execlp(tokens[0], tokens[0], (char *)0);
                if (e < 0)
                    printf("Error occured.\n");
            }
            // else
            //     waitpid(NULL);
            break;

        case 2:
            pid = fork();
            if(pid == 0)
            {
                if(pid == 0)
                {
                    e = execlp(tokens[0], tokens[0], tokens[1], (char *)0);
                    if (e < 0)
                        printf("Error occured.\n");
                }
            }
            break;
            

        case 3:
            pid = fork();
            if(pid == 0)
            {
                if(pid == 0)
                {
                    e = execlp(tokens[0], tokens[0], tokens[1], tokens[2], (char *)0);
                    if (e < 0)
                        printf("Error occured.\n");
                }
            }
            break;

        case 4:
            pid = fork();
            if(pid == 0)
            {
                if(pid == 0)
                {
                    e = execlp(tokens[0], tokens[0], tokens[1], tokens[2], tokens[3], (char *)0);
                    if (e < 0)
                        printf("Error occured.\n");
                }
            }
            break;

        default:
            break;
        }

    } while (1);
}
