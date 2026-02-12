// DemoQueue.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "containers/lists.h"
#include "containers/queue.h"

using namespace std;

void DemoQueue(){

    cout << "\n===== DEMO QUEUE (FIFO) - Branch 29 =====\n";

    // 1) Push / Front / Size / operator<<
    cout << "\n[1] Push / Front / Size / operator<<\n";
    CQueue<int> q;
    cout << "empty? " << q.empty() << "\n";
    cout << "size : " << q.getSize() << "\n";

    q.Push(10, 100);
    q.Push(20, 200);
    q.Push(30, 300);

    cout << "queue:\n" << q;
    cout << "Front = " << q.Front() << "\n";
    cout << "size  = " << q.getSize() << "\n";

    // 2) Pop() y Pop(ref)
    cout << "\n[2] Pop() / Pop(ref)\n";
    int v = q.Pop();
    cout << "Pop() -> " << v << "\n";
    cout << "queue:\n" << q;

    ref_type r;
    v = q.Pop(r);
    cout << "Pop(ref) -> value=" << v << " ref=" << r << "\n";
    cout << "queue:\n" << q;

    // 3) clear + exceptions 
    cout << "\n[3] clear() + exceptions\n";
    q.clear();
    cout << "after clear, empty? " << q.empty() << " size=" << q.getSize() << "\n";

    cout << "Probando Front() en empty...\n";
    try{
        cout << q.Front() << "\n";
        cout << "ERROR: debio lanzar excepcion\n";
    }catch(const exception& e){
        cout << "OK exception: " << e.what() << "\n";
    }

    cout << "Probando Pop() en empty...\n";
    try{
        cout << q.Pop() << "\n";
        cout << "ERROR: debio lanzar excepcion\n";
    }catch(const exception& e){
        cout << "OK exception: " << e.what() << "\n";
    }

    // 4) Copy ctor (deep copy)
    cout << "\n[4] Copy ctor (deep copy)\n";
    CQueue<int> a;
    a.Push(1, 11);
    a.Push(2, 22);
    a.Push(3, 33);

    CQueue<int> b(a);
    cout << "a:\n" << a;
    cout << "b (copy):\n" << b;

    a.Pop();
    a.Push(99, 999);

    cout << "after modify a:\n";
    cout << "a:\n" << a;
    cout << "b (should stay same):\n" << b;

    // 5) Copy assignment + self-assignment
    cout << "\n[5] Copy assignment + self-assignment\n";
    CQueue<int> c;
    c.Push(7, 70);
    c.Push(8, 80);

    cout << "c before:\n" << c;
    c = b;
    cout << "c after c=b:\n" << c;

    c = c;
    cout << "c after c=c:\n" << c;

    // 6) Move ctor
    cout << "\n[6] Move ctor\n";
    CQueue<int> mfrom;
    mfrom.Push(100, 1);
    mfrom.Push(200, 2);
    mfrom.Push(300, 3);

    cout << "mfrom before:\n" << mfrom;

    CQueue<int> mto(std::move(mfrom));
    cout << "mto after move ctor:\n" << mto;
    cout << "mfrom after move ctor:\n" << mfrom;
    cout << "mfrom empty? " << mfrom.empty() << " size=" << mfrom.getSize() << "\n";

    cout << "\n[7] Move assignment\n";
    CQueue<int> x1;
    x1.Push(5, 50);
    x1.Push(6, 60);

    CQueue<int> x2;
    x2.Push(9, 90);

    cout << "x1:\n" << x1;
    cout << "x2:\n" << x2;

    x2 = std::move(x1);
    cout << "after x2=move(x1):\n";
    cout << "x2:\n" << x2;
    cout << "x1:\n" << x1;

    cout << "\n[8] << file / >> file\n";
    CQueue<int> fileQ;
    fileQ.Push(10, 101);
    fileQ.Push(20, 202);
    fileQ.Push(30, 303);

    const char* f1 = "queue_data.txt";
    {
        ofstream ofs(f1);
        ofs << fileQ;
    }

    CQueue<int> loaded;
    {
        ifstream ifs(f1);
        ifs >> loaded;
    }

    cout << "fileQ:\n" << fileQ;
    cout << "loaded:\n" << loaded;
 
    cout << "\n[9] round-trip via stringstream\n";
    ostringstream oss;
    oss << fileQ;
    cout << "serialized: " << oss.str() << "\n";

    CQueue<int> rt;
    istringstream iss(oss.str());
    iss >> rt;
    cout << "rt:\n" << rt;

    cout << "\n[10] Queue<string> (quoted)\n";
    CQueue<string> qs;
    qs.Push("hola", 1);
    qs.Push("EDA UNMSM", 2);
    qs.Push("queue", 3);

    const char* f2 = "queue_string.txt";
    {
        ofstream ofs(f2);
        ofs << qs;
    }

    CQueue<string> qs2;
    {
        ifstream ifs(f2);
        ifs >> qs2;
    }

    cout << "qs:\n"  << qs;
    cout << "qs2:\n" << qs2;

    cout << "\n===== FIN DEMO QUEUE =====\n";
}
