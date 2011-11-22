#include <stdio.h>
#include <stdlib.h>

#include "cd.h"
#include "resizable.h"
#include "input.h"

int main(int argc, char * argv[])
{
  char ** tokens = parse_input();
  char ** token = tokens;
  int i = 0;

  while (NULL != *token) {
    printf("%d) %s\n", ++i, *token);
    free(*token);
    ++token;
  }
  free(tokens);

  exit(0);
}
