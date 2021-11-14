#include "utils.hpp"
#include "linmath.h"



using namespace std;
long getMillis() {
    return chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
}

float getRand() {
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}
