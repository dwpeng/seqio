#include "seqio.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

inline static float
timer(void)
{
  struct timespec ts;
  if (clock_gettime(CLOCK_MONOTONIC, &ts) == -1) {
    return -1;
  }
  return (float)ts.tv_sec + (float)ts.tv_nsec / 1000000000.0;
}

static inline void
seqio_read(char* filename)
{
  seqioOpenOptions openOption = {
    .filename = filename,
  };
  seqioFile* sf = seqioOpen(&openOption);
  seqioRecord* record = NULL;
  size_t sum_length = 0;
  int nseq = 0;
  while ((record = seqioRead(sf, record)) != NULL) {
    sum_length += record->sequence->length;
    nseq++;
  }
  printf("Sum of sequence length: %zu\n", sum_length);
  printf("Number of sequences: %d\n", nseq);
  seqioClose(sf);
}

static inline void
seqio_write(char* filename, int gzip)
{
  seqioOpenOptions openOption = {
    .filename = filename,
  };
  char outname[1024] = { 0 };
  if (gzip) {
    snprintf(outname, 1024, "%s.out.gz", filename);
  } else {
    snprintf(outname, 1024, "%s.out", filename);
  }
  seqioOpenOptions op = {
    .filename = outname,
    .mode = seqOpenModeWrite,
    .isGzipped = gzip,
  };
  seqioFile* sf = seqioOpen(&openOption);
  seqioFile* sfWrite = seqioOpen(&op);
  seqioRecord* record = NULL;
  while ((record = seqioRead(sf, record)) != NULL) {
    if (record->type == seqioRecordTypeFasta) {
      seqioWriteFasta(sfWrite, record, NULL);
    } else if (record->type == seqioRecordTypeFastq) {
      seqioWriteFastq(sfWrite, record, NULL);
    }
  }
  seqioClose(sf);
  seqioClose(sfWrite);
}

int
main(int argc, char** argv)
{
  if (argc < 2) {
    printf("Usage: %s <input>\n", argv[0]);
    return 1;
  }
  float start;
  float end;
#ifdef READ
  printf("Reading %s\n", argv[1]);
  start = timer();
  seqio_read(argv[1]);
  end = timer();
  printf("Time: %.3f\n", end - start);
#endif
#ifdef WRITE
  printf("Writing %s.out\n", argv[1]);
  start = timer();
  seqio_write(argv[1], 0);
  end = timer();
  printf("Time: %.3f\n", end - start);
#endif
#ifdef WRITE_GZ
  printf("Writing %s.out.gz\n", argv[1]);
  start = timer();
  seqio_write(argv[1], 1);
  end = timer();
  printf("Time: %.3f\n", end - start);
#endif
  return 0;
}
