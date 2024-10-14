// Include necessary header files
#include <iostream> // For input/output operations
#include <fstream> // For file handling
#include <string> // For string operations
#include <vector>
#include <algorithm>

std::pair<int, int> countRunsAndLongestRun(const std::string& s) {
    if (s.empty()) return {0, 0};  // empty string case

    int numRuns = 1;
    int currentRunLength = 1;
    int longestRun = 0;
    char prev = s[0];

    for (unsigned i = 1; i < s.size(); i++) {
      if (s[i] == prev) {
        currentRunLength++;
      } else {
        longestRun = std::max(currentRunLength, longestRun);
        // new run
        numRuns++;
        currentRunLength = 1;
        prev = s[i];
      }
    }

    // check last run
    longestRun = std::max(currentRunLength, longestRun);

    return {numRuns, longestRun};
}


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

  // generate BWT
  std::string bwt;
  for (unsigned i = 0; i < suffixes.size(); i++) {
    int idx = suffixes[i].second - 1;
    idx += idx < 0 ? a.size() : 0;
    bwt.push_back(a[idx]);
  }

  std::pair<int, int> runs = countRunsAndLongestRun(bwt);
  outp << bwt << std::endl;
  outp << std::to_string(a.size()) << ':' << std::to_string(runs.first) << std::endl;
  outp << std::to_string(runs.second);

  // Close the input and output files
  inp.close();
  outp.close();
  return 0;
}