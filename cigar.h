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

typedef struct cigar_block_t {
  char state;
  int length;
} cigar_block_t;

typedef struct {
  struct {
    int length;
    int capacity;
    cigar_block_t* cigars;
  } _mete_info;
} cigar_string_t;

cigar_string_t* seqioCigarInit();
void seqioCigarFree(cigar_string_t* cigar);
void seqioCigarPush(cigar_string_t* cigar, char state);
char* seqioCigarToString(cigar_string_t* cigar);
cigar_string_t* seqioCigarFromString(char* cigar_string);
#ifdef __cplusplus
}
#endif
#endif // __seqio_cigar_h__
