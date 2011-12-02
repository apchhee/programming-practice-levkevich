/* command.c -- function to work with commands */

#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "command.h"

static int execute_recursively(command * cmds, int count)
{
  if (count > 1) {
    int pipefd[2];

    if (pipe(pipefd))
      return -1;

    fcntl(pipefd[0], F_SETFD, FD_CLOEXEC);
    fcntl(pipefd[1], F_SETFD, FD_CLOEXEC);

    int childpid = fork();
    if (childpid != 0) {

      if (childpid < 0) {
        return -1;
      }
      dup2(pipefd[0], 0);
      close(pipefd[1]);
      wait(NULL);

    } else {
      close(pipefd[0]);
      dup2(pipefd[1], 1);
      execute_recursively(cmds, count - 1);
      return -1;
    }
  }

  if (NULL != cmds[count - 1].infile) {
    int fd = open(cmds[count - 1].infile, O_RDONLY);
    if (fd < 0)
      return -1;
    dup2(fd, 0);
    fcntl(fd, F_SETFD, FD_CLOEXEC);
  }

  if (NULL != cmds[count - 1].outfile) {
    int fd = creat(cmds[count - 1].outfile, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    if (fd < 0)
      return -1;
    dup2(fd, 1);
    fcntl(fd, F_SETFD, FD_CLOEXEC);
  }

  execvp(cmds[count - 1].args[0], cmds[count - 1].args);
  return -1;
}

int execute_commands(command * cmds, int count)
{
  int childpid = fork();
  if (childpid != 0) {
    if (childpid < 0) {
      return -1;
    }
    wait(NULL); 
  } else {
    execute_recursively(cmds, count);
    return -1;
  }

  return 0;
}

void free_command(command cmd) 
{
  char ** args = cmd.args;
  if (NULL != args) {
      while (NULL != *args) {
        free(*args);
        ++args;
      }
      free(cmd.args);
  }
  free(cmd.infile);
  free(cmd.outfile);
}
