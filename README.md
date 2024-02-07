## seqio
A c library for reading and writing sequences in fasta and fastq format.

## features
* Read and write fasta and fastq files
* Support for gzipped files

## usage
```c
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

  seqioFastaRecord* fastaRecord = NULL;
  seqioFastqRecord* fastqRecord = NULL;
  while ((fastaRecord = seqioReadFasta(sf1, fastaRecord)) != NULL) {
    printf(">%s %s\n", fastaRecord->name->data, fastaRecord->comment->data);
    printf("%s\n", fastaRecord->sequence->data);
    seqioWriteFasta(sf2, fastaRecord, &writeOptions2);
  }
  seqioClose(sf1);
  seqioClose(sf2);

  while ((fastqRecord = seqioReadFastq(sf3, fastqRecord)) != NULL) {
    printf("@%s %s\n", fastqRecord->name->data, fastqRecord->comment->data);
    printf("%s\n", fastqRecord->sequence->data);
    printf("+\n");
    printf("%s\n", fastqRecord->quality->data);
    seqioWriteFastq(sf4, fastqRecord, &writeOptions4);
  }
  seqioClose(sf3);
  seqioClose(sf4);
}
```

## memory check
```bash
valgrind --leak-check=full --leak-check=full --show-leak-kinds=all --log-file=./1.log --track-origins=yes ./main
```

```log
==501278== Memcheck, a memory error detector
==501278== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==501278== Using Valgrind-3.19.0 and LibVEX; rerun with -h for copyright info
==501278== Command: ./main
==501278== Parent PID: 41723
==501278== 
==501278== 
==501278== HEAP SUMMARY:
==501278==     in use at exit: 0 bytes in 0 blocks
==501278==   total heap usage: 43 allocs, 43 frees, 150,896 bytes allocated
==501278== 
==501278== All heap blocks were freed -- no leaks are possible
==501278== 
==501278== For lists of detected and suppressed errors, rerun with: -s
==501278== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

```

## test with kseq
```bash
make -f Makefile-test
time ./test-kseq ./test-data/test2.fa
time ./test-seqio ./test-data/test2.fa
```
