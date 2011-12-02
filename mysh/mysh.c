#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cd.h"
#include "command.h"
#include "resizable.h"
#include "input.h"

void free_arguments(char ** args);

static const char * PROMPT = "> ";

int main(int argc, char * argv[])
{
  char ** arguments;
  printf("%s", PROMPT);

  while ( NULL != (arguments = parse_input()) ) {
    if (strcmp(arguments[0], "exit") == 0) {
      free_arguments(arguments);
      break;

    } if (strcmp(arguments[0], "pwd") == 0) {
      pwd();

    } else if (strcmp(arguments[0], "cd") == 0) {
      cd(arguments[1]);

    } else {
      int incorrect_token = check_syntax(arguments);
      if (incorrect_token >= 0) {
        printf("Syntax error near token \'%s\'\n", arguments[incorrect_token]);

      } else {
        int commands_count;
        command * commands = parse_to_commands(arguments, &commands_count);
        if (execute_commands(commands, commands_count) != 0) {
          perror(NULL);
        }

        for (int i = 0; i < commands_count; ++i)
          free_command(commands[i]);
        free(commands);
      }
 
    }

    printf("%s", PROMPT);
    free_arguments(arguments);
  }

  exit(0);
}

void free_arguments(char ** args)
{
  if (NULL == args)
    return;
  char ** argument = args;
  while (NULL != *argument) {
    free(*argument);
    ++argument;
  }
  free(args);
}
