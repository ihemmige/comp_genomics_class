// Include necessary header files
#include <iostream> // For input/output operations
#include <fstream> // For file handling
#include <string> // For string operations
#include <vector>
#include <tuple>
#include <algorithm>

// modified from https://bit.ly/cgseq_FASTQ
void parseFastq(std::ifstream& inp, std::vector<std::pair<std::string, std::string>>& res) {
  std::string temp;
  while (1) {
    if (!std::getline(inp, temp)) break;
    std::string name = temp.substr(1);
    std::getline(inp, temp);
    std::string seq = std::move(temp);
    std::getline(inp, temp); // line with +
    std::getline(inp, temp); // line with base qualities
    res.push_back({seq, name});
  }
}

// modified from HW4 document
int suffixPrefixMatch(std::string a, std::string b, unsigned minOverlap) {
  if (a.size() < minOverlap) return 0;
  std::string b_prefix = b.substr(0, minOverlap);
  int a_pos = -1;
  while (1) {
    a_pos = a.find(b_prefix, a_pos + 1);
    if (a_pos == -1) return 0;
    std::string a_suffix = a.substr(a_pos);
    if (b.compare(0, a_suffix.size(), a_suffix) == 0) {
      return a_suffix.size();
    }
  }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
    std::cerr << "Usage: " << argv[0] << " <input_file> k <output_file>" << std::endl;
    return 1;
  }
  
  // Assign the input and output filenames from command-line arguments
  std::string input_file = argv[1]; // First argument: input filename
  int k = std::stoi(argv[2]);
  std::string output_file = argv[3]; // Second argument: output filename
  
  // Open input file for reading
  std::ifstream inp(input_file);
  // Open output file for writing
  std::ofstream outp(output_file);

  // parse fastq file to get reads and read IDs
  std::vector<std::pair<std::string, std::string>> reads;
  parseFastq(inp, reads);

  // stores the suffix/prefix matches of requisite length
  std::vector<std::tuple<std::string, int, std::string>> matches;

  std::string result; // output

  for (unsigned i = 0; i < reads.size(); i++) {
    int maxOverlap = 0;
    bool hasTie = false;
    std::string a, b;
    for (unsigned j = 0; j < reads.size(); j++) {
      if (i == j) continue; // don't compare to itself

      // maximum suffix/prefix match length
      int overlap = suffixPrefixMatch(reads[i].first, reads[j].first, k);

      // if a strict max, then save
      if (overlap > maxOverlap) {
        hasTie = false;
        maxOverlap = overlap;
        a = reads[i].second;
        b = reads[j].second;
      } else if (overlap == maxOverlap) { // if equal to a prior max, then there's a tie
        hasTie = true;
      }
    }
    
    // if there was a qualified overlap
    if (maxOverlap >= k && !hasTie) {
      matches.push_back({a, maxOverlap, b});
    }
  }

  std::sort(matches.begin(), matches.end());

  for (const auto& t : matches) {
    result += std::get<0>(t) + " " + std::to_string(std::get<1>(t)) + " " + std::get<2>(t) + '\n';
  }

  if (result.size()) result.pop_back(); // remove trailing newline

  outp << result;

  // Close the input and output files
  inp.close();
  outp.close();
  return 0;
}