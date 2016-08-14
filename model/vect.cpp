#include "vect.h"
#include <cmath>

vect::vect() {
    x = y = z = 0;
}

vect::vect(double _x, double _y, double _z) {
    x = _x;
    y = _y;
    z = _z;
}

vect::vect(vect a, vect b) {
    x = b.x - a.x;
    y = b.y - a.y;
    z = b.z - a.z;
}

vect vect::operator+(const vect & other) const {
    return vect(x + other.x, y + other.y, z + other.z);
}

double vect::length() const {
    return std::sqrt(x * x + y * y + z * z);
}

vect operator*(const double & f, const vect & v) {
    return vect(f * v.x, f * v.y, f * v.z);
}
