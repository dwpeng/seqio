#include "seqio.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <zconf.h>

static char* openModeStr[] = {
  [seqOpenModeRead] = "r",
  [seqOpenModeWrite] = "w",
  [seqOpenModeAppend] = "a",
};

#ifdef enable_gzip
static char* openModeStrGzip[] = {
  [seqOpenModeRead] = "rb",
  [seqOpenModeWrite] = "wb",
  [seqOpenModeAppend] = "ab",
};
#endif

static inline char*
getOpenModeStr(seqioOpenOptions* options)
{
#ifdef enable_gzip
  if (options->isGzipped) {
    return openModeStrGzip[options->mode];
  } else {
    return openModeStr[options->mode];
  }
#else
  return openModeStr[options->mode];
#endif
}

static inline void
ensureWriteable(seqioFile* sf)
{
  if (sf->options->mode == seqOpenModeRead) {
    fprintf(stderr, "Cannot write to a file opened in read mode.\n");
    exit(1);
  }
}

static inline void
ensureReadable(seqioFile* sf)
{
  if (sf->options->mode == seqOpenModeWrite) {
    fprintf(stderr, "Cannot read from a file opened in write mode.\n");
    exit(1);
  }
}

static inline void
clearBuffer(seqioFile* sf)
{
  sf->buffer.offset = 0;
  sf->buffer.left = 0;
}

static inline void
cousumeBufferOne(seqioFile* sf)
{
  assert(sf->buffer.left > 0);
  sf->buffer.offset += 1;
  sf->buffer.left -= 1;
}

static inline size_t
readDataToBuffer(seqioFile* sf)
{
  ensureReadable(sf);
  if (sf->pravite.isEOF) {
    return 0;
  }
  size_t readSize = 0;
  size_t needReadSize = sf->buffer.capacity - sf->buffer.left;
  memmove(sf->buffer.data, sf->buffer.data + sf->buffer.offset,
          sf->buffer.left);
  sf->buffer.offset = 0;
#ifdef enable_gzip
  if (sf->options->isGzipped) {
    readSize =
        gzread(sf->file, sf->buffer.data + sf->buffer.offset, needReadSize);
  } else {
    readSize =
        fread(sf->buffer.data + sf->buffer.offset, 1, needReadSize, sf->file);
  }
#else
  readSize =
      fread(sf->buffer.data + sf->buffer.offset, 1, needReadSize, file->file);
#endif
  if (readSize < needReadSize) {
    sf->pravite.isEOF = true;
  }
  sf->buffer.left += readSize;
  return readSize;
}

// TODO offset/left
static inline size_t
writeDataFromBuffer(seqioFile* sf)
{
  ensureWriteable(sf);
  size_t writeSize = 0;
#ifdef enable_gzip
  if (sf->options->isGzipped) {
    writeSize = gzwrite(sf->file, sf->buffer.data, sf->buffer.capacity);
  } else {
    writeSize = fwrite(sf->buffer.data, 1, sf->buffer.capacity, sf->file);
  }
#else
  writeSize = fwrite(file->buffer.data, 1, file->buffer.capacity, file->file);
#endif
  return writeSize;
}

static inline void
resetFilePointer(seqioFile* sf)
{
#ifdef enable_gzip
  if (sf->options->isGzipped) {
    gzseek(sf->file, 0, SEEK_SET);
  } else {
    fseek(sf->file, 0, SEEK_SET);
  }
#else
  fseek(file->file, sf->pravite.offset, SEEK_SET);
#endif
  sf->pravite.isEOF = false;
  sf->buffer.left = 0;
  sf->buffer.offset = 0;
}

seqioFile*
seqioOpen(seqioOpenOptions* options)
{
  seqioFile* sf = (seqioFile*)seqioMalloc(sizeof(seqioFile));
  if (sf == NULL) {
    return NULL;
  }
  sf->options = options;
#ifdef enable_gzip
  if (options->isGzipped) {
    sf->file = gzopen(options->filename, getOpenModeStr(options));
    if (sf->file == NULL) {
      fclose(sf->file);
      free(sf);
      return NULL;
    }
  } else {
    sf->file = NULL;
  }
#else
  file->file = fopen(options->filename, getOpenModeStr(options));
  if (file->file == NULL) {
    fclose(file->file);
    free(file);
    return NULL;
  }
#endif
  sf->buffer.data = (char*)seqioMalloc(seqioDefaultBufferSize);
  if (sf->buffer.data == NULL) {
    fclose(sf->file);
    free(sf);
    return NULL;
  }
  sf->buffer.capacity = seqioDefaultBufferSize;
  sf->buffer.offset = 0;
  sf->buffer.left = 0;
  sf->pravite.type = seqioRecordTypeUnknown;
  sf->record = NULL;
  sf->pravite.isEOF = false;
  if (options->mode == seqOpenModeRead || options->mode == seqOpenModeAppend) {
    seqioGuessType(sf);
    resetFilePointer(sf);
  }
  return sf;
}

