#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

#include <iostream>
#include <functional>
#include <stdexcept>
#include <iomanip>
#include <type_traits>
#include "../general/types.h"
#include "list_traits.h"
#include "../util.h"
using namespace std;

// TODO: Traits para listas enlazadas

template <typename Traits>
class NodeLinkedList{

    using  value_type  = typename Traits::value_type;
    using  Node        = NodeLinkedList<Traits>;
private:
    value_type m_data;
    ref_type   m_ref;
    Node *m_pNext = nullptr;

public:
    NodeLinkedList(){}
    NodeLinkedList( value_type _value, ref_type _ref = -1)
        : m_data(_value), m_ref(_ref){   }

    value_type  GetValue   () const { return m_data; }

    // IMPORTANTE: 2 overloads (uno para no-const, otro para const)
    value_type       &GetValueRef()       { return m_data; }
    const value_type &GetValueRef() const { return m_data; }

    ref_type    GetRef     () const { return m_ref;   }
    ref_type   &GetRefRef  () { return m_ref;   }

    Node      * GetNext     () const { return m_pNext;   }
    Node      *&GetNextRef  () { return m_pNext;   }

    Node &operator=(const Node &another){
        m_data = another.GetValue();
        m_ref   = another.GetRef();
        return *this;
    }

    bool operator==(const Node &another) const
    { return m_data == another.GetValue();   }

    bool operator<(const Node &another) const
    { return m_data < another.GetValue();   }
};

// Iterators para listas enlazadas
// (Forward Iterator lineal: avanza con Next hasta nullptr)

template <typename Container>
class LinkedListIterator {
public:
    using raw_container = std::remove_const_t<Container>;
    using value_type = typename raw_container::value_type;
    using Node       = typename raw_container::Node;

    using node_ptr   = std::conditional_t<std::is_const_v<Container>, const Node*, Node*>;
    using reference  = std::conditional_t<std::is_const_v<Container>, const value_type&, value_type&>;

protected:
    Container *m_pContainer = nullptr;
    node_ptr   m_pNode      = nullptr;

public:
    LinkedListIterator(Container *pContainer=nullptr, node_ptr pNode=nullptr)
        : m_pContainer(pContainer), m_pNode(pNode) {}

    reference operator*() const { return m_pNode->GetValueRef(); }

    bool operator!=(const LinkedListIterator &other) const { return m_pNode != other.m_pNode; }
    bool operator==(const LinkedListIterator &other) const { return m_pNode == other.m_pNode; }
};


template <typename Container>
class LinkedListForwardIterator : public LinkedListIterator<Container>
{
    using Parent = LinkedListIterator<Container>;
public:
    using Node = typename Parent::Node;

    LinkedListForwardIterator(Container *pContainer=nullptr, Node *pNode=nullptr)
        : Parent(pContainer, pNode) {}

    LinkedListForwardIterator &operator++(){
        if (Parent::m_pNode){
            Parent::m_pNode = Parent::m_pNode->GetNext();
        }
        return *this;
    }
};

template <typename Traits>
class CLinkedList {

public: 
    using  value_type  = typename Traits::value_type;
    using  Node        = NodeLinkedList<Traits>;
    using  forward_iterator  = LinkedListForwardIterator< CLinkedList<Traits> >;
    using const_forward_iterator = LinkedListForwardIterator<const CLinkedList<Traits>>;

private:
    Node *m_pRoot = nullptr;
    Node *m_pLast = nullptr;
    size_t m_nElements = 0;

public:
    CLinkedList(){}
    CLinkedList(const CLinkedList&);
    CLinkedList(CLinkedList&&) noexcept;
    virtual ~CLinkedList();

    // TODO: Constructor copia
    // TODO: Move Constructor
    // TODO: Destructor seguro y virtual
    // TODO: Concurrencia (mutex)
    // TODO: Iterators begin() end()
    // TODO: Operadores de acceso []

    void clear();
    CLinkedList& operator=(const CLinkedList&);
    CLinkedList& operator=(CLinkedList&& other) noexcept;
    
    value_type& operator[](size_t index);
    const value_type& operator[](size_t index) const;


    void push_back(const value_type &val, ref_type ref);
    void Insert(const value_type &val, ref_type ref);
    size_t getSize() const { return m_nElements;  }

    forward_iterator begin() { return forward_iterator(this, m_pRoot); }
    forward_iterator end()   { return forward_iterator(this, nullptr); }

    const_forward_iterator begin() const { return const_forward_iterator(this, m_pRoot); }
    const_forward_iterator end()   const { return const_forward_iterator(this, nullptr); }

    template <typename ObjFunc, typename... Args>
    void Foreach(ObjFunc of, Args... args);
    
    template <typename ObjFunc, typename ...Args>
    auto FirstThat(ObjFunc of, Args... args){
        return ::FirstThat(*this, of, args...);
    }
private:
    void InternalInsert(Node *&rParent, const value_type &val, ref_type ref);

