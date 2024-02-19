#include "seqio.h"

int
main()
{
  seqioOpenOptions openOptions1 = {
    .filename = "./test-data/test1.fa.gz",
    .mode = seqOpenModeRead,
  };
  seqioFile* sf1 = seqioOpen(&openOptions1);

  seqioOpenOptions openOptions2 = {
    .filename = "./test-data/test2.fa",
    .mode = seqOpenModeWrite,
    .isGzipped = false,
  };
  seqioFile* sf2 = seqioOpen(&openOptions2);

  seqioOpenOptions openOptions3 = {
    .filename = "./test-data/test3.fq.gz",
    .mode = seqOpenModeRead,
  };
  seqioFile* sf3 = seqioOpen(&openOptions3);

  seqioOpenOptions openOptions4 = {
    .filename = "./test-data/test4.fq",
    .mode = seqOpenModeWrite,
    .isGzipped = false,
  };
  seqioFile* sf4 = seqioOpen(&openOptions4);

  seqioWriteOptions writeOptions2 = {
    .lineWidth = 5,
    .baseCase = seqioBaseCaseLower,
    .includeComment = true,
  };

  seqioWriteOptions writeOptions4 = {
    .baseCase = seqioBaseCaseUpper,
    .includeComment = true,
  };

  seqioRecord* Record1 = NULL;
  seqioRecord* Record2 = NULL;
  seqioFastaRecord* fastaRecord = NULL;
  seqioFastqRecord* fastqRecord = NULL;
  while ((Record1 = seqioRead(sf1, Record1)) != NULL) {
    fastaRecord = (seqioFastaRecord*)Record1;
    printf(">%s %s\n", fastaRecord->name->data, fastaRecord->comment->data);
    printf("%s\n", fastaRecord->sequence->data);
    seqioWriteFasta(sf2, fastaRecord, &writeOptions2);
  }
  seqioClose(sf1);
  seqioClose(sf2);

  while ((Record2 = seqioRead(sf3, Record2)) != NULL) {
    fastqRecord = (seqioFastqRecord*)Record2;
    printf("@%s %s\n", fastqRecord->name->data, fastqRecord->comment->data);
    printf("%s\n", fastqRecord->sequence->data);
    printf("+\n");
    printf("%s\n", fastqRecord->quality->data);
    seqioWriteFastq(sf4, fastqRecord, &writeOptions4);
  }

  seqioClose(sf3);
  seqioClose(sf4);
}
