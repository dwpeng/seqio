#include "cigar.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char CIGAR_STATE_CHAR[] = {
  'M', 'I', 'D', 'N', 'S', 'H', 'P', '=', 'X',
};

struct cigar_block_t {
  char state;
  int length;
};

cigar_string_t*
seqioCigarInit()
{
  cigar_string_t* cigar = (cigar_string_t*)malloc(sizeof(cigar_string_t));
  if (cigar == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
    return NULL;
  }
  cigar->_mete_info.cigars =
      (cigar_block_t*)malloc(sizeof(cigar_block_t) * 10);
  if (cigar->_mete_info.cigars == NULL) {
    perror("malloc");
    free(cigar);
    exit(EXIT_FAILURE);
    return NULL;
  }
  cigar->_mete_info.capacity = 10;
  cigar->_mete_info.length = 0;
  return cigar;
}

void
seqioCigarFree(cigar_string_t* cigar)
{
  free(cigar->_mete_info.cigars);
  free(cigar);
}

void
seqioCigarPush(cigar_string_t* cigar, char state)
{
  if (cigar->_mete_info.length && state == cigar->_mete_info.cigars[cigar->_mete_info.length - 1].state) {
    cigar->_mete_info.cigars[cigar->_mete_info.length - 1].length++;
    return;
  }
  if (cigar->_mete_info.length == cigar->_mete_info.capacity) {
    cigar->_mete_info.capacity *= 2;
    cigar->_mete_info.cigars = (cigar_block_t*)realloc(
        cigar->_mete_info.cigars,
        sizeof(cigar_block_t) * cigar->_mete_info.capacity);
    if (cigar->_mete_info.cigars == NULL) {
      perror("realloc");
      exit(EXIT_FAILURE);
    }
  }
  cigar->_mete_info.cigars[cigar->_mete_info.length].state = state;
  cigar->_mete_info.cigars[cigar->_mete_info.length].length = 1;
  cigar->_mete_info.length++;
  return;
}