    // TODO: Persistencia (write)
    friend ostream &operator<<(ostream &os, const CLinkedList<Traits> &container){
        Node *pNode = container.m_pRoot;
        os << "[";
        while(pNode != nullptr){
            os<< "(" << pNode->GetValue() << ":" << pNode->GetRef() << ")";
            pNode = pNode->GetNext();
        }
        os << "]" << endl;
        return os;
    }
    // TODO: Persistencia (read)
    template <typename T>
    friend istream &operator>>(istream &is, CLinkedList<T> &container);


    // TODO: Persistencia (read)
};

template <typename Traits>
void CLinkedList<Traits>::push_back(const value_type &val, ref_type ref){
    Node *pNewNode = new Node(val, ref);
    if( !m_pRoot ){
        m_pRoot = pNewNode;
    } else {
        m_pLast->GetNextRef() = pNewNode;
    }
    m_pLast = pNewNode;
    ++m_nElements;
}

template <typename Traits>
void CLinkedList<Traits>::InternalInsert(Node *&rParent, const value_type &val, ref_type ref){
    typename Traits::Func cmp;

    if( !rParent || cmp(rParent->GetValue(), val) ){
        Node *pNew = new Node(val, ref);
        pNew->GetNextRef() = rParent;
        rParent = pNew;

        if(pNew->GetNext() == nullptr){
            m_pLast = pNew;
        }
        ++m_nElements;
        return;
    }
    InternalInsert(rParent->GetNextRef(), val, ref);
}

template <typename Traits>
void CLinkedList<Traits>::Insert(const value_type &val, ref_type ref){
    InternalInsert(m_pRoot, val, ref);
}

template <typename Traits>
void CLinkedList<Traits>::clear(){
    Node *pNode = m_pRoot;
    while(pNode != nullptr){
        Node *pNext = pNode->GetNext();
        delete pNode;
        pNode = pNext;
    }
    m_pRoot = nullptr;
    m_pLast = nullptr;
    m_nElements = 0;
}

template <typename Traits>
CLinkedList<Traits>::~CLinkedList(){
    clear();
}

template <typename Traits>
CLinkedList<Traits>::CLinkedList(const CLinkedList &other)
    : m_pRoot(nullptr), m_pLast(nullptr), m_nElements(0)
{
    Node *pNode = other.m_pRoot;
    while(pNode != nullptr){
        this->push_back(pNode->GetValue(), pNode->GetRef());
        pNode = pNode->GetNext();
    }
}

template <typename Traits>
CLinkedList<Traits>& CLinkedList<Traits>::operator=(const CLinkedList &other){
    if(this == &other) return *this;
    this->clear();

    Node *pNode = other.m_pRoot;
    while(pNode != nullptr){
        this->push_back(pNode->GetValue(), pNode->GetRef());
        pNode = pNode->GetNext();
    }
    return *this;
}

template <typename Traits>
CLinkedList<Traits>::CLinkedList(CLinkedList &&other) noexcept
    : m_pRoot(other.m_pRoot), m_pLast(other.m_pLast), m_nElements(other.m_nElements)
{
    other.m_pRoot = nullptr;
    other.m_pLast = nullptr;
    other.m_nElements = 0;
}

template <typename Traits>
CLinkedList<Traits>& CLinkedList<Traits>::operator=(CLinkedList&& other) noexcept {
    if (this == &other) return *this;

    clear();

    m_pRoot      = other.m_pRoot;
    m_pLast      = other.m_pLast;
    m_nElements  = other.m_nElements;

    other.m_pRoot     = nullptr;
    other.m_pLast     = nullptr;
    other.m_nElements = 0;

    return *this;
}

template <typename Traits>
typename CLinkedList<Traits>::value_type& CLinkedList<Traits>::operator[](size_t index){
    if(index >= m_nElements)
        throw std::out_of_range("Index out of range");

    Node *pNode = m_pRoot;
    for(size_t i = 0; i < index; ++i){
        pNode = pNode->GetNext();
    }
    return pNode->GetValueRef();
}

template <typename Traits>
const typename CLinkedList<Traits>::value_type& CLinkedList<Traits>::operator[](size_t index) const{
    if(index >= m_nElements)
        throw std::out_of_range("Index out of range");

    Node *pNode = m_pRoot;
    for(size_t i = 0; i < index; ++i)
        pNode = pNode->GetNext();

    return pNode->GetValueRef();
}

template <typename Traits>
template <typename ObjFunc, typename... Args>
void CLinkedList<Traits>::Foreach(ObjFunc of, Args... args){
    Node *p = m_pRoot;
    while(p){
        of(p->GetValueRef(), args...);
        p = p->GetNext();
    }
}
template <typename Traits>
istream &operator>>(istream &is, CLinkedList<Traits> &container){
    using value_type = typename CLinkedList<Traits>::value_type;

    if(!is) return is;

    CLinkedList<Traits> tmp;

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


#endif // __LINKEDLIST_H__
