/* cd.c -- the builtins "cd" and "pwd" */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int cd(const char* path) 
{
  if (chdir(path) < 0) {

    switch (errno) {
    case EACCES:
      printf("Permission is denied\n");
      break;
    case EFAULT:
      printf("Path points outside your accessible address space\n");
      break;
    case EIO:
      printf("An I/O error occurred\n");
      break;
    case ENAMETOOLONG:
      printf("Path is too long\n");
      break; 
    case ENOENT:
      printf("No such directory\n");
      break;
    case ENOMEM:
      printf("Insufficient kernel memory was available\n");
      break;
    case ENOTDIR:
      printf("Not a directory\n");
      break;
    default:
      printf("Unknown error\n");
    }
    return 1;

  } 
  return 0;
}

int pwd()
{
  char * cwd = getcwd(NULL, 0);
  if (NULL == cwd) {

    switch (errno) {
    case EACCES:
      printf("Permission is denied\n");
      break;
    case ENOENT:
      printf("The current working directory has been unlinked\n");
      break;
    default:
      printf("Unknown error\n");
    }
    return 1;

  } else {

    printf("%s\n", cwd);
    free(cwd);
    return 0;

  }
} 
