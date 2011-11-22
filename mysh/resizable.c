/* resizable.c -- functions for resizable data structures from resizable.h */

#include <stdlib.h>
#include <string.h>

#include "resizable.h"

int rsz_string_add(rsz_string * string, char c)
{
  if (NULL == string->str) {

    string->str = (char *) malloc(sizeof(char) * RESIZABLE_INIT_SIZE);
    if (NULL == string->str) 
      return -1;
    string->size = RESIZABLE_INIT_SIZE;
    string->used = 0;    

  } else if (string->used == string->size) {

    char * newstr = (char *) malloc(sizeof(char) * string->size * RESIZABLE_GROW);
    if (NULL == newstr)
      return -1;
    memcpy(newstr, string->str, sizeof(char) * string->used);
    free(string->str);
    string->str = newstr;
    string->size *= RESIZABLE_GROW;

  }
  string->str[string->used++] = c;
  return 0;
}

int rsz_str_array_add(rsz_str_array * arr, char * string)
{
  if (NULL == arr->array) {

    arr->array = (char **) malloc(sizeof(char *) * RESIZABLE_INIT_SIZE);
    if (NULL == arr->array)
      return -1;
    arr->size = RESIZABLE_INIT_SIZE;
    arr->used = 0;

  } else if (arr->used == arr->size) {

    char ** newarr = (char **) malloc (sizeof(char *) * arr->size * RESIZABLE_GROW);
    if (NULL == newarr)
      return -1;
    memcpy(newarr, arr->array, sizeof(char *) * arr->used);
    free(arr->array);
    arr->array = newarr;
    arr->size *= RESIZABLE_GROW;

  }
  arr->array[arr->used++] = string;
  return 0;
}

