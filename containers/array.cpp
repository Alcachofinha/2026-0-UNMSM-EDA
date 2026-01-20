#include <iostream>
using namespace std;
#include "array.h"

CArray::CArray(size_t size){
    m_size = size;
    // m_data = new int[size];
}
CArray::~CArray(){
    // delete[] m_data;
}

value_type& CArray::operator[](size_t index){
    return m_data[index];
}

size_t CArray::getSize() const {
    return m_size;
}


void DemoArray(){
    CArray arr1(10);
    for(auto i = 0; i < arr1.getSize(); ++i)
        arr1[i] = i * 10;

    for(auto i = 0; i < arr1.getSize(); ++i)
        cout << arr1[i] << endl;
}