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

  // Compare strings and count differences
  int count = 0;
  for (int i = 0; i < int(a.size()); i++) {
    count += a[i] != b[i];
  }

  // Note: No endl is used to avoid a trailing newline
  output << count; // output the number of differences

  // Close the input and output files
  input.close();
  output.close();
  return 0;
}