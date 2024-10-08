from collections import defaultdict

def generate_kmer_index(genome, k):
  kmer_map = {}
  for r in range(k-1, len(genome)):
    kmer = genome[r - k + 1:r+1]
    if kmer in kmer_map:
      kmer_map[kmer].append(r - k + 1)
    else:
      kmer_map[kmer] = [r - k + 1]
  return kmer_map

def create_check_complement(kmer):
  res = []
  complement = {
     "A": "T", 
     "T": "A", 
     "G": "C",
     "C": "G"
  }
  for c in reversed(kmer):
    res.append(complement[c])
  res = "".join(res)
  return min(res, kmer)

k = 5
genome = "ATGCATGCATGC"
# genome = "ACGTA"
kmer_index = generate_kmer_index(genome, k)
kmers = kmer_index.keys()
canon_set = set()
for k in kmers:
  canonical = create_check_complement(k)
  canon_set.add(canonical)
print(canon_set)