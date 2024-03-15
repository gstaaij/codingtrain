#pragma once

// a + b*i
typedef struct {
    double a;
    double b;
} Complex;

Complex complexAdd(const Complex c1, const Complex c2);
Complex complexSub(const Complex c1, const Complex c2);
Complex complexScale(const Complex c1, const double value);
Complex complexMult(const Complex c1, const Complex c2);
Complex complexSqrt(const Complex this);

double complexDistance(const Complex c1, const Complex c2);
