#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <functional>
#include "containers/binarytree.h"

using namespace std;

// ===== Helpers de impresión =====

template <typename Node>
void PrintInOrdenConPadre(const Node *p){
    if(!p) return;
    PrintInOrdenConPadre(p->GetChild(0));

    cout << "(" << p->GetValue() << ":" << p->GetRef() << ", P=";
    if(p->GetParent()) cout << p->GetParent()->GetValue();
    else cout << "NULL";
    cout << ") ";

    PrintInOrdenConPadre(p->GetChild(1));
}

void PrintLine(const string &title){
    cout << "\n---------------- " << title << " ----------------\n";
}

// Variadic helpers
void PrintInt(int &x, const string &tag){
    cout << tag << x << " ";
}

void PushVec(int &x, vector<int> &v){
    v.push_back(x);
}

void PrintVec(const vector<int> &v){
    for(auto &e : v) cout << e << " ";
    cout << endl;
}

bool EsMayorQue(int &x, int limite){
    return x > limite;
}

template <typename Node>
bool CheckParents(const Node *p, const Node *parent=nullptr){
    if(!p) return true;
    if(p->GetParent() != parent) return false;
    return CheckParents(p->GetChild(0), p) && CheckParents(p->GetChild(1), p);
}

// Para crear el árbol estándar de prueba
template <typename Tree>
void BuildStandardTree(Tree &bt){
    bt.Insert(70, 70);
    bt.Insert(30, 30);
    bt.Insert(80, 80);
    bt.Insert(10, 10);
    bt.Insert(40, 40);
    bt.Insert(75, 75);
    bt.Insert(90, 90);
}

