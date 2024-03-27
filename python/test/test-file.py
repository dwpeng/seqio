from seqio import seqioFile

file = seqioFile("test-data/test2.fa")

for record in file:
    print(record.name)
    print(record.sequence)
    print(record.length())
    print(len(record))
    assert record.length() == len(record)
