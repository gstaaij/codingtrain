#pragma once
#include <stdbool.h>
#include "raylib.h"
#include "complex.h"

// Defines a circle in terms of its bend (curvature) and center point
typedef struct {
    Complex center;
    double bend;
    double radius;
} Circle;

// "Constructor" of the Circle
Circle circleCreate(double bend, double x, double y);

// Draws the circle
void circleDraw(Circle* this, const Color color);

// Determine if two circles are tangent to each other
bool circleIsTangent(const Circle c1, const Circle c2, const double epsilon);
