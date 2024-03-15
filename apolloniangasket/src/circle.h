#pragma once
#include <stdbool.h>
#include "complex.h"

typedef struct {
    Complex center;
    double bend;
    double radius;
} Circle;

Circle circleCreate(double bend, double x, double y);

void circleDraw(Circle* this);

bool circleIsTangent(const Circle c1, const Circle c2, const double epsilon);
