/*
 * Created by Sotirios-Efstathios Maneas on 2017-01-02.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"


/**
 *  A helper function that allocates a new tree node.
 *
 *  @param value The entry's value. It represents either an attribute or a filename.
 *  @return A pointer to the newly allocated node.
 *  (for now it returns NULL, so the compiler does not complain)
 */
struct TreeNode * allocate_node(const char * value) {
  // allocate memory for tree node
  struct TreeNode * retTreeNode = malloc(sizeof(struct TreeNode));
  if (retTreeNode == NULL){
	  perror("Malloc\n");
	  exit(1);
  }
  // allocate memory for str value
  ( * retTreeNode).value = malloc(sizeof(char) * BUFFER_SIZE);
  if (( * retTreeNode).value == NULL){
	  perror("Malloc\n");
	  exit(1);
  }
  // assign tree node's value as value given 
  strcpy(( * retTreeNode).value, value);
  // default sibling an child to null
  ( * retTreeNode).child = NULL;
  ( * retTreeNode).sibling = NULL;
  return retTreeNode;
}

/**
 *  Insert a new image to a tree
 *
 *  @param tree A pointer to the root of the tree.
 *  @param values An array, whose first three members are the attribute values for 
 *                the image and the last one is the filename
 */
void tree_insert(struct TreeNode * root, char * * values) {
  struct TreeNode * file_name = allocate_node( * (values + 3));
  struct TreeNode * param3 = allocate_node( * (values + 2));
  struct TreeNode * param2 = allocate_node( * (values + 1));
  struct TreeNode * param1 = allocate_node( * values);
  if (( * root).child != NULL) {
    struct TreeNode * first_parameter = ( * root).child;
    if (strcmp( * values, ( * first_parameter).value) > 0) {
      while (( * first_parameter).sibling != NULL && strcmp( * values, ( * ( * first_parameter).sibling).value) > 0) {
        first_parameter = ( * first_parameter).sibling;
      }
      if (( * first_parameter).sibling == NULL) {

        ( * first_parameter).sibling = param1;
        ( * param1).child = param2;
        ( * param2).child = param3;
        ( * param3).child = file_name;
      } else if (strcmp( * values, ( * ( * first_parameter).sibling).value) == 0) {
        first_parameter = ( * first_parameter).sibling;
      } else if (strcmp( * values, ( * ( * first_parameter).sibling).value) < 0) {
        ( * param1).sibling = ( * first_parameter).sibling;
        ( * first_parameter).sibling = param1;
        ( * param1).child = param2;
        ( * param2).child = param3;
        ( * param3).child = file_name;
      }
    } else if (strcmp( * values, ( * first_parameter).value) < 0) {
      ( * root).child = param1;
      ( * param1).sibling = first_parameter;
      ( * param1).child = param2;
      ( * param2).child = param3;
      ( * param3).child = file_name;
    }
    if (strcmp( * values, ( * first_parameter).value) == 0) {
      struct TreeNode * second_parameter = ( * first_parameter).child;
      if (strcmp( * (values + 1), ( * second_parameter).value) > 0) {
        while (( * second_parameter).sibling != NULL && strcmp( * (values + 1), ( * ( * second_parameter).sibling).value) > 0) {
          second_parameter = ( * second_parameter).sibling;
        }
        if (( * second_parameter).sibling == NULL) {

          ( * second_parameter).sibling = param2;
          ( * param2).child = param3;
          ( * param3).child = file_name;

        } else if (strcmp( * (values + 1), ( * ( * second_parameter).sibling).value) == 0) {
          second_parameter = ( * second_parameter).sibling;
        } else if (strcmp( * (values + 1), ( * ( * second_parameter).sibling).value) < 0) {
          ( * param2).sibling = ( * second_parameter).sibling;
          ( * second_parameter).sibling = param2;
          ( * param2).child = param3;
          ( * param3).child = file_name;
        }
      } else if (strcmp( * (values + 1), ( * second_parameter).value) < 0) {
        ( * first_parameter).child = param2;
        ( * param2).sibling = second_parameter;
        ( * param2).child = param3;
        ( * param3).child = file_name;
      }
      if (strcmp( * (values + 1), ( * second_parameter).value) == 0) {
        struct TreeNode * third_parameter = ( * second_parameter).child;
        if (strcmp( * (values + 2), ( * third_parameter).value) > 0) {
          while (( * third_parameter).sibling != NULL && strcmp( * (values + 2), ( * ( * third_parameter).sibling).value) > 0) {
            third_parameter = ( * third_parameter).sibling;
          }
          if (( * third_parameter).sibling == NULL) {

            ( * third_parameter).sibling = param3;
            ( * param3).child = file_name;

          } else if (strcmp( * (values + 2), ( * ( * third_parameter).sibling).value) == 0) {
            third_parameter = ( * third_parameter).sibling;
          } else if (strcmp( * (values + 2), ( * ( * third_parameter).sibling).value) < 0) {
            ( * param3).sibling = ( * third_parameter).sibling;
            ( * third_parameter).sibling = param3;
            ( * param3).child = file_name;
          }
        } else if (strcmp( * (values + 2), ( * third_parameter).value) < 0) {
          ( * second_parameter).child = param3;
          ( * param3).sibling = third_parameter;
          ( * param3).child = file_name;
        }
        if (strcmp( * (values + 2), ( * third_parameter).value) == 0) {
          struct TreeNode( * last_parameter) = ( * third_parameter).child;
          if (strcmp( * (values + 3), ( * last_parameter).value) < 0) {
            ( * third_parameter).child = file_name;
            ( * file_name).sibling = last_parameter;
          } else {
            while (( * last_parameter).sibling != NULL && strcmp( * (values + 3), ( * ( * last_parameter).sibling).value) > 0) {
              last_parameter = ( * last_parameter).sibling;
            }
            if (( * last_parameter).sibling != NULL) {
              ( * file_name).sibling = ( * last_parameter).sibling;
              ( * last_parameter).sibling = file_name;
            } else {
              ( * last_parameter).sibling = file_name;
            }
          }
        }
      }
    }
  } else {
    ( * root).child = param1;
    ( * param1).child = param2;
    ( * param2).child = param3;
    ( * param3).child = file_name;
  }
}

