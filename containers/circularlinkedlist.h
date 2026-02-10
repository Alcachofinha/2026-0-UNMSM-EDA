#ifndef __CIRCULARLINKEDLIST_H__
#define __CIRCULARLINKEDLIST_H__

#include <iostream>
#include <functional>
#include <stdexcept>
#include <iomanip>
#include <type_traits>
#include <utility>
#include <string>

#include "list_traits.h"
#include "../general/types.h"
#include "../util.h"

using namespace std;

// TODO: Traits para listas enlazadas
// ==========================
// Nodo para lista circular
// ==========================
template <typename Traits>
class NodeCircularLinkedList{

    using value_type = typename Traits::value_type;
    using Node       = NodeCircularLinkedList<Traits>;

private:
    value_type m_data;
    ref_type   m_ref;
    Node      *m_pNext = nullptr;

public:
    NodeCircularLinkedList(){}
    NodeCircularLinkedList(value_type _value, ref_type _ref = -1)
        : m_data(_value), m_ref(_ref){}

    value_type  GetValue() const { return m_data; }

    // IMPORTANTE: 2 overloads (uno para no-const, otro para const)
    value_type       &GetValueRef()       { return m_data; }
    const value_type &GetValueRef() const { return m_data; }

    ref_type   GetRef() const { return m_ref; }
    ref_type  &GetRefRef()    { return m_ref; }

    Node       *GetNext()       { return m_pNext; }
    const Node *GetNext() const { return m_pNext; }

    Node      *&GetNextRef() { return m_pNext; }

    Node &operator=(const Node &another){
        m_data = another.GetValue();
        m_ref  = another.GetRef();
        return *this;
    }

    bool operator==(const Node &another) const
    { return m_data == another.GetValue(); }

    bool operator<(const Node &another) const
    { return m_data < another.GetValue(); }
};

// ========================================
// Iterators (Forward) para lista circular
// - end() se representa con nullptr
// - operator++ corta cuando regresa al start
// ========================================
template <typename Container>
class CircularLinkedListIterator {
public:
    using raw_container = std::remove_const_t<Container>;
    using value_type    = typename raw_container::value_type;
    using Node          = typename raw_container::Node;

    using node_ptr  = std::conditional_t<std::is_const_v<Container>, const Node*, Node*>;
    using reference = std::conditional_t<std::is_const_v<Container>, const value_type&, value_type&>;

protected:
    Container *m_pContainer = nullptr;
    node_ptr   m_pStart     = nullptr;   // para detectar el retorno al inicio
    node_ptr   m_pNode      = nullptr;   // nullptr = end()

public:
    CircularLinkedListIterator(Container *pContainer=nullptr, node_ptr pStart=nullptr, node_ptr pNode=nullptr)
        : m_pContainer(pContainer), m_pStart(pStart), m_pNode(pNode) {}

    reference operator*() const { return m_pNode->GetValueRef(); }

    bool operator!=(const CircularLinkedListIterator &other) const { return m_pNode != other.m_pNode; }
    bool operator==(const CircularLinkedListIterator &other) const { return m_pNode == other.m_pNode; }
};

template <typename Container>
class CircularLinkedListForwardIterator : public CircularLinkedListIterator<Container>
{
    using Parent = CircularLinkedListIterator<Container>;
public:
    using Node     = typename Parent::Node;
    using node_ptr = typename Parent::node_ptr;

    CircularLinkedListForwardIterator(Container *pContainer=nullptr, node_ptr pStart=nullptr, node_ptr pNode=nullptr)
        : Parent(pContainer, pStart, pNode) {}

    CircularLinkedListForwardIterator &operator++(){
        if(!Parent::m_pNode) return *this;

        node_ptr pNext = Parent::m_pNode->GetNext();

        // Si el siguiente regresa al inicio, terminamos (end = nullptr)
        if(pNext == Parent::m_pStart){
            Parent::m_pNode = nullptr;
        }else{
            Parent::m_pNode = pNext;
        }
        return *this;
    }
};

// ==========================
// Lista Circular
// ==========================
template <typename Traits>
class CCircularLinkedList {

public:
    using value_type            = typename Traits::value_type;
    using Node                  = NodeCircularLinkedList<Traits>;
    using forward_iterator      = CircularLinkedListForwardIterator< CCircularLinkedList<Traits> >;
    using const_forward_iterator= CircularLinkedListForwardIterator< const CCircularLinkedList<Traits> >;

private:
    Node   *m_pRoot     = nullptr;
    Node   *m_pLast     = nullptr;
    size_t  m_nElements = 0;

public:
    CCircularLinkedList(){}
    CCircularLinkedList(const CCircularLinkedList&);
    CCircularLinkedList(CCircularLinkedList&&) noexcept;
    virtual ~CCircularLinkedList();

