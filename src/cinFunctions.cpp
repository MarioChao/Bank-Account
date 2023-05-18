#include "../inc/cinFunctions.h"

void clearCin() {
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}