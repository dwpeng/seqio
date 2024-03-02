#ifndef __seqio_cigar_h__
#define __seqio_cigar_h__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CIGAR_M 0
#define CIGAR_I 1
#define CIGAR_D 2
#define CIGAR_N 3
#define CIGAR_S 4
#define CIGAR_H 5
#define CIGAR_P 6
#define CIGAR_EQ 7
#define CIGAR_X 8

#define CIGAR_STATE "MIDNSHP=X"

typedef struct cigar_block_t cigar_block_t;

typedef struct {
  struct {
    int length;
    int capacity;
    cigar_block_t* cigars;
  } _mete_info;
} cigar_string_t;

typedef struct{
  const cigar_string_t* cigar;
  const char* reference;
  const char* query;
  struct{
    char* ref;
    char* query;
    char* aln;
  }alignment;
}cigar_alignment_t;

cigar_string_t* seqioCigarInit();
void seqioCigarFree(cigar_string_t* cigar);
void seqioCigarPush(cigar_string_t* cigar, char state);
char* seqioCigarToString(cigar_string_t* cigar);
cigar_string_t* seqioCigarFromString(char* cigar_string);
cigar_alignment_t* seqioCigarToAlignment(const cigar_string_t* cigar, const char* reference, const char* query);
void seqioCigarFreeAlignment(cigar_alignment_t* alignment);
#ifdef __cplusplus
}
#endif
#endif // __seqio_cigar_h__
