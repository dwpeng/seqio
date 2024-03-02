#include "cigar.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

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
  s = seqioCigarToString(cigar2);
  printf("%s\n", s);
  assert(strcmp(s, "D1M3X1M2=2M2") == 0);
  free(s);
  seqioCigarFree(cigar2);
  cigar = seqioCigarFromString("D1M3X1M2=2M2");
  char* ref = "AGGGTAAAAAA";
  char* query = "GGGCAAAAAA";
  cigar_alignment_t* alignment = seqioCigarToAlignment(cigar, ref, query);
  assert(strlen(alignment->alignment.aln) == 11);
  printf("%s\n", alignment->alignment.ref);
  printf("%s\n", alignment->alignment.aln);
  printf("%s\n", alignment->alignment.query);
  seqioCigarFreeAlignment(alignment);
  seqioCigarFree(cigar);
  return 0;
}