void DemoBinaryTree(){
    cout << "\n================== DEMO BINARY TREE (PASOS 1..10) ==================\n";

    using BT = CBinaryTree< TreeTraitAscending<int> >;

    // ===== PASO 1/2: Insert, size/empty, padres =====
    PrintLine("PASO 1/2: Insert + Size/Empty + Padres");
    BT bt;
    cout << "Empty? " << (bt.empty() ? "SI" : "NO") << endl;

    cout << "Insertando: 70, 30, 80, 10, 40, 75, 90\n";
    BuildStandardTree(bt);

    cout << "Size bt = " << bt.size() << endl;
    cout << "Inorden (con padre): ";
    PrintInOrdenConPadre(bt.GetRoot());
    cout << endl;

    cout << "CheckParents(bt): " << (CheckParents(bt.GetRoot()) ? "OK" : "FAIL") << endl;


    // ===== PASO 3: Remove =====
    PrintLine("PASO 3: Remove (hoja, 2 hijos, etc.)");
    cout << "Remove hoja 10 => " << (bt.Remove(10) ? "OK" : "NO") << endl;
    cout << "Inorden: ";
    PrintInOrdenConPadre(bt.GetRoot());
    cout << "\nSize = " << bt.size() << endl;

    cout << "Remove nodo con 2 hijos 30 => " << (bt.Remove(30) ? "OK" : "NO") << endl;
    cout << "Inorden: ";
    PrintInOrdenConPadre(bt.GetRoot());
    cout << "\nSize = " << bt.size() << endl;

    // Reinsertamos para seguir con la demo completa (recuperamos el árbol estándar)
    PrintLine("Reconstruyendo arbol estandar para seguir demo");
    bt.clear();
    BuildStandardTree(bt);
    cout << "Inorden: ";
    PrintInOrdenConPadre(bt.GetRoot());
    cout << "\nSize = " << bt.size() << endl;

    // ===== PASO 4: Recorridos variadic =====
    PrintLine("PASO 4: Recorridos variadic (in/pre/post)");
    cout << "INORDEN: ";
    bt.inorden(PrintInt, string(""));
    cout << endl;

    cout << "PREORDEN: ";
    bt.preorden(PrintInt, string(""));
    cout << endl;

    cout << "POSTORDEN: ";
    bt.postorden(PrintInt, string(""));
    cout << endl;

    // ===== PASO 5: Iterador forward (begin/end) =====
    PrintLine("PASO 5: Iterador Forward begin/end (inorden)");
    cout << "Iterando: ";
    for(auto it = bt.begin(); it != bt.end(); ++it)
        cout << *it << " ";
    cout << endl;

    // ===== PASO 6: Iterador backward (rbegin/rend) =====
    PrintLine("PASO 6: Iterador Backward rbegin/rend (inorden inverso)");
    cout << "Iterando reverso: ";
    for(auto it = bt.rbegin(); it != bt.rend(); ++it)
        cout << *it << " ";
    cout << endl;

    // ===== PASO 7: Foreach + FirstThat =====
    PrintLine("PASO 7: Foreach + FirstThat (variadic)");
    cout << "Foreach(tag=v=): ";
    bt.Foreach(PrintInt, string("v="));
    cout << endl;

    auto it = bt.FirstThat(EsMayorQue, 50);
    cout << "FirstThat(>50): ";
    if(it != bt.end()) cout << *it << endl;
    else cout << "No encontrado\n";

    // ===== PASO 8: operator<< / operator>> =====
    PrintLine("PASO 8: Persistencia operator<< / operator>>");
    stringstream ss;
    ss << bt;

    cout << "Serializacion (preorden con #):\n";
    cout << ss.str() << endl;

    BT bt_copy;
    ss >> bt_copy;

    cout << "Iterando bt_copy (begin/end): ";
    for(auto it2 = bt_copy.begin(); it2 != bt_copy.end(); ++it2)
        cout << *it2 << " ";
    cout << endl;

    // ===== Verificación PRO: serialización idéntica + recorridos =====
    PrintLine("VERIFICACION PRO (estructura identica)");
    stringstream s1, s2;
    s1 << bt;
    s2 << bt_copy;

    cout << "bt     : " << s1.str() << endl;
    cout << "bt_copy: " << s2.str() << endl;
    cout << "Serializacion igual? " << ((s1.str() == s2.str()) ? "OK" : "FAIL") << endl;

    cout << "CheckParents(bt_copy): " << (CheckParents(bt_copy.GetRoot()) ? "OK" : "FAIL") << endl;

    vector<int> in1, in2, pre1, pre2, post1, post2;
    bt.inorden(PushVec, std::ref(in1));
    bt_copy.inorden(PushVec, std::ref(in2));

    bt.preorden(PushVec, std::ref(pre1));   
    bt_copy.preorden(PushVec, std::ref(pre2));

    bt.postorden(PushVec, std::ref(post1));
    bt_copy.postorden(PushVec, std::ref(post2));


    cout << "in  bt     : "; PrintVec(in1);
    cout << "in  bt_copy: "; PrintVec(in2);
    cout << "pre bt     : "; PrintVec(pre1);
    cout << "pre bt_copy: "; PrintVec(pre2);
    cout << "post bt     : "; PrintVec(post1);
    cout << "post bt_copy: "; PrintVec(post2);

    cout    << "Recorridos iguales? "
            << ((in1==in2 && pre1==pre2 && post1==post2) ? "OK" : "FAIL") << endl;

    // ===== PASO 9/10: Concurrencia + exchange (pruebas rápidas sin hilos) =====
    PrintLine("PASO 9/10: Move/Clear rapido (exchange) + sanity");
    BT bt_move(std::move(bt_copy));
    cout << "bt_copy.size (despues move) = " << bt_copy.size() << " (esperado 0)\n";
    cout << "bt_move.size = " << bt_move.size() << " (esperado 7)\n";

    bt_move.clear();
    cout << "bt_move.size (despues clear) = " << bt_move.size() << " (esperado 0)\n";

    cout << "\n================== FIN DEMO BINARY TREE (PASOS 1..10) ==================\n\n";
}
