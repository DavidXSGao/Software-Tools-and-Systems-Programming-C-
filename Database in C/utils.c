/*
 * Created by Sotirios-Efstathios Maneas on 2017-01-02.
 */

#include <stdio.h>
#include <string.h>

#include "utils.h"


/*
 * Tokenize the string stored in cmd based on DELIMITERS as separators.
 * Return the number of tokens, and store pointers to them in cmd_argv.
 */
int tokenize(char * cmd, char * * cmd_argv) {
  int num_of_args = 0;
  char * token = strtok(cmd, DELIMITERS);
  while (token != NULL && num_of_args < 6) {
    if (num_of_args != 0 && num_of_args != 5) {
      cmd_argv[num_of_args - 1] = token;
    }
    num_of_args++;
    token = strtok(NULL, DELIMITERS);
  }
  if (num_of_args == 5) {
    // i
    return 4;
  } else if (num_of_args == 4) {
    // p
    return 3;
  } else if (num_of_args == 1) {
    // q
    return 1;
  } else {
    // invalid
    return -1;
  }
}