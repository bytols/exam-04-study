/* Assignment name:    picoshell
Expected files:        picoshell.c
Allowed functions:    close, fork, wait, exit, execvp, dup2, pipe
___

Write the following function:

int    picoshell(char *cmds[]);

The goal of this function is to execute a pipeline. It must execute each
commands [sic] of cmds and connect the output of one to the input of the
next command (just like a shell).

Cmds contains a null-terminated list of valid commands. Each rows [sic]
of cmds are an argv array directly usable for a call to execvp. The first
arguments [sic] of each command is the command name or path and can be passed
directly as the first argument of execvp.

If any error occur [sic], The function must return 1 (you must of course
close all the open fds before). otherwise the function must wait all child
processes and return 0. You will find in this directory a file main.c which
contain [sic] something to help you test your function.


Examples: 
./picoshell /bin/ls "|" /usr/bin/grep picoshell
picoshell
./picoshell echo 'squalala' "|" cat "|" sed 's/a/b/g'
squblblb/
*/

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int    picoshell(char *cmds[])
{
    int i;
    int fd[2];
    t_pid pid;
    
    i = 0;
    last_fd = 0;
    pid = 0;
    while (cmds[i] != NULL)
    {
        if (cmds[i + 1])
        {
            if (pipe(fd) < 0)
                return (1);
        }
        else
        {
            fd[0] = -1;
            fd[1] = -1;
        }
        pid = fork();
        if (pid < 0)
        { 
            if (last_fd != 0)
                close (last_fd);
            if (fd[0] != -1)
                close (fd[0]);
            if (fd[1] != -1)
                close (fd[1]);
            return (1);
        }
        if (pid == 0)
        {
            if (last_fd != 0)
            {
                if (dup2(last_fd, 0) < 0)
                    exit(1);
                close(last_fd);
            }
            if (fd[1] != -1)
            {
                if (dup2(fd[1], 1) < 0)
                    exit(1);
                close(fd[1]);
                close(fd[0]);
            }
            execvp(cmds[i][0], cmds[i]);
            exit(1);
        }
        else
        {
            if (last_fd != 0)
                close(last_fd);
            last_fd = fd[0];
            close(fd[1]);
            i++;
        }
    }
    while(wait(NULL) > 0);
    return (0);

}

int main(void)
{
	char **cmds[4];

	static char *cmd1[] = {"ls", NULL};
    static char *cmd2[] = {"grep", "l", NULL};
    static char *cmd3[] = {"wc", NULL};//essayer commande inexistante (comme "wololo")

    cmds[0] = cmd1;
    cmds[1] = cmd2;
    cmds[2] = cmd3;
    cmds[3] = NULL;
	
	int res = picoshell(cmds);
	if (res)
		write(1, "Returned 1\n", 11);
	return (0);
} 