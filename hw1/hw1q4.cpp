// Include necessary header files
#include <iostream> // For input/output operations
#include <fstream> // For file handling
#include <string> // For string operations
#include <unordered_map>
#include <vector>
#include <algorithm>

// recursive function makes all possible DNA sequences of given length
void generatePossible(unsigned length, std::string& curString, std::vector<std::string>& possible, std::vector<char>& bases) {
  if (curString.size() == length) {
    possible.push_back(curString);
    return;
  }
  for (auto b : bases) {
    curString.push_back(b);
    generatePossible(length, curString, possible, bases);
    curString.pop_back();
  }
}

// generates all DNA strings of given length equal to their own reverse compliments
std::vector<std::string> generatePalindromes(int n) {
  if (n % 2 == 1) return {}; // there are no odd-length strings equal to their reverse complements

  // make all possible strings of half desired length
  std::vector<char> bases = {'A', 'T', 'G', 'C'};
  std::vector<std::string> halfStrings;
  std::string curString;
  generatePossible(n/2, curString, halfStrings, bases);

  // maps each base to its complement
  std::unordered_map<char, char> complement = {
    {'A', 'T'},
    {'T', 'A'},
    {'G', 'C'},
    {'C', 'G'},
  };  

  std::vector<std::string> result;
  for (auto h : halfStrings) {
    // generate second half of string as reverse complement of first half
    std::string backHalf;
    std::transform(h.rbegin(), h.rend(), std::back_inserter(backHalf),
                [&complement](char base) { return complement[base]; });
    result.push_back(h + backHalf);
  }
  return result;
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
    return 1;
  }
  
  // Assign the input and output filenames from command-line arguments
  std::string input_file = argv[1]; // First argument: input filename
  std::string output_file = argv[2]; // Second argument: output filename
  char c = -1; // Initialize character variable
  
  // Open input file for reading
  std::ifstream inp(input_file);
  // Open output file for writing
  std::ofstream outp(output_file);

  inp.get(c);
  int n = std::stoi(std::string(1, c));
  std::vector<std::string> palindromes;

  // for all lengths up to and including n, generate the palindromes
  for (int i = 1; i <= n; i++) {
    std::vector<std::string> newStrings = generatePalindromes(i);
    palindromes.insert(palindromes.end(), std::make_move_iterator(newStrings.begin()), std::make_move_iterator(newStrings.end()));
  }

  std::sort(palindromes.begin(), palindromes.end()); // sort alphabetically
  std::string result;
  for (auto p : palindromes) {
    result += p + '\n';
  }
  result.pop_back(); // remove trailing newline
  outp << result; // send to output file

  // Close the input and output files
  inp.close();
  outp.close();
  return 0;
}