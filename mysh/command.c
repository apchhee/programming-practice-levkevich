/* command.c -- function to work with commands */

#include <stdlib.h>

#include "command.h"

void free_command(command cmd) 
{
  free(cmd.path);
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
