#include "complex.h"
#include <math.h>

Complex complexAdd(const Complex c1, const Complex c2) {
    return (Complex) { c1.a + c2.a, c1.b + c2.b };
}

Complex complexSub(const Complex c1, const Complex c2) {
    return (Complex) { c1.a - c2.a, c1.b - c2.b };
}

Complex complexScale(const Complex c1, const double value) {
    return (Complex) { c1.a * value, c1.b * value };
}

Complex complexMult(const Complex c1, const Complex c2) {
    // (a1 + b1*i) * (a2 + b2*i) = a1*a2 + a1*b2*i + a2*b1*i + b1*b2*i^2
    //                           = a1*a2 - b1*b2 + (a1*b2 + a2*b1) * i
    return (Complex) {
        .a = c1.a * c2.a - c1.b * c2.b,
        .b = c1.a * c2.b + c2.a * c1.b,
    };
}

Complex complexSqrt(const Complex this) {
    // Convert to polar form
    double r = sqrt(this.a * this.a + this.b * this.b);
    double theta = atan2(this.b, this.a);
    // Calculate the square root of the radius and use half the angle to get the square root of the complex number
    r = sqrt(r);
    theta = theta / 2.0;
    // Convert back to rectangular form
    return (Complex) { r * cos(theta), r * sin(theta) };
}

double complexDistance(const Complex c1, const Complex c2) {
    return sqrt((c1.a - c2.a)*(c1.a - c2.a) + (c1.b - c2.b)*(c1.b - c2.b));
}
