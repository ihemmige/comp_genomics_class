// Include necessary header files
#include <iostream> // For input/output operations
#include <fstream> // For file handling
#include <string> // For string operations
#include <vector>
#include <tuple>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <sstream>
#include <unordered_set>

void parseGraph(std::ifstream& inp, std::unordered_set<std::string>& nodes, std::unordered_map<std::string, std::pair<int, std::string>>& leftMatches) {
  // parse input and store each read's best match to right
  std::string temp;
  std::unordered_map<std::string, std::pair<std::string, int>> graph;
  while (1) {
    if (!std::getline(inp, temp)) break;

    std::istringstream iss(temp);
    std::string left, right;
    int length;
    iss >> left >> length >> right;

    graph[left] = {right, length};
    nodes.insert(left);
    nodes.insert(right);
  }

  // find each right side's best match to left
  std::unordered_set<std::string> isAmbiguous;
  for (const auto& g : graph) {
    std::string left = g.first, right = g.second.first;
    int length = g.second.second;
    if (leftMatches.find(right) != leftMatches.end()) {
      if (length > leftMatches[right].first) {
        leftMatches[right] = {length, left};
        isAmbiguous.erase(right);
      } else if (length == leftMatches[right].first) {
        isAmbiguous.insert(right);
      }
    } else {
      leftMatches[right] = {length, left};
    }
  }

  // remove reads with ambiguous left matches
  for (auto& s: isAmbiguous) {
    leftMatches.erase(s);
  }

  // if read has incoming edge (from left), it doesn't start a unitig
  for (auto& s : leftMatches) {
    nodes.erase(s.first);
  }
}

void traverseGraph(std::map<std::string, std::vector<std::pair<std::string, int>>>& unitigs, std::unordered_set<std::string>& startNodes, std::unordered_map<std::string, std::pair<int, std::string>>& leftMatches) {
    // convert right->left edges to left->right
  std::unordered_map<std::string, std::pair<int, std::string>> edges;
  for (const auto& edge : leftMatches) {
    edges[edge.second.second] = {edge.second.first, edge.first};
  }

  // explore graph with DFS from each start read
  for (const auto& start : startNodes) {
    std::vector<std::pair<std::string, int>> newVal;
    std::string cur = start;
    while (edges.find(cur) != edges.end()) {
      newVal.push_back({edges[cur].second, edges[cur].first});
      cur = edges[cur].second;
    }
    unitigs[start] = newVal;
  }
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

  std::unordered_set<std::string> startNodes;
  std::unordered_map<std::string, std::pair<int, std::string>> leftMatches;
  parseGraph(inp, startNodes, leftMatches);

  std::map<std::string, std::vector<std::pair<std::string, int>>> unitigs;
  traverseGraph(unitigs, startNodes, leftMatches);

  // convert unitigs to string output
  std::string toPrint;
  for (const auto& path : unitigs) {
    toPrint += path.first + '\n';
    for (const auto& pair : path.second) {
      toPrint += std::to_string(pair.second) + ' ' + pair.first + '\n';
    }
  }

  // remove trailing newline
  if (toPrint.size())
    toPrint.pop_back();

  outp << toPrint;

  // Close the input and output files
  inp.close();
  outp.close();
  return 0;
}