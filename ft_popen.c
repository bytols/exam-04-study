/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_popen.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erocha-l <erocha-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 10:33:32 by erocha-l          #+#    #+#             */
/*   Updated: 2025/10/19 15:03:26 by erocha-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>



int    ft_popen(const char *file, char *const argv[], char type)
{
    int fd[2];
    pid_t pid;
    
    if (!file || !argv || (type != 'r' && type != 'w'))
        return (-1);
    if (pipe(fd) < 0)
        return (-1);
    pid = fork();
    if (pid < 0)
    {
        close(fd[0]);
        close(fd[1]);
        return (-1);
    }
    if (pid == 0)
    {
        if (type == 'w')
        {
            if (dup2(fd[0], STDOUT_FILENO) < 0)
            {
                close(fd[0]);
                close(fd[1]);
                exit (-1);
            }
            close(fd[1]);
            close(fd[0]);
            execvp(file, argv);
            exit (-1);
        }
        if (type == 'r')
        {
            if (dup2(fd[1], STDOUT_FILENO) < 0)
            {
                close(fd[0]);
                close(fd[1]);
                exit (-1);
            }
            close(fd[0]);
            close(fd[1]);
            execvp(file, argv);
            exit (-1);
        }
    }
    if (type == 'r')
    {
        close(fd[0]);
        return(fd[1]);
    }
    else
    {
        close(fd[1]);
        return(fd[0]);
    }
}


int main(int argc, char *argv[], char *envp[])
{
    char    *file;
    int     fd;
    
    (void) argc;
    file = argv[1];
    printf("aqui está file %s\n" , file);
    fd = ft_popen(file, envp, 'w');
    printf("aqui o que voltou %d\n", fd);
    return (0);
}