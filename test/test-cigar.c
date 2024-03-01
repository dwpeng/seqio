#include "cigar.h"
#include <assert.h>

int
main()
{
  cigar_string_t* cigar = seqioCigarInit();
  seqioCigarPush(cigar, CIGAR_D);
  seqioCigarPush(cigar, CIGAR_M);
  seqioCigarPush(cigar, CIGAR_M);
  seqioCigarPush(cigar, CIGAR_M);
  seqioCigarPush(cigar, CIGAR_X);
  seqioCigarPush(cigar, CIGAR_M);
  seqioCigarPush(cigar, CIGAR_M);
  seqioCigarPush(cigar, CIGAR_EQ);
  seqioCigarPush(cigar, CIGAR_EQ);
  seqioCigarPush(cigar, CIGAR_M);
  seqioCigarPush(cigar, CIGAR_M);
  char* s = seqioCigarToString(cigar);
  printf("%s\n", s);
  assert(strcmp(s, "D1M3X1M2=2M2") == 0);
  free(s);
  seqioCigarFree(cigar);
  cigar_string_t* cigar2 = seqioCigarFromString("D1M3X1M2=2M2");
  s = seqioCigarToString(cigar);
  printf("%s\n", s);
  assert(strcmp(s, "D1M3X1M2=2M2") == 0);
  free(s);
  seqioCigarFree(cigar2);
  return 0;
}
