#include <stdio.h>
#include "CAT.h"

int main (int argc, char *argv[]){
  CATData d1;
  CATData d2;
  CATData d3;
  d1  = CAT_new(5);
  d2  = CAT_new(8);
  d3  = CAT_new(0);
  CAT_add(d3, d1, d2);
  CAT_set(d1, 3);
  int64_t v = CAT_get(d3);
  CAT_sub(d3, d1, d2);
  printf("CAT invocations = %ld\n", CAT_invocations());
  return v == 3 ? 1 : 0 ;
  return CAT_get(d3);
}
