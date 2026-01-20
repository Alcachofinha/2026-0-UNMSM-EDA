#ifndef __ARRAY_H__
#define __ARRAY_H__

class CArray{
private:
    int  m_size;
    // int* m_data;
public:
    CArray(int size);
    ~CArray();

    int& operator[](int index);
    int  getSize() const;
};

void DemoArray();

#endif // __ARRAY_H__