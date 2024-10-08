import sys

## borrowed from https://bit.ly/cgseq_FASTQ
def parse_fastq(fh):
    """ Parse reads from a FASTQ filehandle.  For each read, we
        return a name, nucleotide-string, quality-string triple. """
    reads = []
    while True:
        first_line = fh.readline()
        if len(first_line) == 0:
            break  # end of file
        name = first_line[1:].rstrip() # not needed here
        seq = fh.readline().rstrip()
        fh.readline()  # ignore line starting with +
        qual = fh.readline().rstrip() # not needed here
        reads.append(seq)
    return reads

def parse_fasta(fh):
    reads = []
    _ = fh.readline() # throw away first line
    acceptable = set(["A", "T", "G", "C"])
    while True:
        line = fh.readline().strip()
        if len(line) == 0:
            break  # end of file
        for c in line:
          if c in acceptable: # remove non-ATGC
            reads.append(c)
    return "".join(reads)

def generate_kmer_index(genome, k):
  kmer_map = {}
  for r in range(k-1, len(genome)):
    kmer = genome[r - k + 1:r+1]
    if kmer in kmer_map:
      kmer_map[kmer].append(r - k + 1)
    else:
      kmer_map[kmer] = [r - k + 1]
  return kmer_map

def find_max_occur_kmer(kmer_map):
  max_occur = 0
  res = []
  for key, occur in kmer_map.items():
    if len(occur) > max_occur: # if new max, reset
      max_occur = len(occur)
      res = [key]
    elif len(occur) == max_occur: # if equal to existing max, save all
      res.append(key)
  return res

def exact_align(genome, reads, kmer_index):
  index_to_align_ct = {}
  exact_alignments = 0
  seen_reads = set()
  for r in reads:
    # don't check repeated reads
    if r in seen_reads:
      continue
    seen_reads.add(r)
    # find indices of the prefix and check each for match
    potential_matches = kmer_index.get(r[:6], [])
    for idx in potential_matches:
      if r == genome[idx:idx + len(r)]:
        exact_alignments += 1
        index_to_align_ct[idx] = index_to_align_ct.get(idx, 0) + 1
  return index_to_align_ct, exact_alignments
      
if len(sys.argv) != 4:
  print("Usage: python3 hw2q1.py <FASTA_file> <FASTQ_file> <output_file>")
  sys.exit(1)

# Assign the input and output filenames from command-line arguments
fasta_file = sys.argv[1] # First argument: FASTA file
fastq_file = sys.argv[2] # Second argument: FASTQ_file
output_file = sys.argv[3] # Third argument: output filename

to_print = []
with open(fasta_file, 'r') as fasta, open(fastq_file, 'r') as fastq, open(output_file, 'w') as out_file:
  # parse input and generate kmer index
  genome = parse_fasta(fasta)
  kmer_index = generate_kmer_index(genome, 6)
  reads = parse_fastq(fastq)

  most_frequent = find_max_occur_kmer(kmer_index)
  most_frequent.sort()
  to_print.append(",".join(most_frequent))

  # find exact matches
  index_to_align_ct, ct_alignments = exact_align(genome, reads, kmer_index)
  to_print.append(str(ct_alignments))

  # find smallest index with maximum alignments
  max_ct = 0
  max_index = -1
  for idx, ct in index_to_align_ct.items():
    if ct > max_ct:
      max_index = idx
      max_ct = ct
    elif ct == max_ct:
      max_index = min(max_index, idx)

  to_print.append(str(max_index))
  out_file.write(" ".join(to_print))