#ifndef __ARRAY_H__
#define __ARRAY_H__
#include <stddef.h>



template <typename T>
class CArray {
  using value_type = T;
private:
  size_t m_capacity = 0, m_last = 0;
  value_type *m_data = nullptr;

public:
  CArray(size_t size);
  ~CArray();

  void push_back(value_type value);
  value_type &operator[](size_t index);
  size_t getSize() const
  {   return m_last + 1;  };
  void resize(size_t delta = 10);
};

template <typename T>
CArray<T>::CArray(size_t size) {
  m_capacity = size;
  m_data = new value_type[size];
}
template <typename T>
CArray<T>::~CArray() { delete[] m_data; }

template <typename T>
typename CArray<T>::value_type &CArray<T>::operator[](size_t index) {
    // cout << "XResizing from " << m_capacity << " to at least " << index + 5 << endl;
    if (index > m_capacity) {
      cout << "Resizing from " << m_capacity << " to at least " << index + 5 << endl;
      resize(index - m_last + 5);
    }

    assert(index < m_capacity);

    if (index > m_last)
      m_last = index;
    return m_data[index];
}

template <typename T>
void CArray<T>::push_back(value_type value) {
    if (m_last >= m_capacity)
      resize();
    m_data[m_last++] = value;
}

template <typename T>
void CArray<T>::resize(size_t delta) {
    size_t new_capacity = m_capacity + delta;
    value_type *new_data = new value_type[new_capacity];
    for (auto i = 0; i < m_capacity; ++i)
      new_data[i] = m_data[i];
    delete[] m_data;
    m_data = new_data;
    m_capacity = new_capacity;
}

void DemoArray();

#endif // __ARRAY_H__
