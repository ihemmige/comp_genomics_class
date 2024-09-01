// Include necessary header files
#include <iostream> // For input/output operations
#include <fstream> // For file handling
#include <string> // For string operations
#include <unordered_map>
#include <map>

int main(int argc, char* argv[]) {
    if (argc != 3) {
      std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
      return 1;
    }
    std::unordered_map<std::string, char> codonTable = {
      {"UUU", 'F'}, {"UUC", 'F'}, {"UUA", 'L'}, {"UUG", 'L'},
      {"UCU", 'S'}, {"UCC", 'S'}, {"UCA", 'S'}, {"UCG", 'S'},
      {"UAU", 'Y'}, {"UAC", 'Y'}, {"UAA", '*'}, {"UAG", '*'},
      {"UGU", 'C'}, {"UGC", 'C'}, {"UGA", '*'}, {"UGG", 'W'},
      {"CUU", 'L'}, {"CUC", 'L'}, {"CUA", 'L'}, {"CUG", 'L'},
      {"CCU", 'P'}, {"CCC", 'P'}, {"CCA", 'P'}, {"CCG", 'P'},
      {"CAU", 'H'}, {"CAC", 'H'}, {"CAA", 'Q'}, {"CAG", 'Q'},
      {"CGU", 'R'}, {"CGC", 'R'}, {"CGA", 'R'}, {"CGG", 'R'},
      {"AUU", 'I'}, {"AUC", 'I'}, {"AUA", 'I'}, {"AUG", 'M'},
      {"ACU", 'T'}, {"ACC", 'T'}, {"ACA", 'T'}, {"ACG", 'T'},
      {"AAU", 'N'}, {"AAC", 'N'}, {"AAA", 'K'}, {"AAG", 'K'},
      {"AGU", 'S'}, {"AGC", 'S'}, {"AGA", 'R'}, {"AGG", 'R'},
      {"GUU", 'V'}, {"GUC", 'V'}, {"GUA", 'V'}, {"GUG", 'V'},
      {"GCU", 'A'}, {"GCC", 'A'}, {"GCA", 'A'}, {"GCG", 'A'},
      {"GAU", 'D'}, {"GAC", 'D'}, {"GAA", 'E'}, {"GAG", 'E'},
      {"GGU", 'G'}, {"GGC", 'G'}, {"GGA", 'G'}, {"GGG", 'G'}
    };

    std::map<char, int> aminoCounts = {
      {'F', 0}, {'L', 0}, {'S', 0}, {'Y', 0},
      {'C', 0}, {'W', 0}, {'P', 0}, {'H', 0},
      {'Q', 0}, {'R', 0}, {'I', 0}, {'M', 0},
      {'T', 0}, {'N', 0}, {'K', 0}, {'V', 0},
      {'A', 0}, {'D', 0}, {'E', 0}, {'G', 0}
    };

    // Assign the input and output filenames from command-line arguments
    std::string input_file = argv[1]; // First argument: input filename
    std::string output_file = argv[2]; // Second argument: output filename
    char c = -1; // Initialize character variable
    
    // Open input file for reading
    std::ifstream inp(input_file);
    // Open output file for writing
    std::ofstream outp(output_file);
    std::string rnaString;

    // Read input file character by character into stringstream
    while(inp.get(c)) {
      // Check if the character is one of ’A’, ’C’, ’G’, or ’U’
      if(c == 'A' || c == 'C' || c == 'G' || c == 'U') {
        // If so, write it to the RNA string
        rnaString += c;
      }
    }
    
    // iterate through RNA string in groups of 3 bases
    for (int i = 0; i < int(rnaString.size()); i += 3) {
      char aminoAcid = codonTable[rnaString.substr(i, 3)];
      if (aminoAcid != '*') aminoCounts[aminoAcid]++;
    }

    std::string result;
    for (auto pair : aminoCounts) {
      result += std::to_string(pair.second) + ',';
    }
    result.pop_back(); // remove trailing comma
    outp << result;

    // Close the input and output files
    inp.close();
    outp.close();
    return 0;
}