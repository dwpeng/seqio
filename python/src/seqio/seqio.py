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
        self.__mode = mode
        self.__compressed = compressed
        if path == "-":
            self.__file = _seqioFile("", mode, compressed)
            return
        self.__file = _seqioFile(path, mode, compressed)


    @property
    def readable(self):
        return self.__mode == seqioOpenMode.READ
    
    @property
    def writable(self):
        return self.__mode == seqioOpenMode.WRITE

    def _get_file(self):
        return self.__file

    def readOne(self):
        if not self.readable:
            raise ValueError("File not opened in read mode")
        file = self._get_file()
        record = file.readOne()
        if record is None:
            return None
        return seqioRecord.fromRecord(record)

    def readFasta(self):
        if not self.readable:
            raise ValueError("File not opened in read mode")
        file = self._get_file()
        record = file.readFasta()
        if record is None:
            return None
        return seqioRecord.fromRecord(record)

    def readFastq(self):
        if not self.readable:
            raise ValueError("File not opened in read mode")
        file = self._get_file()
        record = file.readFastq()
        if record is None:
            return None
        return seqioRecord.fromRecord(record)

    def writeFastq(self, record: seqioRecord):
        if not self.writable:
            raise ValueError("File not opened in write mode")
        file = self._get_file()
        file.writeFastq(record.name, record.sequence, record.quality, record.comment)

    def writeFasta(self, record: seqioRecord):
        if not self.writable:
            raise ValueError("File not opened in write mode")
        file = self._get_file()
        file.writeFasta(record.name, record.sequence, record.comment)

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
        self.__mode = seqioOpenMode.READ

    def _get_file(self):
        return self.__file

class seqioStdoutFile(seqioFile):
    def __init__(self):
        self.__file = _seqioFile("", seqioOpenMode.WRITE, False)
        self.__mode = seqioOpenMode.WRITE

    def _get_file(self):
        return self.__file
