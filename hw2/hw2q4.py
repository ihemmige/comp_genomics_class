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
          if c in acceptable:
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

def find_num_hits(genome, kmer_index, read, partition_length, allowed_dif):
  num_hits = [] # number of index hits for each partition
  matching_offsets = {n:set() for n in range(allowed_dif+1)} # set prevents duplicates
  for i in range(len(read)//partition_length): # loop goes x+1 times
    # check index for matches
    potential_matches = kmer_index.get(read[i*partition_length:(i+1)*partition_length], [])
    num_hits.append(len(potential_matches))
    
    for idx in potential_matches:
      genome_ptr = idx - partition_length*i # find pattern's (potential) start in genome
      if genome_ptr < 0: # if the alignment hangs off genome start
        continue
      num_dif = 0
      for read_ptr in range(len(read)):
        if genome_ptr >= len(genome): # if the alignment hangs off genome end
          num_dif = float('inf')
          break
        num_dif += read[read_ptr] != genome[genome_ptr] # count differences
        genome_ptr += 1
      if num_dif in matching_offsets:
        matching_offsets[num_dif].add(idx - partition_length*i)
  return num_hits, matching_offsets

if len(sys.argv) != 4:
  print("Usage: python3 hw2q1.py <FASTA_file> <FASTQ_file> <output_file>")
  sys.exit(1)

# Assign the input and output filenames from command-line arguments
fasta_file = sys.argv[1] # First argument: FASTA file
fastq_file = sys.argv[2] # Second argument: FASTQ_file
output_file = sys.argv[3] # Third argument: output filename
P = 30 # length of read
x = 4 # allowed differences
k = P // (x + 1) # partition length
to_print = []
with open(fasta_file, 'r') as fasta, open(fastq_file, 'r') as fastq, open(output_file, 'w') as out_file:
  # process input and generate kmer
  genome = parse_fasta(fasta)
  kmer_index = generate_kmer_index(genome, k)
  reads = parse_fastq(fastq)

  to_print = []
  seen_reads = set()
  for r in reads:
    if r in seen_reads: # don't check repeated reads
      continue
    seen_reads.add(r)
    partition_hits, offsets = find_num_hits(genome, kmer_index, r, k, x)
    new_line = [str(ct) for ct in partition_hits]
    # num dif:index,index,...
    hits = [str(key) + ":" + (",".join(str(v) for v in sorted(vals))) for key, vals in offsets.items()]
    to_print.append(" ".join(new_line + hits))
  out_file.write("\n".join(to_print))