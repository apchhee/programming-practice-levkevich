/* input.c -- functions to parse input into commands, arguments and special symbols  */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "resizable.h"

static const char * special_symbols = "|><&";

static bool is_special(char c)
{
  const char * ptr = strchr(special_symbols, c);
  return ptr != NULL;
}

enum states {
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
  enum states state = NORMAL_TEXT;
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
      if (isspace(symbol) || is_special(symbol)) {

        if (current_string.str != NULL) {
          rsz_string_add(&current_string, '\0');
          rsz_str_array_add(&tokens, current_string.str);
          current_string.str = NULL;
        }

        if (is_special(symbol)) {
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
