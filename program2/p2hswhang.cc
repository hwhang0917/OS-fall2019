#include <iostream>
#include <fstream>
#include <typeinfo>
#include <pthread.h>

using namespace std;

// Maximum number of integers in the list
#define MAX 20

int unsorted[MAX-1]; // Global array that is shared between two sorting threads
                // each thread will work on half of the array

int sorted[MAX-1]; // Global array that is shared between two sorting threads
                // to store their own sorted subarrays here

typedef int int_type;

// ================ UTILITY FUNCTIONS ================
int readData() {
  // Function to read data from "p2data.txt"
  // Check if there are even number of integers in the file & does not exceed MAX
  // Returns:
  //        number of integers if suceeds
  //        -1 if there is an error
  ifstream file("p2data.txt"); // filename "p2data.txt" is hard coded & open file
  int num, element_count = 0, index = 0;

  if (file.is_open()) {
    while (file >> num) {
      if (typeid(num) != typeid(int_type)) { // data type check
        cout << "ERROR [INVALID DATA TYPE]: Only integers should be stored in p2data.txt!!" << endl;
        return -1;
      }
      if(MAX <= element_count++) { // MAX count check
        cout << "* Note: Maximum of 20 elements or less is assumed in the assignment. " << endl;
        cout << "ERROR [ARRAY OVERFLOW]: There cannot be more than 20 elements!!" << endl;
        return -1;
      }
      unsorted[index++] = num; // populate global unsorted with integers from p2data.txt
    }
  } else { // file open check
    cout << "ERROR [FILE OPEN FAIL]: file p2data.txt failed to open!!" << endl;
    return -1;
  }

  if (element_count % 2 != 0) { // Even number of element check
    cout << "* Note: even number of element is assumed in the assignment." << endl;
    cout << "ERROR [ELEMENT COUNT ODD]: file contains odd number of elements!!" << endl;
    return -1;
  }

  return element_count; // successfully read all integers from p2data.txt
}

// ================ PTHREAD MERGE / SORT FUNCTIONS ================
void* sort(void *rangePtr) {
  // Sort half of the array with given range array
  // Paramter:
  //          rangePtr (pointer to array of rangeA)
  cout << "~~~~~ SORT THREAD START" << endl;
  int *range = (int *) rangePtr; // Retrieve range array
  int min_idx; // minimum index

  // DO SELECTION SORT
  for (int i = range[0]; i < range[1]; i++) { // Iterate through range
    min_idx = i;
    // cout << "THREAD from range[" << range[0] << "]: min:" << min_idx << endl;
    for (int j = i + 1; j < range[1]; j++)
      if (unsorted[j] < unsorted[min_idx]) min_idx = j;

    swap(unsorted[min_idx], unsorted[i]); // swap until sorted
  }
  cout << "***** SORT THREAD END" << endl;
}

void* merge(void *sizePtr) {
  int size = (*(int*) sizePtr);
  int min_idx;
  cout << "~~~~~ MERGE THREAD START" << endl;
  // DO SELECTION SORT
  for (int i = 0; i < size; i++) { // Iterate through range
    min_idx = i;

    for (int j = i + 1; j < size; j++)
      if (unsorted[j] < unsorted[min_idx]) min_idx = j;

    swap(unsorted[min_idx], unsorted[i]); // swap until sorted
  }

  for (int i = 0; i < size; i++) // copy sorted array
    sorted[i] = unsorted[i];
  cout << "***** MERGE THREAD END" << endl;
}

int main() {
  int size = readData();
  if (size == -1) return 0; // Error exit

  cout << ":::::::::::::::::::UNSORTED::::::::::::::::" << endl;
  cout << "Array Size: " << size << endl;
  for (int i = 0; i < size; i++)
    cout << unsorted[i] << " ";
  cout << endl;
  cout << ":::::::::::::::::::::::::::::::::::::::::::" << endl;

  pthread_t sortThreadA, sortThreadB, mergeThread; // Three thread initialize
  int rangeA[2] = {0, ((size/2))};
  int rangeB[2] = {(size / 2), size};

  cout << "Range of Sort A thread: [" << rangeA[0] << "] ~ [" << rangeA[1] << "]" << endl;
  cout << "Range of Sort B thread: [" << rangeB[0] << "] ~ [" << rangeB[1] << "]" << endl;
  cout << endl << endl;

  // Create two sorting arrays
  pthread_create(&sortThreadA, NULL, sort, (void*)rangeA);
  pthread_create(&sortThreadB, NULL, sort, (void*)rangeB);

  // main thread waits for sorting threads finishes
  pthread_join(sortThreadA, NULL);
  pthread_join(sortThreadB, NULL);

  cout << endl << endl;
  // Print half sorted array
  cout << "::::::::::::::::HALF SORTED::::::::::::::::" << endl;
  cout << "Array Size: " << size << endl;
  for (int i = 0; i < size; i++) {
    if (i == (size/2))  cout << " || ";
    cout << unsorted[i] << " ";
  }
  cout << endl;
  cout << ":::::::::::::::::::::::::::::::::::::::::::" << endl;

  // When sorting threads are done, main thread create merging thread
  pthread_create(&mergeThread, NULL, merge, (void*)&size);
  pthread_join(mergeThread, NULL); // waits for merge to finish
  cout << endl << endl;

  // Print sorted array
  cout << "::::::::::::::::::::SORTED:::::::::::::::::" << endl;
  cout << "Array Size: " << size << endl;
  for (int i = 0; i < size; i++)
    cout << sorted[i] << " ";
  cout << endl;
  cout << ":::::::::::::::::::::::::::::::::::::::::::" << endl;

  return 0;
}
