import os
import subprocess

if not os.path.isdir("seqkit-benchmark-data"):
  subprocess.run(["wget", "http://app.shenwei.me/data/seqkit/seqkit-benchmark-data.tar.gz", "-O", "seqkit-benchmark-data.tar.gz"])
  subprocess.run(["tar", "-xzf", "seqkit-benchmark-data.tar.gz"])

subprocess.run(["make", "clean"])
subprocess.run(["make"])

bins = [
  'benchmark-seqio-read',
  'benchmark-kseq'
]

cwd = os.getcwd()

datasets = [
  'seqkit-benchmark-data/dataset_A.fa',
  'seqkit-benchmark-data/dataset_B.fa',
  'seqkit-benchmark-data/dataset_C.fq',
]

def test(exe, data):
  exe = os.path.join(cwd, exe)
  data = os.path.join(cwd, data)
  # hot data cache
  for i in range(3):
    os.system(exe + " " + data + " > /dev/null 2>&1")
  subprocess.run([exe, data])

for exe in bins:
  print('***************************')
  print(exe)
  for data in datasets:
    test(exe, data)
  print('***************************')
