#include <iostream>
#include <fstream>
#include <string>
int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
    return 1;
  }
  
  // Open input file for reading
  std::ifstream input(argv[1]);
  // Open output file for writing
  std::ofstream output(argv[2]);

  std::string a, b; // Declare strings to store the two lines
  
  // Read the first line from input file
  std::getline(input, a);
  // Read the second line from input file
  std::getline(input, b);

  // Compare strings and write result to output file
  // If a equals b, write "Equal", otherwise write "Unequal"
  // Note: No endl is used to avoid a trailing newline
  output << (a == b ? "Equal" : "Unequal");

  // Close the input and output files
  input.close();
  output.close();
  return 0;
}