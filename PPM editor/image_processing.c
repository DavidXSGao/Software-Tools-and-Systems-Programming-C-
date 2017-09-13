#include <stdio.h>

  /*
   * Read an image from the standard input and set the red value of each pixel to
   * zero.
   */
void print_title();
char title;
int row;
int column;
int largest_int;

void remove_red() {
  print_title();
  int num;
  int counter = 0;
  while (scanf("%d\n", & num) != EOF) {
    if (counter % 3 == 0) {
      num = 0;
    }
    printf("%d ", num);
    counter++;
    if (counter % ((3 * column)) == 0) {
      printf("\n");
    }
  }
}

/*
 * Read an image from the standard input and convert it from colour to black and white.
 */
void convert_to_black_and_white() {
  print_title();
  int num;
  int counter = 3;
  int red;
  int blue;
  int green;

  while (scanf("%d\n", & num) != EOF) {
    if (counter % 3 == 0) {
      red = num;
    } else if (counter % 3 == 1) {
      green = num;
    } else if (counter % 3 == 2) {
      blue = num;
    }
    int avg = (red + blue + green) / 3;
    if ((counter != 0) && (counter % 3 == 2)) {
      printf("%d %d %d ", avg, avg, avg);
	  printf("\n");
    }
    counter++;
  }
}

/*
 * Read an image from the standard input and convert it to a square image.
 */
void instagram_square() {
  scanf("%s", & title);
  scanf("%d", & row);
  scanf("%d", & column);
  int difference = 0;
  int useless;
  if (row > column) {
	difference = row - column;
    row = column;
  } else if (column > row) {
	difference = row - column;
    column = row;
  }
  printf("%s\n", & title);
  printf("%d ", row);
  printf("%d\n", column);
  scanf("%d", & largest_int);
  printf("%d\n", largest_int);
  int extra = difference * 3;
  int num;
  int counter = 0;
  while (scanf("%d\n", & num) != EOF) {
    printf("%d ", num);
    counter++;
    if (counter % ((3 * column)) == 0) {
      if (difference > 0) {
        for (int i = 0; i < extra; i++) {
          scanf("%d\n", &useless);
        }
      }
	  printf("\n");
    }
	if (counter == row * row * 3){
		break;
	}
  }

}

void print_title() {
  scanf("%s", & title);
  printf("%s\n", & title);
  scanf("%d", & row);
  printf("%d ", row);
  scanf("%d", & column);
  printf("%d\n", column);
  scanf("%d", & largest_int);
  printf("%d\n", largest_int);
}