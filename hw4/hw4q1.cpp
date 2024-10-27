// Include necessary header files
#include <iostream> // For input/output operations
#include <fstream> // For file handling
#include <string> // For string operations
#include <vector>
#include <algorithm> // for max

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
  std::string t, p;
  std::getline(inp, t);
  std::getline(inp, p);

  int m = t.size(), n = p.size();

  // DP matrix
  std::vector<std::vector<int>> dp(m+1, std::vector<int>(n+1, 0));

  // use bottom-up DP
  for (int i = 1; i <= m; i++) {
    for (int j = 1; j <= n; j++) {
      if (t[i-1] == p[j-1]) { // characters match
        dp[i][j] = dp[i-1][j-1] + 1;
      } else { // characters mismatch
        dp[i][j] = std::max(dp[i-1][j], dp[i][j-1]);
      }
    }
  }
  
  outp << dp[m][n]; // write to output

  // Close the input and output files
  inp.close();
  outp.close();
  return 0;
}