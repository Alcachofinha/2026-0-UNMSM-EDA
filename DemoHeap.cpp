#include <iostream>
#include <fstream>
#include "containers/lists.h"

using namespace std;

void DemoHeap(){

    cout << "\n===== DEMO HEAP (MIN/MAX) =====\n";

    // -------------------------
    // 1) MIN HEAP
    // -------------------------
    cout << "\n[1] MinHeap\n";
    CHeap<MinHeapTrait<int>> hmin;

    hmin.Push(50);
    hmin.Push(10);
    hmin.Push(30);
    hmin.Push(5);

    cout << "Heap: " << hmin << "\n";
    cout << "Top:  " << hmin.top() << "\n";

    cout << "Pop:  " << hmin.Pop() << "\n";
    cout << "Heap: " << hmin << "\n";

    // -------------------------
    // 2) MAX HEAP
    // -------------------------
    cout << "\n[2] MaxHeap\n";
    CHeap<MaxHeapTrait<int>> hmax;

    hmax.Push(50);
    hmax.Push(10);
    hmax.Push(30);
    hmax.Push(100);

    cout << "Heap: " << hmax << "\n";
    cout << "Top:  " << hmax.top() << "\n";

    cout << "Pop:  " << hmax.Pop() << "\n";
    cout << "Heap: " << hmax << "\n";

    // -------------------------
    // 3) ARCHIVO (operator>>)
    // Formato: n seguido de n valores
    // -------------------------
    cout << "\n[3] Archivo\n";
    {
        ofstream out("heap_demo.txt");
        out << "5 20 60 10 40 5";   // n=5, luego 5 valores
    }

    CHeap<MinHeapTrait<int>> hfile;
    ifstream in("heap_demo.txt");
    if (!in.is_open()){
        cout << "No se pudo abrir heap_demo.txt\n";
    }else{
        in >> hfile;
        in.close();
        cout << "Heap leido: " << hfile << "\n";
        cout << "Top:       " << hfile.top() << "\n";
    }

    // -------------------------
    // 4) COPY + MOVE
    // -------------------------
    cout << "\n[4] Copy + Move\n";

    CHeap<MinHeapTrait<int>> copia(hfile);
    copia.Push(-999);

    cout << "Original: " << hfile << "\n";
    cout << "Copia:    " << copia << "\n";

    CHeap<MinHeapTrait<int>> movido(std::move(hfile));
    cout << "Movido:   " << movido << "\n";
    cout << "Original vacio? " << (hfile.empty() ? "SI" : "NO") << "\n";

    cout << "\n--- FIN DEMO ---\n";
}