    void clear();

    CCircularLinkedList& operator=(const CCircularLinkedList&);
    CCircularLinkedList& operator=(CCircularLinkedList&& other) noexcept;

    value_type& operator[](size_t index);
    const value_type& operator[](size_t index) const;

    void push_back(const value_type &val, ref_type ref);
    void Insert(const value_type &val, ref_type ref);

    size_t getSize() const { return m_nElements; }

    forward_iterator begin() {
        return forward_iterator(this, m_pRoot, m_pRoot);
    }
    forward_iterator end() {
        return forward_iterator(this, m_pRoot, nullptr);
    }

    const_forward_iterator begin() const {
        return const_forward_iterator(this, m_pRoot, m_pRoot);
    }
    const_forward_iterator end() const {
        return const_forward_iterator(this, m_pRoot, nullptr);
    }

    template <typename ObjFunc, typename... Args>
    void Foreach(ObjFunc of, Args... args);

    template <typename ObjFunc, typename... Args>
    auto FirstThat(ObjFunc of, Args... args){
        return ::FirstThat(*this, of, args...);
    }

private:
    // Insert ordenado (iterativo) para evitar recursión infinita en circular
    void InternalInsert(const value_type &val, ref_type ref);

    // TODO: Persistencia (write)
    friend ostream &operator<<(ostream &os, const CCircularLinkedList<Traits> &container){
        os << "[";
        Node *pNode = container.m_pRoot;

        for(size_t i=0; i < container.m_nElements; ++i){
            os << "(" << pNode->GetValue() << ":" << pNode->GetRef() << ")";
            pNode = pNode->GetNext();
        }

        os << "]" << endl;
        return os;
    }

    // TODO: Persistencia (read)
    template <typename T>
    friend istream &operator>>(istream &is, CCircularLinkedList<T> &container);
};

// ==========================
// Implementación
// ==========================
template <typename Traits>
void CCircularLinkedList<Traits>::push_back(const value_type &val, ref_type ref){
    Node *pNew = new Node(val, ref);

    if(!m_pRoot){
        m_pRoot = m_pLast = pNew;
        pNew->GetNextRef() = pNew;   // circular (self-loop)
    }else{
        m_pLast->GetNextRef() = pNew;
        pNew->GetNextRef()    = m_pRoot;
        m_pLast               = pNew;
    }
    ++m_nElements;
}

template <typename Traits>
void CCircularLinkedList<Traits>::InternalInsert(const value_type &val, ref_type ref){
    typename Traits::Func cmp;

    // Caso vacío
    if(!m_pRoot){
        push_back(val, ref);
        return;
    }

    // Insertar antes del root (nuevo mínimo para Asc, o nuevo máximo para Desc)
    if(cmp(m_pRoot->GetValue(), val)){
        Node *pNew = new Node(val, ref);
        pNew->GetNextRef() = m_pRoot;
        m_pRoot = pNew;
        m_pLast->GetNextRef() = m_pRoot;
        ++m_nElements;
        return;
    }

    // Buscar posición: prev -> cur, deteniendo cuando cmp(cur, val) sea true
    Node *prev = m_pRoot;
    Node *cur  = m_pRoot->GetNext();

    while(cur != m_pRoot && !cmp(cur->GetValue(), val)){
        prev = cur;
        cur  = cur->GetNext();
    }

    Node *pNew = new Node(val, ref);
    prev->GetNextRef() = pNew;
    pNew->GetNextRef() = cur;

    // Si insertamos al final (prev era last), actualizar last
    if(prev == m_pLast){
        m_pLast = pNew;
    }

    ++m_nElements;
}

template <typename Traits>
void CCircularLinkedList<Traits>::Insert(const value_type &val, ref_type ref){
    InternalInsert(val, ref);
}

template <typename Traits>
void CCircularLinkedList<Traits>::clear(){
    if(!m_pRoot){
        m_pLast = nullptr;
        m_nElements = 0;
        return;
    }

    Node *pNode = m_pRoot;
    for(size_t i=0; i < m_nElements; ++i){
        Node *pNext = pNode->GetNext();
        delete pNode;
        pNode = pNext;
    }

    m_pRoot = nullptr;
    m_pLast = nullptr;
    m_nElements = 0;
}

