/* command.h -- structure to represent commands and declarations of the functions from command.c*/

#ifndef _COMMAND_H_
#define _COMMAND_H_

typedef struct command {
  char ** args;
  char * infile;
  char * outfile;
} command;

int execute_commands(command * cmds, int count);
void free_command(command cmd);

#endif /* _COMMAND_H_ */
