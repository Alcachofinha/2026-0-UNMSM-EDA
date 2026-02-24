//#include <iostream.h>
#include <time.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <utility>   // std::pair
#include "containers/BTree.h"
#include "containers/lists.h"

using namespace std;

// const char * keys="CDAMPIWNBKEHOLJYQZFXVRTSGU";
const char * keys1 = "D1XJ2xTg8zKL9AhijOPQcEowRSp0NbW567BUfCqrs4FdtYZakHIuvGV3eMylmn";
const char * keys2 = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const char * keys3 = "DYZakHIUwxVJ203ejOP9Qc8AdtuEop1XvTRghSNbW567BfiCqrs4FGMyzKLlmn";

const int BTreeSize = 3;

// =======================================================
// Helpers para recorridos variadicos
// Firma esperada: func(ObjectInfo&, int level, extras...)
// =======================================================

void PrintTraversal(BTree<char>::ObjectInfo &info, int level, const char* tag)
{
    cout << tag << " [nivel=" << level << "] "
         << "key=" << info.key
         << ", ObjID=" << info.ObjID
         << ", UseCounter=" << info.UseCounter
         << endl;
}

void PrintTraversalCount(BTree<char>::ObjectInfo &info, int level, const char* tag, int &count)
{
    ++count;
    cout << tag << " #" << count
         << " [nivel=" << level << "] "
         << "key=" << info.key
         << ", ObjID=" << info.ObjID
         << endl;
}

bool FirstGreaterThan(BTree<char>::ObjectInfo &info, int /*level*/, char lim)
{
    return info.key > lim;
}

bool FirstAtLevelAndGreater(BTree<char>::ObjectInfo &info, int level, int minLevel, char minKey)
{
    return (level >= minLevel) && (info.key >= minKey);
}

// =======================================================
// Demo principal
// =======================================================

