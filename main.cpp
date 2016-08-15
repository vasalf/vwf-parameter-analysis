#include <iostream>
#include "genetics.h"

using namespace std;

int main() {
    genetics gn(4, 4, 179, 4, 0.125);
    gn.execute();
    return 0;
}
