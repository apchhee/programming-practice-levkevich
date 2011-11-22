/* resizable.h -- resizable data structures and declarations of the functions in resizable.c */

#ifndef _RESIZABLE_H_
#define _RESIZABLE_H_

#include <stddef.h>

typedef struct resizable_string {
  size_t size;
  size_t used;
  char * str;
} rsz_string;

#define RESIZABLE_INIT_SIZE 2
#define RESIZABLE_GROW 2

int rsz_string_add(rsz_string * string, char c);

typedef struct resizable_string_array {
  size_t size;
  size_t used;
  char ** array;
} rsz_str_array;

int rsz_str_array_add(rsz_str_array * arr, char * string);

#endif /*_RESIZABLE_H_ */ 
