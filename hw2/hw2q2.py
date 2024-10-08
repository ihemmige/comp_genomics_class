import sys

def parse_fasta(fh):
    reads = []
    _ = fh.readline() # throw away first line
    acceptable = set(["A", "T", "G", "C"])
    while True:
        line = fh.readline().strip()
        if len(line) == 0:
            break  # end of file
        for c in line:
          if c in acceptable: # remove non-ACGT
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

if len(sys.argv) != 4:
  print("Usage: python3 hw2q1.py <FASTA_file> <input_file> <output_file>")
  sys.exit(1)

# Assign the input and output filenames from command-line arguments
fasta_file = sys.argv[1] # First argument: FASTA file
input_file = sys.argv[2] # Second argument: input file with k
output_file = sys.argv[3] # Third argument: output filename

with open(fasta_file, 'r') as fasta, open(input_file, 'r') as in_file, open(output_file, 'w') as out_file:
  genome = parse_fasta(fasta)
  k = int(in_file.readline().rstrip())
  kmer_index = generate_kmer_index(genome, k)
  num_keys = len(kmer_index)
  num_single_keys = sum(len(ct) == 1 for ct in kmer_index.values())
  out_file.write(str(num_keys) + " " + str(num_single_keys))
