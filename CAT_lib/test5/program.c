#include <CAT.h>

int main (int argc, char *argv[]){
  CATData d = CAT_new(5);
  CAT_add(d, d, d);
  CAT_sub(d, d, d);
  CAT_set(d, 42);
  CAT_get(d);
  printf("CAT invocations = %ld\n", CAT_invocations());
	return 0;
}
