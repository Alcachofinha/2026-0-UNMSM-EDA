#ifndef __ARRAY_H__
#define __ARRAY_H__
#include <stddef.h>

using value_type = int;

class CArray{
private:
    size_t  m_size;
    value_type  m_data[100]; // Fixed size for simplicity;
public:
    CArray(size_t size);
    ~CArray();

    value_type& operator[](size_t index);
    size_t  getSize() const;
};

void DemoArray();

#endif // __ARRAY_H__