from _seqio import (
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
        name: str,
        sequence: str,
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
        self,
        path: str,
        mode: seqioOpenMode = seqioOpenMode.READ,
        compressed: bool = False,
    ):
        self.__file = _seqioFile(path, mode, compressed)

    def _get_file(self):
        return self.__file

    def readOne(self):
        file = self._get_file()
        record = file.readOne()
        if record is None:
            return None
        return seqioRecord.fromRecord(record)

    def readFasta(self):
        file = self._get_file()
        record = file.readFasta()
        if record is None:
            return None
        return seqioRecord.fromRecord(record)

    def readFastq(self):
        file = self._get_file()
        record = file.readFastq()
        if record is None:
            return None
        return seqioRecord.fromRecord(record)

    def __iter__(self):
        file = self._get_file()
        while True:
            record = file.readOne()
            if record is None:
                break
            yield seqioRecord.fromRecord(record)


class seqioStdinFile(seqioFile):
    def __init__(self):
        self.__file = _seqioFile("", seqioOpenMode.READ, False)

    def _get_file(self):
        return self.__file
