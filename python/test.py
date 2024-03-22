from seqio import seqioFile, seqioOpenMode

file = seqioFile("test-data/test2.fa", seqioOpenMode.READ, True)

for record in file:
    print(record.name)
    print(record.sequence)
    print(record.length())
    s = record.sequence
    print(s.upper())