/**
 *  Searches a tree to print all files with matching attribute values.
 *
 *  @param tree A pointer to the root of the tree.
 *  @param values An array of attribute values
 */
void tree_search(const struct TreeNode * root, char * * values) {
  if (( * root).child != NULL) {
    struct TreeNode * first_parameter = ( * root).child;
    if (strcmp( * values, ( * first_parameter).value) > 0) {
      while (( * first_parameter).sibling != NULL && strcmp( * values, ( * first_parameter).value) != 0) {
        first_parameter = ( * first_parameter).sibling;
      }
      if (strcmp( * values, ( * first_parameter).value) != 0 && ( * first_parameter).sibling == NULL) {
        printf("(NULL)\n");
      }
    } else if (strcmp( * values, ( * first_parameter).value) < 0) {
      printf("(NULL)\n");
    }
    if (strcmp( * values, ( * first_parameter).value) == 0) {
      struct TreeNode * second_parameter = ( * first_parameter).child;
      if (strcmp( * (values + 1), ( * second_parameter).value) > 0) {
        while (( * second_parameter).sibling != NULL && strcmp( * (values + 1), ( * second_parameter).value) != 0) {
          second_parameter = ( * second_parameter).sibling;
        }
        if (strcmp( * (values + 1), ( * second_parameter).value) != 0 && ( * second_parameter).sibling == NULL) {
          printf("(NULL)\n");
        }
      } else if (strcmp( * (values + 1), ( * second_parameter).value) < 0) {
        printf("(NULL)\n");
      }
      if (strcmp( * (values + 1), ( * second_parameter).value) == 0) {
        struct TreeNode * third_parameter = ( * second_parameter).child;
        if (strcmp( * (values + 2), ( * third_parameter).value) > 0) {
          while (( * third_parameter).sibling != NULL && strcmp( * (values + 2), ( * third_parameter).value) != 0) {
            third_parameter = ( * third_parameter).sibling;
          }
          if (strcmp( * (values + 2), ( * third_parameter).value) != 0 && ( * third_parameter).sibling == NULL) {
            printf("(NULL)\n");
          }
        } else if (strcmp( * (values + 2), ( * third_parameter).value) < 0) {
          printf("(NULL)\n");
        }
        if (strcmp( * (values + 2), ( * third_parameter).value) == 0) {
          struct TreeNode( * last_parameter) = ( * third_parameter).child;
          while (( * last_parameter).sibling != NULL) {
            printf("%s ", ( * last_parameter).value);
            last_parameter = ( * last_parameter).sibling;
          }
          printf("%s \n", ( * last_parameter).value);
        }
      }
    }
  } else {
    printf("(NULL)\n");
  }
}

/**
 *  Prints a complete tree to the standard output.
 *
 *  @param tree A pointer to the root of the tree.
 */
