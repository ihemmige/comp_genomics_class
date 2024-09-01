// Include necessary header files
#include <iostream> // For input/output operations
#include <fstream> // For file handling
#include <string> // For string operations
#include <unordered_map>
#include <sstream>

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
    return 1;
  }

  // maps each base to RNA
  std::unordered_map<char, char> rnaBases = {
    {'A', 'A'},
    {'T', 'U'},
    {'G', 'G'},
    {'C', 'C'},
  };
  
  // Assign the input and output filenames from command-line arguments
  std::string input_file = argv[1]; // First argument: input filename
  std::string output_file = argv[2]; // Second argument: output filename
  char c = -1; // Initialize character variable
  
  // Open input file for reading
  std::ifstream inp(input_file);
  // Open output file for writing
  std::ofstream outp(output_file);

  // add dash on multiples after every 3 bases
  int ptr = 0;
  // Read input file character by character into stringstream
  while(inp.get(c)) {
    // Check if the character is one of ’A’, ’C’, ’G’, or ’T’
    if(c == 'A' || c == 'C' || c == 'G' || c == 'T') {
      // If so, write it to the output file
      if (ptr == 3) {
        outp << "-";
        ptr = 0;
      }
      outp << rnaBases[c];
      ptr++;
    }
  }

  // No endl at the end to avoid trailing newline
  // Close the input and output files
  inp.close();
  outp.close();
  return 0;
}