#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int    picoshell(char *cmds[])
{
    int i = 0;
    int last_fd = 0;
    int fd[2];
    pid_t pid;

    while (cmds)
    {
        if (cmds[i + 1])
        {
            if (pipe(fd) < 0)
                return (1);
        }
        else
        {
            fd[0] == -1;
            fd[1] == -1;
        }
        pid = fork();
        if (pid < 0)
        {
            if (last_fd != 0)
                close(last_fd);
            if (fd[1] != -1)
                close (fd[1]);
            if (fd[0] != -1)
                close (fd[0]);
            return (1);
        }
        if (pid == 0)
        {
            if (last_fd != 0)
            {
                if (dup2(last_fd, 0) < 0)
                {
                    close(fd[0]);
                    close(fd[1]);
                    exit (1);
                }
                close(last_fd);
            }
            if (fd[1] != -1)
            {
                if (dup2(fd[1], 1) < 0)
                {
                    close(fd[0]);
                    close(fd[1]);
                    exit (1);
                }
                close(fd[0]);
                close(fd[1]);
            }
            execvp(cmds[i][0], cmds[i]);
            exit (1);
        }
        else
        {
            if (last_fd != 0)
                close (last_fd);
            if (fd[1] != -1)
                close (fd[1]);
            last_fd = fd[0];
            i++;
        }
    }
    while (wait(NULL) < 0)
    return (0);
}