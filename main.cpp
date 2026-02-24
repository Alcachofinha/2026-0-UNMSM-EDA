#include <iostream>
#include "util.h"
#include "pointers.h"
#include "containers/array.h"
#include "containers/lists.h"
#include "containers/BTree.h"

void DemoBTree();   // <-- prototipo (afuera)

int main() {
    std::cout << "Hello EDA-UNMSM!" << std::endl;

    void DemoBTree();  // declaración
    DemoBTree();       // llamada

    return 0;
}