void
seqioClose(seqioFile* sf)
{
  if (sf == NULL) {
    return;
  }
  if (sf->file != NULL) {
#ifdef enable_gzip
    if (sf->options->isGzipped) {
      gzclose(sf->file);
    } else {
      fclose(sf->file);
    }
#else
    fclose(file->file);
#endif
  }
  if (sf->buffer.data != NULL) {
    seqioFree(sf->buffer.data);
  }
  if (sf->record != NULL) {
    seqioFree(sf->record);
  }
  seqioFree(sf);
}

seqioRecordType
seqioGuessType(seqioFile* sf)
{
  if (sf->pravite.type != seqioRecordTypeUnknown) {
    return sf->pravite.type;
  }
  if (sf->options->mode != seqOpenModeRead) {
    return seqioRecordTypeUnknown;
  }
  seqioRecordType type = seqioRecordTypeUnknown;
  int flag = 0;
  while (!sf->pravite.isEOF) {
    if (flag == 1) {
      break;
    }
    size_t readSize = readDataToBuffer(sf);
    if (readSize == 0) {
      return seqioRecordTypeUnknown;
    }
    for (size_t i = 0; i < readSize; i++) {
      if (sf->buffer.data[i] == '>') {
        type = seqioRecordTypeFasta;
        flag = 1;
        break;
      } else if (sf->buffer.data[i] == '@') {
        type = seqioRecordTypeFastq;
        flag = 1;
        break;
      }
    }
  }
  resetFilePointer(sf);
  sf->pravite.type = type;
  return type;
}

static inline seqioString*
seqioStringNew(size_t capacity)
{
  seqioString* string = (seqioString*)seqioMalloc(sizeof(seqioString));
  if (string == NULL) {
    exit(1);
  }
  string->data = (char*)seqioMalloc(capacity);
  if (string->data == NULL) {
    seqioFree(string);
    exit(1);
  }
  memset(string->data, 0, capacity);
  string->length = 0;
  return string;
}

static inline void
seqioStringFree(seqioString* string)
{
  if (string == NULL) {
    return;
  }
  if (string->data != NULL) {
    seqioFree(string->data);
  }
  seqioFree(string);
}

static inline seqioString*
seqioStringAppend(seqioString* string, char* data, size_t length)
{
  if (string->length + length > string->capacity) {
    string->data = (char*)seqioRealloc(string->data, string->length + length);
    if (string->data == NULL) {
      return NULL;
    }
    string->capacity = string->length + length;
    memset(string->data + string->length, 0, length);
  }
  memcpy(string->data + string->length, data, length);
  string->length += length;
  return string;
}

void
seqioFreeRecord(seqioRecord* record)
{
  if (record == NULL) {
    return;
  }
  if (record->type == seqioRecordTypeFasta) {
    seqioFastaRecord* fastaRecord = (seqioFastaRecord*)record;
    seqioStringFree(fastaRecord->name);
    seqioStringFree(fastaRecord->comment);
    seqioStringFree(fastaRecord->sequence);
  } else if (record->type == seqioRecordTypeFastq) {
    seqioFastqRecord* fastqRecord = (seqioFastqRecord*)record;
    seqioStringFree(fastqRecord->name);
    seqioStringFree(fastqRecord->comment);
    seqioStringFree(fastqRecord->sequence);
    seqioStringFree(fastqRecord->quality);
  }
  seqioFree(record);
}

static inline void
ensureFastqRecord(seqioFile* sf)
{
  if (sf->pravite.type != seqioRecordTypeFastq) {
    fprintf(stderr,
            "Cannot write fastq record to a file opened in fasta mode.\n");
    exit(1);
  }
}

static inline void
ensureFastaRecord(seqioFile* sf)
{
  if (sf->pravite.type != seqioRecordTypeFasta) {
    fprintf(stderr,
            "Cannot write fasta record to a file opened in fastq mode.\n");
    exit(1);
  }
}

seqioFastaRecord*
seqioReadFasta(seqioFile* sf)
{
  if (sf->pravite.isEOF) {
    return NULL;
  }
  ensureFastaRecord(sf);
  seqioFastaRecord* record =
      (seqioFastaRecord*)seqioMalloc(sizeof(seqioFastaRecord));
  if (record == NULL) {
    return NULL;
  }
  record->base.type = seqioRecordTypeFasta;
  record->name = seqioStringNew(50);
  record->comment = seqioStringNew(50);
  record->sequence = seqioStringNew(256);
  while (1) {
    size_t readSize = readDataToBuffer(sf);
    if (readSize == 0) {
      break;
    }
    for (size_t i = 0; i < readSize; i++) {
    }
  }
  return record;
}
