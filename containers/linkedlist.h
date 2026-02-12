#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

#include <iostream>
#include <functional> 
#include "../general/types.h"
#include "../util.h"
using namespace std;

template <typename T, typename _Func>
struct ListTrait{
    using value_type = T;
    using Func       = _Func;
};

template <typename T>
struct AscendingTrait : public ListTrait<T, std::greater<T> >{};

template <typename T>
struct DescendingTrait : public ListTrait<T, std::less<T> >{};

template <typename Traits>
class NodeLinkedList{
    using value_type  = typename Traits::value_type;
    using Node        = NodeLinkedList<Traits>;

private:
    value_type m_data{};
    ref_type   m_ref{-1};
    Node*      m_pNext{nullptr};

public:
    NodeLinkedList() {}

    NodeLinkedList(const value_type& _value, ref_type _ref = -1, Node* _next = nullptr)
        : m_data(_value), m_ref(_ref), m_pNext(_next) {}

    value_type  GetValue() const { return m_data; }
    value_type& GetValueRef()    { return m_data; }

    ref_type    GetRef() const   { return m_ref; }
    ref_type&   GetRefRef()      { return m_ref; }

    Node*       GetNext()        { return m_pNext; }
    const Node* GetNext() const  { return m_pNext; }
    Node*&      GetNextRef()     { return m_pNext; }

    Node& operator=(const Node& another){
        m_data = another.GetValue();
        m_ref  = another.GetRef();
        // no copiamos m_pNext (eso lo maneja la estructura)
        return *this;
    }

    bool operator==(const Node& another) const { return m_data == another.GetValue(); }
    bool operator<(const Node& another)  const { return m_data <  another.GetValue(); }
};

template <typename Traits>
class CLinkedList {
    using value_type  = typename Traits::value_type;
    using Node        = NodeLinkedList<Traits>;

private:
    Node*  m_pRoot{nullptr};
    Node*  m_pLast{nullptr};
    size_t m_nElements{0};

private:
    void InternalInsert(Node*& rParent, const value_type& val, ref_type ref){
        typename Traits::Func cmp;


        if(!rParent || cmp(rParent->GetValue(), val)){
            Node* pNew = new Node(val, ref, rParent);
            rParent = pNew;
            ++m_nElements;
            return;
        }
        InternalInsert(rParent->GetNextRef(), val, ref);
    }

    void UpdateLast(){
        m_pLast = m_pRoot;
        if(!m_pLast) return;
        while(m_pLast->GetNext() != nullptr)
            m_pLast = m_pLast->GetNext();
    }

public:
    CLinkedList() {}

    size_t getSize() const { return m_nElements; }


    void push_back(const value_type& val, ref_type ref = -1){
        Node* pNewNode = new Node(val, ref, nullptr);

        if(!m_pRoot){
            m_pRoot = m_pLast = pNewNode;
        }else{
            m_pLast->GetNextRef() = pNewNode;
            m_pLast = pNewNode;
        }
        ++m_nElements;
    }

    void Insert(const value_type& val, ref_type ref = -1){
        InternalInsert(m_pRoot, val, ref);
        UpdateLast();
    }

    // Persistencia (write)
    friend ostream& operator<<(ostream& os, const CLinkedList& container){
        os << "CLinkedList: size = " << container.getSize() << endl;
        os << "[";
        Node* p = container.m_pRoot;
        while(p != nullptr){
            os << "(" << p->GetValue() << ":" << p->GetRef() << ")";
            p = p->GetNext();
        }
        os << "]" << endl;
        return os;
    }


};

#endif // __LINKEDLIST_H__
