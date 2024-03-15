#pragma once
#include "circle.h"

double* descartes(Circle c1, Circle c2, Circle c3);

Circle* descartesComplex(Circle c1, Circle c2, Circle c3, double* k4);

bool descartesValidateCircle(const Circle* otherCircles, Circle c4, Circle c1, Circle c2, Circle c3);
