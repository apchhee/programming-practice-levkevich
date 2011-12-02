/* cd.c -- the builtins "cd" and "pwd" */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int cd(const char* path) 
{
  if (NULL == path) {
    printf("Path is not specified\n");
    return 1;
  }

  if (chdir(path) < 0) {
    perror(NULL);
    return 1;
  } 

  return 0;
}

int pwd()
{
  char * cwd = getcwd(NULL, 0);
  if (NULL == cwd) {
    perror(NULL);
    return 1;

  } else {

    printf("%s\n", cwd);
    free(cwd);
    return 0;

  }
} 
