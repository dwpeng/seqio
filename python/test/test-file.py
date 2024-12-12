from fastseqio import seqioFile, Record

file = seqioFile("test-data/test2.fa")

for record in file:
    print(record.name)
    print(record.sequence)
    print(len(record))
    print(record.upper())
    print(record.hpc_commpress())


record = Record("test", "ACGGGGGGGTTTT")

print(record.hpc_commpress())

file = seqioFile("out.fa", "w")

file.writeFasta(record)
file.writeFasta(record)
