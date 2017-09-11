/*
 * Created by Sotirios-Efstathios Maneas on 2017-01-02.
 */

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>


#include "tree.h"
#include "utils.h"

int main(void) {

  // char array to hold a line of input
  char buf[BUFFER_SIZE] = {
    '\0'
  };

  // char* array to hold the pointers to tokens
  char * args[INPUT_ARG_MAX_NUM];

  // the root of the tree
  struct TreeNode root;
  root.value = "";
  root.child = NULL;
  root.sibling = NULL;

  struct TreeNode * root_ptr = & root;

  char * token;
  int num_of_args;
  char *p_error;

  while ((p_error = fgets(buf, BUFFER_SIZE, stdin)) != NULL) {
    num_of_args = 0;
    char buf1[BUFFER_SIZE] = {
      '\0'
    };
    strcpy(buf1, buf);
    /* get the first token */
    if (buf[0] == '\n') {
      fprintf(stderr, "Invalid command.\n"); 
    } else {
      token = strtok(buf, DELIMITERS);
      if (strcmp(token, "i") == 0) {
        num_of_args = tokenize(buf1, args);
        if (num_of_args != 4) {
          fprintf(stderr, "Invalid command.\n");
        } else {
          tree_insert(root_ptr, args);
        }
      } else if (strcmp(token, "q") == 0) {
        num_of_args = tokenize(buf1, args);
        if (num_of_args != 3) {
          fprintf(stderr, "Invalid command.\n");
        } else {
          tree_search(root_ptr, args);
        }
      } else if (strcmp(token, "p") == 0) {
        num_of_args = tokenize(buf1, args);
        if (num_of_args != 1) {
          fprintf(stderr, "Invalid command.\n");
        } else {
          tree_print(root_ptr);
        }
      } else {
        fprintf(stderr, "Invalid command.\n");
      }
    }
  }

  if(p_error == NULL){
	  //fprintf("error in fgets, possibly input too long or end of file reached");
	  exit(1);
  }

  return 0;
}