template <typename Traits>
CCircularLinkedList<Traits>::~CCircularLinkedList(){
    clear();
}

template <typename Traits>
CCircularLinkedList<Traits>::CCircularLinkedList(const CCircularLinkedList &other)
    : m_pRoot(nullptr), m_pLast(nullptr), m_nElements(0)
{
    Node *pNode = other.m_pRoot;
    for(size_t i=0; i < other.m_nElements; ++i){
        push_back(pNode->GetValue(), pNode->GetRef());
        pNode = pNode->GetNext();
    }
}

template <typename Traits>
CCircularLinkedList<Traits>& CCircularLinkedList<Traits>::operator=(const CCircularLinkedList &other){
    if(this == &other) return *this;
    clear();

    Node *pNode = other.m_pRoot;
    for(size_t i=0; i < other.m_nElements; ++i){
        push_back(pNode->GetValue(), pNode->GetRef());
        pNode = pNode->GetNext();
    }
    return *this;
}

template <typename Traits>
CCircularLinkedList<Traits>::CCircularLinkedList(CCircularLinkedList &&other) noexcept
    : m_pRoot(other.m_pRoot), m_pLast(other.m_pLast), m_nElements(other.m_nElements)
{
    other.m_pRoot = nullptr;
    other.m_pLast = nullptr;
    other.m_nElements = 0;
}

template <typename Traits>
CCircularLinkedList<Traits>& CCircularLinkedList<Traits>::operator=(CCircularLinkedList&& other) noexcept{
    if(this == &other) return *this;

    clear();

    m_pRoot     = other.m_pRoot;
    m_pLast     = other.m_pLast;
    m_nElements = other.m_nElements;

    other.m_pRoot = nullptr;
    other.m_pLast = nullptr;
    other.m_nElements = 0;

    return *this;
}

template <typename Traits>
typename CCircularLinkedList<Traits>::value_type&
CCircularLinkedList<Traits>::operator[](size_t index){
    if(index >= m_nElements)
        throw std::out_of_range("Index out of range");

    Node *pNode = m_pRoot;
    for(size_t i=0; i < index; ++i)
        pNode = pNode->GetNext();

    return pNode->GetValueRef();
}

template <typename Traits>
const typename CCircularLinkedList<Traits>::value_type&
CCircularLinkedList<Traits>::operator[](size_t index) const{
    if(index >= m_nElements)
        throw std::out_of_range("Index out of range");

    Node *pNode = m_pRoot;
    for(size_t i=0; i < index; ++i)
        pNode = pNode->GetNext();

    return pNode->GetValueRef();
}

template <typename Traits>
template <typename ObjFunc, typename... Args>
void CCircularLinkedList<Traits>::Foreach(ObjFunc of, Args... args){
    Node *p = m_pRoot;
    for(size_t i=0; i < m_nElements; ++i){
        of(p->GetValueRef(), args...);
        p = p->GetNext();
    }
}

template <typename Traits>
istream &operator>>(istream &is, CCircularLinkedList<Traits> &container){
    using value_type = typename CCircularLinkedList<Traits>::value_type;

    if(!is) return is;

    CCircularLinkedList<Traits> tmp;

    char ch;
    do{
        if(!is.get(ch)) return is;
    } while(ch != '[');

    while(true){
        is >> ws;

        int pk = is.peek();
        if(pk == EOF){
            is.setstate(ios::failbit);
            return is;
        }

        if(static_cast<char>(pk) == ']'){
            is.get(ch);
            break;
        }

        is >> ch;
        if(!is || ch != '('){
            is.setstate(ios::failbit);
            return is;
        }

        value_type val{};
        ref_type ref{};

        if constexpr (std::is_same_v<value_type, std::string>){
            is >> std::quoted(val);
        }else{
            is >> val;
        }

        is >> ch;
        if(!is || ch != ':'){
            is.setstate(ios::failbit);
            return is;
        }

        is >> ref;

        is >> ch;
        if(!is || ch != ')'){
            is.setstate(ios::failbit);
            return is;
        }

        tmp.Insert(val, ref);
    }

    container = std::move(tmp);
    return is;
}

#endif // __CIRCULARLINKEDLIST_H__
