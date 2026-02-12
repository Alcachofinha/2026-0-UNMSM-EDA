#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <iostream>
#include <stdexcept>
#include <iomanip>      // std::quoted
#include <type_traits>  // std::is_same_v
#include <vector>
#include <utility>

#include "../general/types.h"
#include "../util.h"
using namespace std;


template <typename T>
class NodeQueue{
public:
    using value_type = T;
    using Node       = NodeQueue<T>;

private:
    value_type m_data{};
    ref_type   m_ref{-1};
    Node*      m_pNext{nullptr};

public:
    NodeQueue() {}

    NodeQueue(const value_type& _value, ref_type _ref = -1, Node* _next = nullptr)
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
        return *this;
    }

    bool operator==(const Node& another) const { return m_data == another.GetValue(); }
};

template <typename T>
class CQueue{
public:
    using value_type = T;
    using Node       = NodeQueue<T>;

private:
    Node*  m_pFront{nullptr};  
    Node*  m_pBack{nullptr};    
    size_t m_nElements{0};

private:
    void CopyFrom(const CQueue& other){
        
        for(Node* p = other.m_pFront; p != nullptr; p = p->GetNext()){
            Push(p->GetValue(), p->GetRef());
        }
    }

public:
    CQueue() {}

    CQueue(const CQueue& other){
        CopyFrom(other);
    }

    CQueue(CQueue&& other) noexcept{
        m_pFront     = other.m_pFront;
        m_pBack      = other.m_pBack;
        m_nElements  = other.m_nElements;

        other.m_pFront    = nullptr;
        other.m_pBack     = nullptr;
        other.m_nElements = 0;
    }

    ~CQueue(){
        clear();
    }

    CQueue& operator=(const CQueue& other){
        if(this == &other) return *this;
        clear();
        CopyFrom(other);
        return *this;
    }

    // Move assignment
    CQueue& operator=(CQueue&& other) noexcept{
        if(this == &other) return *this;
        clear();

        m_pFront     = other.m_pFront;
        m_pBack      = other.m_pBack;
        m_nElements  = other.m_nElements;

        other.m_pFront    = nullptr;
        other.m_pBack     = nullptr;
        other.m_nElements = 0;
        return *this;
    }

    bool empty() const { return m_nElements == 0; }
    size_t getSize() const { return m_nElements; }

    void clear(){
        while(m_pFront != nullptr){
            Node* p = m_pFront;
            m_pFront = m_pFront->GetNext();
            delete p;
        }
        m_pBack = nullptr;
        m_nElements = 0;
    }

    void Push(const value_type& val, ref_type ref = -1){
        Node* pNew = new Node(val, ref, nullptr);

        if(!m_pFront){
            m_pFront = m_pBack = pNew;
        }else{
            m_pBack->GetNextRef() = pNew;
            m_pBack = pNew;
        }
        ++m_nElements;
    }


    value_type Pop(){
        if(!m_pFront) throw out_of_range("Pop() on empty queue");

        Node* p = m_pFront;
        value_type ret = p->GetValue();

        m_pFront = m_pFront->GetNext();
        if(!m_pFront) m_pBack = nullptr; 

        delete p;
        --m_nElements;
        return ret;
    }

    value_type Pop(ref_type& ref){
        if(!m_pFront) throw out_of_range("Pop() on empty queue");

        Node* p = m_pFront;
        value_type ret = p->GetValue();
        ref = p->GetRef();

        m_pFront = m_pFront->GetNext();
        if(!m_pFront) m_pBack = nullptr;

        delete p;
        --m_nElements;
        return ret;
    }

    value_type& Front(){
        if(!m_pFront) throw out_of_range("Front() on empty queue");
        return m_pFront->GetValueRef();
    }

    const value_type& Front() const{
        if(!m_pFront) throw out_of_range("Front() on empty queue");
        return m_pFront->GetValue();
    }

    friend ostream& operator<<(ostream& os, const CQueue& container){
        os << "[";
        Node* pNode = container.m_pFront;

        while(pNode != nullptr){
            os << "(";

            if constexpr (std::is_same_v<value_type, std::string>)
                os << std::quoted(pNode->GetValue());
            else
                os << pNode->GetValue();

            os << ":" << pNode->GetRef() << ")";
            pNode = pNode->GetNext();
        }

        os << "]" << endl;
        return os;
    }

    friend istream& operator>>(istream& is, CQueue& container){
        if(!is) return is;

        CQueue tmp;

        char ch;
        do{
            if(!is.get(ch)) return is;   // EOF
        }while(ch != '[');

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
            ref_type ref{-1};

            if constexpr (std::is_same_v<value_type, std::string>)
                is >> std::quoted(val);
            else
                is >> val;

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

            tmp.Push(val, ref); // FIFO: el orden leído es front->back
        }

        container = std::move(tmp);
        return is;
    }
};

#endif // __QUEUE_H__
