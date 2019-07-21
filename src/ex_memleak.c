#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {
  char * a;
  if (argc == 1) {
    a = malloc(100 * sizeof(char));
  } else {
    a = "";
  }
  printf("%s\n", a);
  // No free() here.
  return 0;
}
