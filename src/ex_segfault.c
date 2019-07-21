int main(int argc, char * argv[]) {
  char a;
  char * p;
  if (argc == 1) {
    p = & a;
  } else {
    p = 0;
  }
  *p = 'F';
  return 0;
}