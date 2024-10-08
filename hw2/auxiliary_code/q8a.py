from collections import defaultdict

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

def generatePalindromes(n):
  if n%2 == 1:
    return []

  bases = ['A', 'T', 'G', 'C']
  half_strings = []
  
  def generatePossible(length, curString):
    if len(curString) == length:
      half_strings.append("".join(curString))
      return

    for b in bases:
      curString.append(b)
      generatePossible(length, curString)
      curString.pop()

  generatePossible(n/2, [])
  complement = {
     "A": "T", 
     "T": "A", 
     "G": "C",
     "C": "G"
  }
  result = []
  for half in half_strings:
    backhalf = ""
    for c in half[::-1]:
      backhalf += complement[c]
    result.append(half + backhalf)
  return result


# Assign the input and output filenames from command-line arguments
file_list = {
   "alpha": "q8/sarscov2_alpha.fa.txt", 
   "delta": "q8/sarscov2_delta.fa.txt", 
   "omicron": "q8/sarscov2_omicron.fa.txt",
   "wuhan": "q8/sarscov2_wuhan.fa.txt"
}
k = 6
virus_kmers = defaultdict(set)
rev_comp = set(generatePalindromes(k))
print(len(rev_comp))
for virus, file_name in file_list.items():
  with open(file_name, 'r') as fasta:
    genome = parse_fasta(fasta)
  kmer_index = generate_kmer_index(genome, k)
  virus_kmers[virus] = set(kmer_index.keys())

possibilities = virus_kmers["omicron"] - virus_kmers["delta"] - virus_kmers["wuhan"] - virus_kmers["alpha"]
print(len(possibilities))
result = [p for p in possibilities if p not in rev_comp]
print(len(result))



