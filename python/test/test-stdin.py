from fastseqio import seqioStdinFile

file = seqioStdinFile()

for record in file:
    print(record.name)
    print(record.sequence)
    s = record.sequence
    print(s.upper())
