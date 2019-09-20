
#include <cmath>
#include "Powell.h"
using namespace std;

namespace {
    inline void shft3(DP &a, DP &b, DP &c, const DP d)
    {
        a=b;
        b=c;
        c=d;
    }
}

