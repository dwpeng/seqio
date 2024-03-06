#include "kseq.h"
#include <stdio.h>
#include <time.h>
#include <zlib.h>

KSEQ_INIT(gzFile, gzread)

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
benchmark_kseq_read(char* filename)
{
  gzFile fp;
  kseq_t* seq;
  int l;
  int nseq = 0;
  size_t sum_length = 0;
  if (filename == NULL) {
    return;
  }
  fp = gzopen(filename, "r");
  seq = kseq_init(fp);
  while ((l = kseq_read(seq)) >= 0) {
    sum_length += seq->seq.l;
    nseq++;
  }
  kseq_destroy(seq);
  gzclose(fp);
  printf("Sum of sequence length: %lu\n", sum_length);
  printf("Number of sequences: %d\n", nseq);
}

int
main(int argc, char* argv[])
{
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <in.seq>\n", argv[0]);
    return 1;
  }
  float start = timer();
  benchmark_kseq_read(argv[1]);
  float end = timer();
  printf("Time: %f\n", end - start);
  return 0;
}
