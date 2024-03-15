#pragma once

// Defines a complex number and its operations
// a + b*i
typedef struct {
    double a;
    double b;
} Complex;

// There is no "constructor" here, just do (Complex) { a, b } to make a new complex number

// Adds two complex numbers together
Complex complexAdd(const Complex c1, const Complex c2);
// Subtracts two complex numbers
Complex complexSub(const Complex c1, const Complex c2);
// Scales a complex number by a real number value
Complex complexScale(const Complex c1, const double value);
// Multiplies two complex numbers together, using the formula (ac-bd) + (ad+bc)i
Complex complexMult(const Complex c1, const Complex c2);
// Calculates the square root of a complex number
Complex complexSqrt(const Complex this);

// Computes the distance between two complex numbers on the complex plane
double complexDistance(const Complex c1, const Complex c2);
