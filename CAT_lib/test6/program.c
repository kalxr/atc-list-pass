#include <stdio.h>
#include <CAT.h>

int CAT_execution (void){
  int res;

	printf("H0: I'm going to do some very important computation\n");

  res = 0;
  for (int i=0; i < 10000; i++){
    res++;
  }

	return res;
}

int main (int argc, char *argv[]){
  int r;
	printf("H0: Begin\n");
	r = CAT_execution();
	printf("H0:   Output = %d\n", r);
	printf("H0: End\n");

  CATData d = CAT_new(5);
  CAT_add(d, d, d);
  CAT_sub(d, d, d);
  CAT_set(d, 42);
  CAT_get(d);

  printf("CAT invocations = %ld\n", CAT_invocations());
	return 0;
}
