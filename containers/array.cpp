#include <iostream>
using namespace std;
#include "array.h"

CArray::CArray(int size){
    m_size = size;
    // m_data = new int[size];
}
CArray::~CArray(){
    // delete[] m_data;
}

int& CArray::operator[](int index){
    // return m_data[index];
    static int dummy = 0;
    return dummy;
}

int CArray::getSize() const {
    return m_size;
}


void DemoArray(){
    CArray arr1(5);
    for(int i = 0; i < arr1.getSize(); ++i)
        arr1[i] = i * 10;

    for(int i = 0; i < arr1.getSize(); ++i)
        cout << arr1[i] << endl;
    
}