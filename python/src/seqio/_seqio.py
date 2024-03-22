from .lib._seqio import (
    seqioFile as _seqioFile,
    seqOpenMode as _seqOpenMode,
    seqioRecord as _seqioRecord,
)
from typing import Optional

class seqioOpenMode:
    READ = _seqOpenMode.READ
    WRITE = _seqOpenMode.WRITE


class seqioRecord:
    def __init__(
        self,
        name,
        sequence,
        quality: Optional[str] = None,
        comment: Optional[str] = None,
    ):
        self.__record = None
        self.__length = len(sequence)
        self.name = name
        self.sequence = sequence
        self.quality = quality
        self.comment = comment

    @classmethod
    def fromRecord(cls, record: _seqioRecord):
        self = cls(record.name, record.sequence, record.quality, record.comment)
        self.__record = record
        self.__length = record.length()
        return self

    def length(self):
        return self.__length

    def upper(self):
        if self.__record is not None:
            self.sequence = self.__record.upper()
        else:
            self.sequence = self.sequence.upper()
        return self

    def lower(self):
        if self.__record is not None:
            self.sequence = self.__record.lower()
        else:
            self.sequence = self.sequence.lower()
        return self

    def __str__(self):
        return f"seqioRecord(name={self.name})"


class seqioFile:
    def __init__(
        self, path: str, mode: seqioOpenMode = seqioOpenMode.READ, compressed: bool = False
    ):
        self.__file = _seqioFile(path, mode, compressed)

    def readOne(self):
        record = self.__file.readOne()
        if record is None:
            return None
        return seqioRecord.fromRecord(record)

    def readFasta(self):
        record = self.__file.readFasta()
        if record is None:
            return None
        return seqioRecord.fromRecord(record)

    def readFastq(self):
        record = self.__file.readFastq()
        if record is None:
            return None
        return seqioRecord.fromRecord(record)

    def __iter__(self):
        while True:
            record = self.__file.readOne()
            if record is None:
                break
            yield seqioRecord.fromRecord(record)