void tree_print(const struct TreeNode * tree) {
  if (( * tree).child != NULL) {
    struct TreeNode * first_parameter = ( * tree).child;
    char * att1_value = ( * first_parameter).value;
    struct TreeNode * second_parameter = ( * first_parameter).child;
    char * att2_value = ( * second_parameter).value;
    struct TreeNode * third_parameter = ( * second_parameter).child;
    char * att3_value = ( * third_parameter).value;
    struct TreeNode * last_parameter = ( * third_parameter).child;
    char * filename = ( * last_parameter).value;
    printf("%s %s %s %s \n", att1_value, att2_value, att3_value, filename);
    while (( * last_parameter).sibling != NULL) {
      last_parameter = ( * last_parameter).sibling;
      filename = ( * last_parameter).value;
      printf("%s %s %s %s \n", att1_value, att2_value, att3_value, filename);
    }
    while (( * third_parameter).sibling != NULL) {
      third_parameter = ( * third_parameter).sibling;
      att3_value = ( * third_parameter).value;
      last_parameter = ( * third_parameter).child;
      filename = ( * last_parameter).value;
      printf("%s %s %s %s \n", att1_value, att2_value, att3_value, filename);
      while (( * last_parameter).sibling != NULL) {
        last_parameter = ( * last_parameter).sibling;
        filename = ( * last_parameter).value;
        printf("%s %s %s %s \n", att1_value, att2_value, att3_value, filename);
      }
    }
    while (( * second_parameter).sibling != NULL) {
      second_parameter = ( * second_parameter).sibling;
      att2_value = ( * second_parameter).value;
      third_parameter = ( * second_parameter).child;
      att3_value = ( * third_parameter).value;
      last_parameter = ( * third_parameter).child;
      filename = ( * last_parameter).value;
      printf("%s %s %s %s \n", att1_value, att2_value, att3_value, filename);
      while (( * last_parameter).sibling != NULL) {
        last_parameter = ( * last_parameter).sibling;
        filename = ( * last_parameter).value;
        printf("%s %s %s %s \n", att1_value, att2_value, att3_value, filename);
      }
      while (( * third_parameter).sibling != NULL) {
        third_parameter = ( * third_parameter).sibling;
        att3_value = ( * third_parameter).value;
        last_parameter = ( * third_parameter).child;
        filename = ( * last_parameter).value;
        printf("%s %s %s %s \n", att1_value, att2_value, att3_value, filename);
        while (( * last_parameter).sibling != NULL) {
          last_parameter = ( * last_parameter).sibling;
          filename = ( * last_parameter).value;
          printf("%s %s %s %s \n", att1_value, att2_value, att3_value, filename);
        }
      }
    }
    while (( * first_parameter).sibling != NULL) {
      first_parameter = ( * first_parameter).sibling;
      att1_value = ( * first_parameter).value;
      second_parameter = ( * first_parameter).child;
      att2_value = ( * second_parameter).value;
      third_parameter = ( * second_parameter).child;
      att3_value = ( * third_parameter).value;
      last_parameter = ( * third_parameter).child;
      filename = ( * last_parameter).value;
      printf("%s %s %s %s \n", att1_value, att2_value, att3_value, filename);
      while (( * last_parameter).sibling != NULL) {
        last_parameter = ( * last_parameter).sibling;
        filename = ( * last_parameter).value;
        printf("%s %s %s %s \n", att1_value, att2_value, att3_value, filename);
      }
      while (( * third_parameter).sibling != NULL) {
        third_parameter = ( * third_parameter).sibling;
        att3_value = ( * third_parameter).value;
        last_parameter = ( * third_parameter).child;
        filename = ( * last_parameter).value;
        printf("%s %s %s %s \n", att1_value, att2_value, att3_value, filename);
        while (( * last_parameter).sibling != NULL) {
          last_parameter = ( * last_parameter).sibling;
          filename = ( * last_parameter).value;
          printf("%s %s %s %s \n", att1_value, att2_value, att3_value, filename);
        }
      }
      while (( * second_parameter).sibling != NULL) {
        second_parameter = ( * second_parameter).sibling;
        att2_value = ( * second_parameter).value;
        third_parameter = ( * second_parameter).child;
        att3_value = ( * third_parameter).value;
        last_parameter = ( * third_parameter).child;
        filename = ( * last_parameter).value;
        printf("%s %s %s %s \n", att1_value, att2_value, att3_value, filename);
        while (( * last_parameter).sibling != NULL) {
          last_parameter = ( * last_parameter).sibling;
          filename = ( * last_parameter).value;
          printf("%s %s %s %s \n", att1_value, att2_value, att3_value, filename);
        }
        while (( * third_parameter).sibling != NULL) {
          third_parameter = ( * third_parameter).sibling;
          att3_value = ( * third_parameter).value;
          last_parameter = ( * third_parameter).child;
          filename = ( * last_parameter).value;
          printf("%s %s %s %s \n", att1_value, att2_value, att3_value, filename);
          while (( * last_parameter).sibling != NULL) {
            last_parameter = ( * last_parameter).sibling;
            filename = ( * last_parameter).value;
            printf("%s %s %s %s \n", att1_value, att2_value, att3_value, filename);
          }
        }
      }
    }
  } else {
    printf("(NULL)\n");
  }
}