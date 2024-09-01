// Include necessary header files
#include <iostream> // For input/output operations
#include <fstream> // For file handling
#include <string> // For string operations
#include <unordered_map>
#include <algorithm>

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

    // map each amino acid to its optimal codon
    std::unordered_map<char, std::pair<std::string, int>> optimalCodons;
    for (auto pair : codonTable) {
      int cgCount = std::count_if(pair.first.begin(), pair.first.end(), [](char c) {
        return c == 'C' || c == 'G';
      });
      if (optimalCodons.find(pair.second) == optimalCodons.end() || optimalCodons[pair.second].second < cgCount) {
        optimalCodons[pair.second] = {pair.first, cgCount};
      } else if (optimalCodons[pair.second].second == cgCount) {
        optimalCodons[pair.second] = {std::min(pair.first, optimalCodons[pair.second].first), cgCount};
      }
    }

    // Assign the input and output filenames from command-line arguments
    std::string input_file = argv[1]; // First argument: input filename
    std::string output_file = argv[2]; // Second argument: output filename
    
    // Open input file for reading
    std::ifstream inp(input_file);
    // Open output file for writing
    std::ofstream outp(output_file);
    
    // get the amino acid string
    std::string aminoString;
    std::getline(inp, aminoString);

    // get optimal codon for each amino acid
    std::string result;
    for (auto c : aminoString) {
      result += optimalCodons[c].first;
    }

    outp << result; // send RNA string to output

    // Close the input and output files
    inp.close();
    outp.close();
    return 0;
}