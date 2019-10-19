#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;

void printVector(vector<int> set) {
  cout << "==================================" << endl;
  for (int i = 0; i < set.size(); i++)
    cout << "[" << i << "]: " << set[i] << endl;
  cout << "==================================" << endl;
}

int main() {
  system("clear");

  // Initiate srand and vector, integers
  srand(time(NULL));
  vector<int> set;
  int start, end;

  // Determine range of integers
  cout << "Starint integer: "; cin >> start;
  cout << "Ending integer: "; cin >> end;

  for (int i = start; i <= end; i ++) set.push_back(i); // Populate vector from start~end
  cout << "Before Shuffle" << endl;
  printVector(set);

  random_shuffle(set.begin(), set.end()); // shuffle the vector

  cout << "After Shuffle" << endl;
  printVector(set);

  ofstream file("p2data.txt"); // open filestream
  for (int num: set) file << num << endl; // write vector integers into file
  file.close(); // close filestream

  return 0;
}
