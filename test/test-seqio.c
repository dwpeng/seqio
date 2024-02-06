#include "../seqio.h"
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
  seqioFastaRecord* fastaRecord = NULL;
  while ((fastaRecord = seqioReadFasta(sf, fastaRecord)) != NULL) {
    printf("name: %s: length: %lu\n", fastaRecord->name->data, fastaRecord->sequence->length);
  }
  seqioClose(sf);
}
