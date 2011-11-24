/* input.c -- functions to parse input into commands, arguments and special symbols  */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"
#include "resizable.h"
#include "input.h"

static const char * SPECIAL_SYMBOLS = "|><&";
static const char * SPECIAL_TOKENS[] = {"|", "<", ">", "&"};
static const int SPECIAL_TOKENS_COUNT = 4;

static bool is_special_symbol(char c)
{
  const char * ptr = strchr(SPECIAL_SYMBOLS, c);
  return ptr != NULL;
}

static bool is_special_token(char * str)
{
  for (int i = 0; i < SPECIAL_TOKENS_COUNT; ++i)
    if (strcmp(str, SPECIAL_TOKENS[i]) == 0)
      return true;
  return false;
}

enum parsing_state {
  NORMAL_TEXT,
  ESCAPE_SYMBOL,
  QUOTED_TEXT,
  ESCAPE_IN_QUOTED_TEXT,
};

/* Parses input into a list of arguments and special symbols. 
   Every argument is a null-terminated string. The list is terminated by NULL pointer.
   Memory for the list and for each argument is allocated using malloc. 
   The caller should free this memory. */
char ** parse_input()
{
  rsz_str_array tokens = {
    .array = NULL
  };
  rsz_string current_string = {
    .str = NULL
  };
  enum parsing_state state = NORMAL_TEXT;
  char symbol;
  bool end_of_input = false;

  while (!end_of_input) {
    symbol = getchar();

    switch (state) {
    case NORMAL_TEXT:

      if ('\"' == symbol) {
        state = QUOTED_TEXT;
        break;
      }
      if ('\\' == symbol) {
        state = ESCAPE_SYMBOL;
        break;
      }
      if (isspace(symbol) || is_special_symbol(symbol)) {

        if (current_string.str != NULL) {
          rsz_string_add(&current_string, '\0');
          rsz_str_array_add(&tokens, current_string.str);
          current_string.str = NULL;
        }

        if (is_special_symbol(symbol)) {
          rsz_string_add(&current_string, symbol);
          rsz_string_add(&current_string, '\0');
          rsz_str_array_add(&tokens, current_string.str);
          current_string.str = NULL;
        }

        if ('\n' == symbol) {
          end_of_input = true;
        }

      } else {
          rsz_string_add(&current_string, symbol);
      }
      break;

    case ESCAPE_SYMBOL:
      if ('\n' != symbol) {
        rsz_string_add(&current_string, symbol);
      }
      state = NORMAL_TEXT;
      break;

    case QUOTED_TEXT:
      if ('\"' == symbol) {
        state = NORMAL_TEXT;
      } else if ('\\' == symbol) {
        state = ESCAPE_IN_QUOTED_TEXT;
      } else {
        rsz_string_add(&current_string, symbol);
      }
      break;

    case ESCAPE_IN_QUOTED_TEXT:
      if ('\n' != symbol) {
        rsz_string_add(&current_string, symbol);
      }
      state = QUOTED_TEXT;
      break;
    }
  }

  if (NULL != current_string.str) {
    rsz_string_add(&current_string, '\0');
    rsz_str_array_add(&tokens, current_string.str);
  }

  rsz_str_array_add(&tokens, NULL);
  return tokens.array;
}

enum syntax_state {
  NORMAL_TOKEN,
  SPECIAL_TOKEN
};

/* Returns -1 if syntax of arguments is correct,
   otherwise returns index of the argument near which syntax is incorrect */
int check_syntax(char ** args)
{
  if (NULL == args) 
    return -1;

  int index = 0;
  enum syntax_state state = SPECIAL_TOKEN;
  char ** argument = args;

  while (NULL != *argument) {
    switch (state) {
    case SPECIAL_TOKEN:
      if (is_special_token(*argument))
        return index;
      else 
        state = NORMAL_TOKEN;
      break;

    case NORMAL_TOKEN:
      if (is_special_token(*argument))
        state = SPECIAL_TOKEN;
      break;
    }

    ++index;
    ++argument;
  }

  if (state == SPECIAL_TOKEN) {
    --argument;
    if (strcmp("&", *argument) != 0) 
      return index - 1;
  }

  return -1;
}

/* Creates from array of arguments array of commands.
   Returns NULL if the sequence of arguments is incorrect or an internal error occured */
command * parse_to_commands(char ** args, int * commands_count)
{
  if (NULL == *args)
    return NULL;

  if (check_syntax(args) >= 0)
    return NULL;

  *commands_count = 1;
  char ** argument = args;
  while (NULL != *argument) {
    if (strcmp(*argument, "|") == 0) 
      ++ *commands_count;
    ++argument;
  }

  command * commands = (command *) malloc(sizeof(command) * *commands_count);
  if (NULL == commands) 
    return NULL;

  argument = args;
  int i = 0;
  while (NULL != *argument) {
    commands[i].path = string_copy(*argument);
    ++argument;
    commands[i].args = NULL;
    commands[i].infile = commands[i].outfile = NULL;
    rsz_str_array args = {
      .array = NULL
    };

    while (NULL != *argument && strcmp(*argument, "|")) {
      if (strcmp(*argument, "<") == 0) {
        ++argument;
        commands[i].infile = string_copy(*argument);

      } else if (strcmp(*argument, ">") == 0) {
        ++argument;
        commands[i].outfile = string_copy(*argument);

      } else {
        rsz_str_array_add(&args, string_copy(*argument));

      }
      ++argument;
    }
    if (NULL != *argument) 
      ++argument;

    rsz_str_array_add(&args, NULL);
    commands[i].args = args.array;
    ++i;
  }
  return commands;
}

char * string_copy(const char * str) 
{
  if (NULL == str)
    return NULL;

  int len = strlen(str);
  char * res = (char *) malloc(sizeof(char) * (len + 1));
  if (NULL != res) {
    strcpy(res, str);
  }

  return res;
}