void DemoBTree()
{
    int i;

    cout << "\n=========================================\n";
    cout << " DEMO BTree (base profe + mejoras)\n";
    cout << " B-Tree con comportamiento tipo B*\n";
    cout << "=========================================\n\n";

    // ---------------------------------------------------
    // 1) Constructor + insercion clasica (estilo profe)
    // ---------------------------------------------------
    cout << "[1] Constructor + insercion clasica (keys1)\n";
    BTree<char> bt(BTreeSize);

    for (i = 0; keys1[i]; i++)
    {
        bt.Insert(keys1[i], i * i);
    }

    cout << "Arbol construido con keys1.\n";
    cout << "Print(cout):\n";
    bt.Print(cout);
    cout << "\n";

    // ---------------------------------------------------
    // 2) Search (salvando la parte del profe)
    // ---------------------------------------------------
    cout << "[2] Search sobre keys2 (muestra ObjID devuelto)\n";
    cout << "Nota: si no existe, Search devuelve valor por defecto del tipo.\n";
    for (i = 0; keys2[i]; i++)
    {
        long obj = bt.Search(keys2[i]);
        cout << "Search(" << keys2[i] << ") -> " << obj << "\n";
    }
    cout << "\n";

    // ---------------------------------------------------
    // 3) Forward iterator (begin/end)
    // ---------------------------------------------------
    cout << "[3] Forward iterator (begin/end)\n";
    for (BTree<char>::iterator it = bt.begin(); it != bt.end(); ++it)
    {
        cout << it->key << " -> " << it->ObjID << "\n";
    }
    cout << "\n";

    // ---------------------------------------------------
    // 4) Backward iterator (rbegin/rend)
    // ---------------------------------------------------
    cout << "[4] Backward iterator (rbegin/rend)\n";
    for (BTree<char>::reverse_iterator it = bt.rbegin(); it != bt.rend(); ++it)
    {
        cout << it->key << " -> " << it->ObjID << "\n";
    }
    cout << "\n";

    // ---------------------------------------------------
    // 5) Recorridos variadicos
    // ---------------------------------------------------
    cout << "[5] InOrder variadico\n";
    bt.InOrder(PrintTraversal, "IN");
    cout << "\n";

    cout << "[6] PreOrder variadico\n";
    bt.PreOrder(PrintTraversal, "PRE");
    cout << "\n";

    cout << "[7] PostOrder variadico\n";
    bt.PostOrder(PrintTraversal, "POST");
    cout << "\n";

    cout << "[8] InOrder variadico con contador extra\n";
    int count = 0;
    bt.InOrder(PrintTraversalCount, "COUNT-IN", count);
    cout << "Total visitados: " << count << "\n\n";

    // ---------------------------------------------------
    // 6) FirstThat variadico
    // ---------------------------------------------------
    cout << "[9] FirstThat variadico\n";

    BTree<char>::ObjectInfo* p1 = bt.FirstThatV(FirstGreaterThan, 'm');
    if (p1)
        cout << "Primero con key > 'm': " << p1->key << " -> " << p1->ObjID << "\n";
    else
        cout << "No se encontro key > 'm'\n";

    BTree<char>::ObjectInfo* p2 = bt.FirstThatV(FirstAtLevelAndGreater, 1, 'A');
    if (p2)
        cout << "Primero con (nivel >= 1 && key >= 'A'): " << p2->key << " -> " << p2->ObjID << "\n";
    else
        cout << "No se encontro condicion compuesta\n";

    cout << "\n";

    // ---------------------------------------------------
    // 7) Insercion variadica (InsertMany)
    // ---------------------------------------------------
    cout << "[10] InsertMany (variadic insert)\n";
    BTree<char> manyTree(BTreeSize);

    manyTree.InsertMany(
        std::pair<char,long>('M', 100),
        std::pair<char,long>('C', 200),
        std::pair<char,long>('X', 300),
        std::pair<char,long>('A', 400),
        std::pair<char,long>('Q', 500)
    );

    cout << "Contenido de manyTree (forward):\n";
    for (BTree<char>::iterator it = manyTree.begin(); it != manyTree.end(); ++it)
        cout << it->key << " -> " << it->ObjID << "\n";
    cout << "\n";

    // ---------------------------------------------------
    // 8) operator< y operator>
    // ---------------------------------------------------
    cout << "[11] operator< y operator>\n";
    BTree<char> a(BTreeSize), b(BTreeSize);

    a.InsertMany(
        std::pair<char,long>('A', 1),
        std::pair<char,long>('B', 2),
        std::pair<char,long>('C', 3)
    );

    b.InsertMany(
        std::pair<char,long>('A', 1),
        std::pair<char,long>('B', 2),
        std::pair<char,long>('D', 4)
    );

    cout << "Arbol A: ";
    for (BTree<char>::iterator it = a.begin(); it != a.end(); ++it)
        cout << "(" << it->key << "," << it->ObjID << ") ";
    cout << "\n";

    cout << "Arbol B: ";
    for (BTree<char>::iterator it = b.begin(); it != b.end(); ++it)
        cout << "(" << it->key << "," << it->ObjID << ") ";
    cout << "\n";

    cout << "A < B ? " << (a < b) << "\n";
    cout << "A > B ? " << (a > b) << "\n";
    cout << "B < A ? " << (b < a) << "\n";
    cout << "B > A ? " << (b > a) << "\n\n";

    // ---------------------------------------------------
    // 9) Move Constructor
    // ---------------------------------------------------
    cout << "[12] Move Constructor\n";
    BTree<char> moved(std::move(manyTree));

    cout << "Contenido de moved (despues del move):\n";
    for (BTree<char>::iterator it = moved.begin(); it != moved.end(); ++it)
        cout << it->key << " -> " << it->ObjID << "\n";

    cout << "Contenido de manyTree (movido):\n";
    for (BTree<char>::iterator it = manyTree.begin(); it != manyTree.end(); ++it)
        cout << it->key << " -> " << it->ObjID << "\n";
    cout << "(Si no imprime nada, quedo vacio logico tras el move)\n\n";

    // ---------------------------------------------------
    // 10) Move Assignment (extra)
    // ---------------------------------------------------
    cout << "[13] Move Assignment (extra)\n";
    BTree<char> assigned(BTreeSize);
    assigned.InsertMany(
        std::pair<char,long>('Z', 999),
        std::pair<char,long>('Y', 888)
    );

    cout << "assigned antes del move assignment:\n";
    for (BTree<char>::iterator it = assigned.begin(); it != assigned.end(); ++it)
        cout << it->key << " -> " << it->ObjID << "\n";

    assigned = std::move(moved);

    cout << "assigned despues del move assignment:\n";
    for (BTree<char>::iterator it = assigned.begin(); it != assigned.end(); ++it)
        cout << it->key << " -> " << it->ObjID << "\n";
    cout << "\n";

    // ---------------------------------------------------
    // 11) Remove (salvando la parte comentada del profe)
    // ---------------------------------------------------
    cout << "[14] Remove parcial usando keys3 (salvado del demo del profe)\n";
    // Para no hacer salida gigante, borramos solo las primeras 15 claves de keys3
    for (i = 0; keys3[i] && i < 15; i++)
    {
        cout << "Removing " << keys3[i] << " ... ";
        if (bt.Remove(keys3[i], -1))
            cout << "removed!\n";
        else
            cout << "not found!\n";
    }

    cout << "\nArbol bt luego de removes parciales:\n";
    bt.Print(cout);
    cout << "\n";

    // ---------------------------------------------------
    // 12) Destructor (demo por scope)
    // ---------------------------------------------------
    cout << "[15] Destructor (por scope)\n";
    {
        BTree<char> local(BTreeSize);
        local.InsertMany(
            std::pair<char,long>('L', 11),
            std::pair<char,long>('O', 22),
            std::pair<char,long>('K', 33)
        );
        cout << "Dentro del scope: local creado y usado.\n";
    }
    cout << "Fuera del scope: destructor ejecutado automaticamente.\n\n";

    cout << "==== FIN DEMO BTREE ====\n";
}