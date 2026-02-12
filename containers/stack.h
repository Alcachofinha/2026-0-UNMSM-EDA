#ifndef __STACK_H__
#define __STACK_H__
#include <iostream>
#include <stdexcept>
#include <iomanip>      
#include <type_traits>  
#include <vector>     
#include <utility>

#include "../general/types.h"
#include "../util.h" 
using namespace std;

template <typename T>
class NodeStack{
public:
    using value_type = T;
    using Node       = NodeStack<T>;

private:
    value_type m_data{};
    ref_type   m_ref{-1};
    Node*      m_pNext{nullptr};

public:
    NodeStack() {}

    NodeStack(const value_type& _value, ref_type _ref = -1, Node* _next = nullptr)
        : m_data(_value), m_ref(_ref), m_pNext(_next) {}

    value_type GetValue() const { return m_data; }

    
    value_type&       GetValueRef()       { return m_data; }
    const value_type& GetValueRef() const { return m_data; }

    ref_type  GetRef() const { return m_ref; }
    ref_type& GetRefRef()    { return m_ref; }

    // overloads como tu lista
    Node*       GetNext()       { return m_pNext; }
    const Node* GetNext() const { return m_pNext; }
    Node*&      GetNextRef()    { return m_pNext; }
};


template <typename T>
class CStack{
public:
    using value_type = T;
    using Node       = NodeStack<T>;

private:
    Node*  m_pTop{nullptr};
    size_t m_nElements{0};

private:
    
    void CopyFrom(const CStack& other){
        vector<pair<value_type, ref_type>> elems;
        elems.reserve(other.m_nElements);

        for(Node* p = other.m_pTop; p != nullptr; p = p->GetNext())
            elems.push_back({p->GetValue(), p->GetRef()});

        for(auto it = elems.rbegin(); it != elems.rend(); ++it)
            Push(it->first, it->second);
    }

public:
    CStack() {}

    // Constructor Copia (deep copy)
    CStack(const CStack& other){
        CopyFrom(other);
    }

    // Move Constructor
    CStack(CStack&& other) noexcept{
        m_pTop      = other.m_pTop;
        m_nElements = other.m_nElements;

        other.m_pTop      = nullptr;
        other.m_nElements = 0;
    }

    // Destructor seguro
    ~CStack(){
        clear();
    }

    // (Extra útil y coherente con tu estilo, aunque no esté en la imagen)
    CStack& operator=(const CStack& other){
        if(this == &other) return *this;
        clear();
        CopyFrom(other);
        return *this;
    }

    CStack& operator=(CStack&& other) noexcept{
        if(this == &other) return *this;
        clear();

        m_pTop      = other.m_pTop;
        m_nElements = other.m_nElements;

        other.m_pTop      = nullptr;
        other.m_nElements = 0;
        return *this;
    }

    bool empty() const { return m_nElements == 0; }
    size_t getSize() const { return m_nElements; }

    void clear(){
        while(m_pTop != nullptr){
            Node* p = m_pTop;
            m_pTop  = m_pTop->GetNext();
            delete p;
        }
        m_nElements = 0;
    }

    // Push: mete arriba (O(1))
    void Push(const value_type& val, ref_type ref = -1){
        Node* pNew = new Node(val, ref, m_pTop);
        m_pTop = pNew;
        ++m_nElements;
    }

    // Pop: saca arriba (O(1))
    value_type Pop(){
        if(m_pTop == nullptr) throw out_of_range("Pop() on empty stack");

        Node* p = m_pTop;
        value_type ret = p->GetValue();

        m_pTop = m_pTop->GetNext();
        delete p;
        --m_nElements;

        return ret;
    }

    // Pop con ref (si lo quieres usar en demo)
    value_type Pop(ref_type& ref){
        if(m_pTop == nullptr) throw out_of_range("Pop() on empty stack");

        Node* p = m_pTop;
        value_type ret = p->GetValue();
        ref = p->GetRef();

        m_pTop = m_pTop->GetNext();
        delete p;
        --m_nElements;

        return ret;
    }

    value_type& Top(){
        if(m_pTop == nullptr) throw out_of_range("Top() on empty stack");
        return m_pTop->GetValueRef();
    }

    const value_type& Top() const{
        if(m_pTop == nullptr) throw out_of_range("Top() on empty stack");
        return m_pTop->GetValueRef();
    }

    // Persistencia (write) - mismo estilo que tu linkedlist: solo [...]
    friend ostream& operator<<(ostream& os, const CStack& container){
        Node* pNode = container.m_pTop;
        os << "[";
        while(pNode != nullptr){
            os << "(" << pNode->GetValue() << ":" << pNode->GetRef() << ")";
            pNode = pNode->GetNext();
        }
        os << "]" << endl;
        return os;
    }

    // Persistencia (read) - mismo patrón que tu linkedlist:
    // 1) si hay texto antes, se salta hasta encontrar '['
    // 2) parsea (val:ref) hasta ']'
    // 3) reconstruye conservando TOP->BOTTOM
    friend istream& operator>>(istream& is, CStack& container){
        using value_type = typename CStack::value_type;

        if(!is) return is;

        CStack tmp;

        char ch;
        do{
            if(!is.get(ch)) return is;
        }while(ch != '[');

        vector<pair<value_type, ref_type>> elems;

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

            elems.push_back({val, ref}); // viene TOP -> BOTTOM en el archivo
        }

        // Para reconstruir igual: Push al revés
        for(auto it = elems.rbegin(); it != elems.rend(); ++it)
            tmp.Push(it->first, it->second);

        container = std::move(tmp);
        return is;
    }
};

#endif // __STACK_H__
