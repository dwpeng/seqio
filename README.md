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
  while ((Record1 = seqioRead(sf1, Record1)) != NULL) {
    printf(">%s %s\n", Record1->name->data, Record1->comment->data);
    printf("%s\n", Record1->sequence->data);
    seqioWriteFasta(sf2, Record1, &writeOptions2);
  }
  seqioClose(sf1);
  seqioClose(sf2);

  while ((Record2 = seqioRead(sf3, Record2)) != NULL) {
    printf("@%s %s\n", Record2->name->data, Record2->comment->data);
    printf("%s\n", Record2->sequence->data);
    printf("+\n");
    printf("%s\n", Record2->quality->data);
    seqioWriteFastq(sf4, Record2, &writeOptions4);
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
==1139682== Memcheck, a memory error detector
==1139682== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==1139682== Using Valgrind-3.19.0 and LibVEX; rerun with -h for copyright info
==1139682== Command: ./main
==1139682== Parent PID: 1107213
==1139682== 
==1139682== 
==1139682== HEAP SUMMARY:
==1139682==     in use at exit: 0 bytes in 0 blocks
==1139682==   total heap usage: 45 allocs, 45 frees, 151,872 bytes allocated
==1139682== 
==1139682== All heap blocks were freed -- no leaks are possible
==1139682== 
==1139682== For lists of detected and suppressed errors, rerun with: -s
==1139682== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

## test with kseq
```bash
make -f Makefile-test
time ./test-kseq ./test-data/test2.fa
time ./test-seqio ./test-data/test2.fa
```
