#include "core/Simplex.h"

int main() {
    Simplex simplex;
    if (!simplex.Init()) {
        return 0;
    }

    simplex.Start();
    return 0;
}
