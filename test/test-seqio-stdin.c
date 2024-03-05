#include "seqio.h"

#define UNUSED(x) (void)(x)

int
main(int argc, char* argv[])
{
  UNUSED(argc);
  UNUSED(argv);
  seqioFile* sf = seqioStdinOpen();
  seqioRecord* record = NULL;
  while ((record = seqioRead(sf, record)) != NULL) {
      printf("name: %s: length: %lu\n", record->name->data,
             record->sequence->length);
    
  }
  seqioClose(sf);
}
