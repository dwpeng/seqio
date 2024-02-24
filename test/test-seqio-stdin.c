#include "seqio.h"

#define UNUSED(x) (void)(x)

int
main(int argc, char* argv[])
{
  UNUSED(argc);
  UNUSED(argv);
  seqioOpenOptions openOptions = {
    .filename = NULL,
    .mode = seqOpenModeRead,
  };
  seqioFile* sf = seqioOpen(&openOptions);
  seqioRecord* record = NULL;
  while ((record = seqioRead(sf, record)) != NULL) {
    if (record->type == seqioRecordTypeFasta) {
      seqioFastaRecord* fastaRecord = (seqioFastaRecord*)record;
      printf("name: %s: length: %lu\n", fastaRecord->name->data,
             fastaRecord->sequence->length);
    } else if (record->type == seqioRecordTypeFastq) {
      seqioFastqRecord* fastqRecord = (seqioFastqRecord*)record;
      printf("name: %s: length: %lu\n", fastqRecord->name->data,
             fastqRecord->sequence->length);
    }
  }
  seqioClose(sf);
}
