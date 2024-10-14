// Include necessary header files
#include <iostream> // For input/output operations
#include <fstream> // For file handling
#include <string> // For string operations
#include <vector>
#include <algorithm>

void generateSortedSuffixes(std::string& a, std::vector<std::pair<std::string, int>>& result) {
  for (int i = a.size() - 1; i >= 0; i--) {
    result.push_back({a.substr(i), i});
  }
  std::sort(result.begin(), result.end());
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
    return 1;
  }
  
  // Assign the input and output filenames from command-line arguments
  std::string input_file = argv[1]; // First argument: input filename
  std::string output_file = argv[2]; // Second argument: output filename
  
  // Open input file for reading
  std::ifstream inp(input_file);
  // Open output file for writing
  std::ofstream outp(output_file);

  // read input
  std::string a;
  std::getline(inp, a);

  // get sorted suffixes list
  std::vector<std::pair<std::string, int>> suffixes;
  generateSortedSuffixes(a, suffixes);

  // use suffix tree to get offsets of BWT permutation
  std::string res;
  for (unsigned i = 0; i < suffixes.size(); i++) {
    int idx = suffixes[i].second - 1;
    idx += idx < 0 ? a.size() : 0;
    res += std::to_string(idx) + " ";
  }

  if (res.size())
    res.pop_back();

  outp << res;

  // Close the input and output files
  inp.close();
  outp.close();
  return 0;
}