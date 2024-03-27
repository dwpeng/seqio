from seqio import seqioStdinFile

file = seqioStdinFile()

for record in file:
    print(record.name)
    print(record.sequence)
    print(record.length())
    s = record.sequence
    print(s.upper())

