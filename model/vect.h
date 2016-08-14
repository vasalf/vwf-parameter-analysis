#ifndef VECT_H_
#define VECT_H_

struct vect {
    double x, y, z;

    vect();               // default constructor, create (0, 0, 0) vector
    vect(double _x, double _y, double _z);
    vect(vect a, vect b); // creates a vector between points a and b
    vect operator+(const vect & other) const; // vector sum
    double length() const;
};

// that allows us to multiply a vector by a number

vect operator*(const double & f, const vect & v);

#endif // VECT_H_
