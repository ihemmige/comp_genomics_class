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
        qual = fh.readline().rstrip()
        reads.append((seq, qual))
    return reads

## borrowed from https://bit.ly/cgseq_FASTQ
def phred33_to_q(qual):
  """ Turn Phred+33 ASCII-encoded quality into Phred-scaled integer """
  return ord(qual)-33

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

def find_mismatches(genome, kmer_index, read, qual, partition_length, allowed_dif):
  offset_to_mismatch = {} # for an offset, stores its mismatched bases and their quality
  for i in range(len(read)//partition_length): # loop goes x+1 times
    # check index for matches
    potential_matches = kmer_index.get(read[i*partition_length:(i+1)*partition_length], [])
    for idx in potential_matches:
      genome_ptr = idx - partition_length*i # find pattern's (potential) start in genome
      if genome_ptr < 0: # if the alignment hangs off genome start
        continue
      num_dif = 0
      differences = {} # stores any differences this read has from reference
      for read_ptr in range(len(read)):
        if genome_ptr >= len(genome): # if the alignment hangs off genome end
          num_dif = float('inf')
          break
        if read[read_ptr] != genome[genome_ptr]:
          num_dif += 1  # count differences
          if genome_ptr in differences:
            differences[genome_ptr].add((read[read_ptr], qual[read_ptr]))
          else:
            differences[genome_ptr] = set([(read[read_ptr], qual[read_ptr])])
        genome_ptr += 1
      # if the read had acceptable # of differences, add the differences found
      if num_dif <= allowed_dif: 
        offset_to_mismatch = {k: offset_to_mismatch.get(k, set()).union(differences.get(k, set())) for k in offset_to_mismatch.keys() | differences.keys()}
  return offset_to_mismatch

def process_mismatches(genome, differences_to_check):
  to_print = []
  # for each index, check its mismatches for most likely non-reference base
  for index, bases in differences_to_check.items():
    base_to_weight = {} # ATGC to their total weights
    for base, qual in bases:
      value = phred33_to_q(qual)
      if base in base_to_weight:
        base_to_weight[base] += value
      else:
        base_to_weight[base] = value
    
    # sort highest to lowest by total weight, then base
    ordered_bases = sorted(base_to_weight.items(), key=lambda item: (-item[1], item[0]))

    first_base = ''
    second_base = '-'
    first_weight = 0
    second_weight = 0
    # if any differences were found
    if len(ordered_bases) > 0:
      # if the first non-reference base has high enough weight
      if ordered_bases[0][1] > 20:
        first_base = ordered_bases[0][0]
        first_weight = ordered_bases[0][1]
        # if there's a second base and its weight is large enough
        if len(ordered_bases) > 1 and ordered_bases[1][1] > 20:
          second_base = ordered_bases[1][0]
          second_weight = ordered_bases[1][1]
    if first_base: # not the empty string, then we need to write a line
      new_row = [str(index), genome[index], first_base, str(first_weight), second_base, str(second_weight)]
      to_print.append((index, " ".join(new_row)))
  to_print.sort()
  return [t[1] for t in to_print]

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
  differences_to_check = {}
  # check each read against the reference genome using index
  for r,qual in reads:
    offset_to_mismatch = find_mismatches(genome, kmer_index, r, qual, k, x)
    # for each offset, get its mismatch and save
    for off, pairs in offset_to_mismatch.items():
      if off in differences_to_check:
        differences_to_check[off].extend(list(pairs))
      else:
        differences_to_check[off] = list(pairs)

  to_print = process_mismatches(genome, differences_to_check)
  out_file.write("\n".join(to_print))