import sys
from collections import defaultdict

## borrowed from https://bit.ly/cgseq_FASTQ
def parse_fastq(fh):
    """ Parse reads from a FASTQ filehandle.  For each read, we
        return a name, nucleotide-string, quality-string triple. """
    reads = []
    while True:
        first_line = fh.readline()
        if len(first_line) == 0:
            break  # end of file
        name = first_line[1:].rstrip()
        seq = fh.readline().rstrip()
        fh.readline()  # ignore line starting with +
        qual = fh.readline().rstrip()
        reads.append(seq)
    return reads

## borrowed from https://bit.ly/cgseq_FASTQ
def phred33_to_q(qual):
  """ Turn Phred+33 ASCII-encoded quality into Phred-scaled integer """
  return ord(qual)-33

def find_high_low_quality(reads):
  min_quality = float('inf')
  min_index = -1
  max_quality = float('-inf')
  max_index = -1

  for i, read in enumerate(reads):
    quality = read[2]
    total_quality = sum(phred33_to_q(c) for c in quality)
    # if new minimum
    if total_quality < min_quality:
      min_index = i
      min_quality = total_quality
    # if new maximum
    if total_quality > max_quality:
      max_index = i
      max_quality = total_quality
  return min_index + 1, max_index + 1
  
def find_extreme_base_quality(reads):
  ct_under_10 = ct_30 = 0
  for read in reads:
    quality = read[2]
    for c in quality:
      q = phred33_to_q(c)
      ct_under_10 += q < 10
      ct_30 += q >= 30
  return ct_under_10, ct_30

def count_non_nucleotides(reads):
  ct_non_nucleo = 0
  bases = set(['A', 'T', 'G', 'C'])
  for read in reads:
    seq = read[1]
    ct_non_nucleo += sum(c not in bases for c in seq) # count non-nucleotides
  return ct_non_nucleo

file_a = "q6/hw2q1_A.fastq.txt"
file_b = "q6/hw2q1_B.fastq.txt"
file_c = "q6/hw2q1_C.fastq.txt"


to_print = []
with open(file_a, 'r') as a_file, open(file_b, 'r') as b_file, open(file_c, 'r') as c_file:
  a_reads = parse_fastq(a_file)
  b_reads = parse_fastq(b_file)
  c_reads = parse_fastq(c_file)

  a_AT = []
  total_at = 0
  total_bases = 0
  for r in a_reads:
    ct = 0
    for i,c in enumerate(r):
      if c in ["A", "T"]:
        ct += 1
        total_at += 1
      total_bases += 1
    a_AT.append(ct / len(r))
  print(sum(a_AT)/len(a_AT))
  print(total_at/total_bases, '\n')

  b_AT = []
  total_at = 0
  total_bases = 0
  for r in b_reads:
    ct = 0
    for i,c in enumerate(r):
      if c in ["A", "T"]:
        ct += 1
        total_at += 1
      total_bases += 1
    b_AT.append(ct / len(r))
  print(sum(b_AT)/len(b_AT))
  print(total_at/total_bases, '\n')

  c_AT = []
  total_at = 0
  total_bases = 0
  for r in c_reads:
    ct = 0
    for i,c in enumerate(r):
      if c in ["A", "T"]:
        ct += 1
        total_at += 1
      total_bases += 1
    c_AT.append(ct / len(r))
  print(sum(c_AT)/len(c_AT))
  print(total_at/total_bases, '\n')
