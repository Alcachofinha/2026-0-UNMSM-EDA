#include <assert.h>
#include <iostream>
using namespace std;
#include "array.h"

void DemoArray() {
  const int N = 5;
  CArray<int> arr1(N);
  for (auto i = 0; i < N; ++i)
    arr1.push_back(i * 10);
  cout << "First part finished" << endl;
  arr1[3] = 43;
  arr1.push_back(80);
  arr1.push_back(75);

  // test for [] operator with out of bounds index
  arr1[9] = 10;
  arr1[12] = 15;

  cout << "Ready to print" << endl;
  for (auto i = 0; i < arr1.getSize(); ++i)
    cout << "[" << i << "] = " << arr1[i] << endl;
  // cout << arr1[50] << endl; // This will trigger an assertion failure
  cout << "DemoArray finished." << endl;
}
