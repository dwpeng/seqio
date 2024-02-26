#include "seqio.h"

namespace seqio {
int
read()
{
  seqioOpenOptions openOptions = { 
    .filename = NULL,
    .isGzipped = false,
    .mode = seqOpenModeRead
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
  return 0;
}
} // namespace seqio

int
main()
{
  seqio::read();
  return 0;
}
