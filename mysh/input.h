/* input.h -- declarations of the functions in input.c */

#ifndef _INPUT_H_
#define _INPUT_H_

#include "command.h"

char ** parse_input();
int check_syntax(char ** args);
char * string_copy(const char * str);
command * parse_to_commands(char ** args, int * commands_count);

#endif /* _INPUT_H_ */ 
