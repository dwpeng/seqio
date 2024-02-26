#include "seqio.h"
#include <stdio.h>

int
main(int argc, char* argv[])
{
  if (argc == 1) {
    fprintf(stderr, "Usage: %s <in.fasta>\n", argv[0]);
    return 1;
  }
  seqioOpenOptions openOptions = {
    .filename = argv[1],
    .mode = seqOpenModeRead,
  };
  seqioFile* sf = seqioOpen(&openOptions);
  seqioRecord* Record = NULL;
  while ((Record = seqioRead(sf, Record)) != NULL) {
  }
  seqioClose(sf);
}
