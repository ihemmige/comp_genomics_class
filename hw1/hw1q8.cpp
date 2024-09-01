#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <set>
#include <vector>

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
    return 1;
  }

  // Open input file for reading
  std::ifstream input(argv[1]);
  // Open output file for writing
  std::ofstream output(argv[2]);

  // Read the first line from input file
  std::string dnaString;
  std::getline(input, dnaString);

  std::unordered_map<std::string, int> subCounts; // count of each substring
  std::unordered_map<std::string, std::vector<int>> substringIndex; // occurences of each substring

  int maxCount = 0;
  std::unordered_map<std::string, int> substring_count;
  int k = 6;
  for (int i = 0; i < int(dnaString.size()) - 6; i++) {
    std::string curString = dnaString.substr(i, k);
    subCounts[curString]++;
    maxCount = std::max(maxCount, subCounts[curString]);
    substringIndex[curString].push_back(i);
  }

  // find all substrings that occur the max number of times, insert in set (sorted)
  std::set<std::string> substrings;
  for (auto pair : subCounts) {
    if (pair.second == maxCount) substrings.insert(pair.first);
  }

  // only print if there is at least one substring of length 6
  if (substrings.size() > 0) {
    // first element in the set is lexicographically smallest
    std::vector<int> start_indices = substringIndex[*substrings.begin()];
    for (size_t i = 0; i < start_indices.size(); ++i) {
        if (i > 0) output << ','; // Add a comma before each element except the first one
        output << start_indices[i];
    }
  }

  // Close the input and output files
  input.close();
  output.close();
  return 0;
}