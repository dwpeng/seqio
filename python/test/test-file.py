import os
from fastseqio import seqioFile, Record

file = seqioFile("test-data/test2.fa")

for record in file:
    print(record.name)
    print(record.sequence)
    print(len(record))
    print(record.upper())
    print(record.hpc_commpress())
    print("##############")

for record in file:
    print(record.name)

print("NO Data")

file.reset()
for record in file:
    print("####", record.name)

record = Record("test", "ACGGGGGGGTTTT")

print(record.hpc_commpress())

file = seqioFile("out.fa", "w")

file.writeFasta(record)
file.writeFasta(record)

file.close()

content = ">test\nACGGGGGGGTTTT\n>test\nACGGGGGGGTTTT\n"

with open("out.fa", "r") as fp:
    data = fp.read()
    print(data)
    assert data == content

os.remove("out.fa")

id1 = id(record.sequence)
id2 = id(record.sequence)
print(id1, id2, id1 == id2)

record.sequence = record.sequence

print(record.sequence)

record.sequence += "AAAAAAAAA"

print(record.sequence)