char*
seqioCigarToString(cigar_string_t* cigar)
{
  int slen = 0;
  int i = 0;
  for (; i < cigar->_mete_info.length; i++) {
    slen += 1; // length of the state
    cigar_block_t* block = &cigar->_mete_info.cigars[i];
    if (block->length < 10) {
      slen += 1; // length of the number (1 digit)
    } else if (block->length < 100) {
      slen += 2; // length of the number
    } else if (block->length < 1000) {
      slen += 3; // length of the number
    } else if (block->length < 10000) {
      slen += 4; // length of the number
    } else if (block->length < 100000) {
      slen += 5; // length of the number
    } else if (block->length < 1000000) {
      slen += 6; // length of the number
    } else if (block->length < 10000000) {
      slen += 7; // length of the number
    } else if (block->length < 100000000) {
      slen += 8; // length of the number
    } else if (block->length < 1000000000) {
      slen += 9; // length of the number
    } else {
      slen += 10; // length of the number
    }
  }
  char* str = (char*)malloc(sizeof(char) * (slen + 1));
  if (str == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  char numberBuffer[11] = { 0 };
  int j = 0;
  for (i = 0; i < cigar->_mete_info.length; i++) {
    cigar_block_t* block = &cigar->_mete_info.cigars[i];
    str[j] = CIGAR_STATE[(int)block->state];
    j++;
    int offset = snprintf(numberBuffer, 11, "%d", block->length);
    memcpy(str+j, numberBuffer, offset);
    j += offset;
  }
  str[j] = '\0';
  return str;
}

cigar_string_t*
seqioCigarFromString(char* str)
{
  cigar_string_t* cigar = seqioCigarInit();
  char numberBuffer[11] = { 0 };
  int i = 0;
  int slen = strlen(str);
  for (; i < slen;) {
    char state = str[i];
    int valid = 0;
    for (int si = 0; si < (int)strlen(CIGAR_STATE); si++) {
      if (state == CIGAR_STATE_CHAR[si]) {
        state = si;
        valid = 1;
        break;
      }
    }
    if (!valid) {
      fprintf(stderr, "Invalid state: %c\n", state);
      exit(EXIT_FAILURE);
    }
    i++;
    int j = 0;
    while (i < slen && str[i] >= '0' && str[i] <= '9') {
      numberBuffer[j] = str[i];
      i++;
      j++;
    }
    numberBuffer[j] = '\0';
    int length = atoi(numberBuffer);
    for (int k = 0; k < length; k++) {
      seqioCigarPush(cigar, state);
    }
  }
  return cigar;
}


void
seqioCigarFreeAlignment(cigar_alignment_t *alignment)
{
  free(alignment->alignment.query);
  free(alignment->alignment.ref);
  free(alignment->alignment.aln);
  free(alignment);
  return;
}


cigar_alignment_t*
seqioCigarToAlignment(const cigar_string_t* cigar,
                      const char* reference,
                      const char* query)
{
  int refLen = strlen(reference);
  int queryLen = strlen(query);
  int alignmentLen = 0;
  int cigarRefLen = 0;
  int cigarQueryLen = 0;
  for (int i = 0; i < cigar->_mete_info.length; i++) {
    switch (cigar->_mete_info.cigars[i].state) {
    case CIGAR_M: {
      alignmentLen += cigar->_mete_info.cigars[i].length;
      cigarRefLen += cigar->_mete_info.cigars[i].length;
      cigarQueryLen += cigar->_mete_info.cigars[i].length;
      break;
    }
    case CIGAR_I: {
      alignmentLen += cigar->_mete_info.cigars[i].length;
      cigarQueryLen += cigar->_mete_info.cigars[i].length;
      break;
    }
    case CIGAR_D: {
      alignmentLen += cigar->_mete_info.cigars[i].length;
      cigarRefLen += cigar->_mete_info.cigars[i].length;
      break;
    }
    case CIGAR_N: {
      alignmentLen += cigar->_mete_info.cigars[i].length;
      cigarRefLen += cigar->_mete_info.cigars[i].length;
      break;
    }
    case CIGAR_S: {
      alignmentLen += cigar->_mete_info.cigars[i].length;
      cigarQueryLen += cigar->_mete_info.cigars[i].length;
      break;
    }
    case CIGAR_EQ: {
      alignmentLen += cigar->_mete_info.cigars[i].length;
      cigarRefLen += cigar->_mete_info.cigars[i].length;
      cigarQueryLen += cigar->_mete_info.cigars[i].length;
      break;
    }
    case CIGAR_X: {
      alignmentLen += cigar->_mete_info.cigars[i].length;
      cigarRefLen += cigar->_mete_info.cigars[i].length;
      cigarQueryLen += cigar->_mete_info.cigars[i].length;
      break;
    }
    default: {
      break;
    }
    }
  }
  if (cigarQueryLen != queryLen || cigarRefLen != refLen) {
    fprintf(stderr, "CIGAR length does not match the sequence length\n");
    fprintf(stderr, "Expected ref: %d, got: %d\n", cigarRefLen, refLen);
    fprintf(stderr, "Expected query: %d, got: %d\n", cigarQueryLen, queryLen);
    exit(EXIT_FAILURE);
  }
  cigar_alignment_t* alignment =
      (cigar_alignment_t*)malloc(sizeof(cigar_alignment_t));
  if (alignment == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  alignment->cigar = cigar;
  alignment->query = query;
  alignment->reference = reference;
  alignment->alignment.query =
      (char*)malloc(sizeof(char) * (alignmentLen + 1));
  alignment->alignment.ref = (char*)malloc(sizeof(char) * (alignmentLen + 1));
  alignment->alignment.aln = (char*)malloc(sizeof(char) * (alignmentLen + 1));
  if (alignment->alignment.query == NULL || alignment->alignment.ref == NULL
      || alignment->alignment.aln == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  int alnIndex = 0;
  int refIndex = 0;
  int queryIndex = 0;

  for (int i = 0; i < cigar->_mete_info.length; i++) {
    switch (cigar->_mete_info.cigars[i].state) {
    case CIGAR_M:
    case CIGAR_EQ:
    case CIGAR_X: {
      for (int j = 0; j < cigar->_mete_info.cigars[i].length; j++) {
        alignment->alignment.query[alnIndex] = query[queryIndex];
        alignment->alignment.ref[alnIndex] = reference[refIndex];
        if (query[queryIndex] == reference[refIndex]) {
          alignment->alignment.aln[alnIndex] = '|';
        } else {
          alignment->alignment.aln[alnIndex] = '*';
        }
        alnIndex++;
        refIndex++;
        queryIndex++;
      }
      break;
    }
    case CIGAR_I: {
      for (int j = 0; j < cigar->_mete_info.cigars[i].length; j++) {
        alignment->alignment.query[alnIndex] = query[queryIndex];
        alignment->alignment.ref[alnIndex] = '-';
        alignment->alignment.aln[alnIndex] = ' ';
        alnIndex++;
        queryIndex++;
      }
      break;
    }
    case CIGAR_D: {
      for (int j = 0; j < cigar->_mete_info.cigars[i].length; j++) {
        alignment->alignment.query[alnIndex] = '-';
        alignment->alignment.ref[alnIndex] = reference[refIndex];
        alignment->alignment.aln[alnIndex] = ' ';
        alnIndex++;
        refIndex++;
      }
      break;
    }
    case CIGAR_N: {
      for (int j = 0; j < cigar->_mete_info.cigars[i].length; j++) {
        alignment->alignment.query[alnIndex] = '-';
        alignment->alignment.ref[alnIndex] = reference[refIndex];
        alignment->alignment.aln[alnIndex] = ' ';
        alnIndex++;
        refIndex++;
      }
      break;
    }
    case CIGAR_S: {
      for (int j = 0; j < cigar->_mete_info.cigars[i].length; j++) {
        alignment->alignment.query[alnIndex] = query[queryIndex];
        alignment->alignment.ref[alnIndex] = '-';
        alignment->alignment.aln[alnIndex] = ' ';
        alnIndex++;
        queryIndex++;
      }
      break;
    }
    default: {
      break;
    }
    }
  }
  alignment->alignment.query[alnIndex] = '\0';
  alignment->alignment.ref[alnIndex] = '\0';
  alignment->alignment.aln[alnIndex] = '\0';
  return alignment;
}
