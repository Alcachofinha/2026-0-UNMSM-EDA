#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include "containers/lists.h" 
#include "containers/stack.h"
using namespace std;


void DemoStacks(){
    try{
        cout << "DEMO STACK (LIFO)\n";

        // 1) Push / Top / Size / operator<<
        cout << "1) Push / Top / Size / operator<<\n";
        CStack<int> st;
        cout << "empty? " << st.empty() << "\n";
        cout << "size : "  << st.getSize() << "\n";

        st.Push(10, 100);
        st.Push(20, 200);
        st.Push(30, 300);

        cout << "stack:\n" << st;
        cout << "Top = " << st.Top() << "\n";
        cout << "size = " << st.getSize() << "\n";

        // 2) Pop() y Pop(ref)
        cout << "2) Pop() / Pop(ref)\n";
        int v = st.Pop();
        cout << "Pop() -> " << v << "\n";
        cout << "stack:\n" << st;

        ref_type r;
        v = st.Pop(r);
        cout << "Pop(ref) -> value=" << v << " ref=" << r << "\n";
        cout << "stack:\n" << st;

        // 3) clear + excepciones Pop/Top vacio
        cout << "3) clear() + exceptions\n";
        st.clear();
        cout << "after clear, empty? " << st.empty() << " size=" << st.getSize() << "\n";

        try { st.Top();  cout << "ERROR: Top() debio lanzar\n"; }
        catch(const exception& e){ cout << "OK Top() exception: " << e.what() << "\n"; }

        try { st.Pop();  cout << "ERROR: Pop() debio lanzar\n"; }
        catch(const exception& e){ cout << "OK Pop() exception: " << e.what() << "\n"; }

        // 4) Copy ctor
        cout << "4) Copy ctor (deep copy)\n";
        CStack<int> a;
        a.Push(1, 11); a.Push(2, 22); a.Push(3, 33);
        CStack<int> b(a);

        cout << "a:\n" << a;
        cout << "b (copy):\n" << b;

        a.Pop(); a.Push(99, 999);
        cout << "after modify a:\n";
        cout << "a:\n" << a;
        cout << "b (should stay same):\n" << b;

        // 5) Copy assignment + self-assignment
        cout << "5) Copy assignment + self-assignment\n";
        CStack<int> c;
        c.Push(7, 70); c.Push(8, 80);
        cout << "c before:\n" << c;

        c = b;
        cout << "c after c=b:\n" << c;

        c = c;
        cout << "c after c=c:\n" << c;

        // 6) Move ctor
        cout << "6) Move ctor\n";
        CStack<int> mfrom;
        mfrom.Push(100,1); mfrom.Push(200,2); mfrom.Push(300,3);

        cout << "mfrom before:\n" << mfrom;
        CStack<int> mto(std::move(mfrom));
        cout << "mto after move ctor:\n" << mto;
        cout << "mfrom after move ctor:\n" << mfrom;

        // 7) Move assignment
        cout << "7) Move assignment\n";
        CStack<int> x1;
        x1.Push(5,50); x1.Push(6,60);

        CStack<int> x2;
        x2.Push(9,90);

        cout << "x1:\n" << x1;
        cout << "x2:\n" << x2;

        x2 = std::move(x1);
        cout << "after x2=move(x1):\n";
        cout << "x2:\n" << x2;
        cout << "x1:\n" << x1;

        // 8) operator<< y operator>>
        cout << "8) << file / >> file\n";
        CStack<int> fileStack;
        fileStack.Push(10,101);
        fileStack.Push(20,202);
        fileStack.Push(30,303);

        const char* f1 = "stack_data.txt";
        { ofstream ofs(f1); if(!ofs) throw runtime_error("no write file"); ofs << fileStack; }

        CStack<int> loaded;
        { ifstream ifs(f1); if(!ifs) throw runtime_error("no read file"); ifs >> loaded; }

        cout << "fileStack:\n" << fileStack;
        cout << "loaded:\n" << loaded;

        // 9) Stack<string>
        cout << "9) Stack<string> (quoted)\n";
        CStack<string> ss;
        ss.Push("hola", 1);
        ss.Push("EDA UNMSM", 2);
        ss.Push("stack", 3);

        const char* f2 = "stack_string.txt";
        { ofstream ofs(f2); if(!ofs) throw runtime_error("no write string file"); ofs << ss; }

        CStack<string> ss2;
        { ifstream ifs(f2); if(!ifs) throw runtime_error("no read string file"); ifs >> ss2; }

        cout << "ss:\n" << ss;
        cout << "ss2:\n" << ss2;

        cout << "FIN DEMO STACK\n";
    }
    catch(const exception& e){
        cerr << "\n[DemoStack ERROR] " << e.what() << "\n";
    }
}
