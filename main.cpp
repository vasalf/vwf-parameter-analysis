#include <iostream>
#include "run.h"

using namespace std;

int main() {
    run_holder rh;
    run_results res = rh.run(1, {2, 0, 3, 4, 2, 3, 3, 4});
    cout << res.rating() << endl;
    return 0;
}
