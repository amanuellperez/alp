#include "alp_math.h"

namespace alp{

Degree normalize(const Degree& a)
{ 
    using Rep = Degree::Rep;
// modulo(a.value(), 360):
    int x = static_cast<int>(a.value());

    if (x >= 0){
	int r = x % 360;
	int delta = a.value() - static_cast<Rep>(x);

	return  Degree{static_cast<Rep>(r + delta)};
    }
    else{ // x < 0
	x = -x;

	int r = x % 360;
	int delta = -a.value() - static_cast<Rep>(x);

	return  Degree{static_cast<Rep>(360 - (r + delta))};

    }
}

}// namespace


