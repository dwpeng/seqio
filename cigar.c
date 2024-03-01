#include "cigar.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

static char CIGAR_STATE_CHAR[] = {
  'M', 'I', 'D', 'N', 'S', 'H', 'P', '=', 'X',
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
  if (state == cigar->_mete_info.cigars[cigar->_mete_info.length - 1].state) {
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
    for (int k = 0; k < offset; k++) {
      str[j] = numberBuffer[k];
      j++;
    }
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
