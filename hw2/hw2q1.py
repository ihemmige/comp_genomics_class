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
        name = first_line[1:].rstrip()
        seq = fh.readline().rstrip()
        fh.readline()  # ignore line starting with +
        qual = fh.readline().rstrip()
        reads.append((name, seq, qual))
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

if len(sys.argv) != 3:
  print("Usage: python3 hw2q1.py <input_file> <output_file>")
  sys.exit(1)

# Assign the input and output filenames from command-line arguments
input_file = sys.argv[1] # First argument: input filename
output_file = sys.argv[2] # Second argument: output filename

to_print = []
with open(input_file, 'r') as in_file, open(output_file, 'w') as out_file:
  reads = parse_fastq(in_file)
  to_print.extend(find_high_low_quality(reads))
  to_print.extend(find_extreme_base_quality(reads))
  to_print.append(count_non_nucleotides(reads))
  out_file.write(" ".join([str(t) for t in to_print]))