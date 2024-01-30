#include "seqio.h"

int
main()
{
  seqioOpenOptions openOptions = {
    .filename = "test.fa",
    .mode = seqOpenModeRead,
    .isGzipped = false,
  };
  seqioFile* sf = seqioOpen(&openOptions);
  seqioClose(sf);
}
