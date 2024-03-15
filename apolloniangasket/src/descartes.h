#pragma once
#include "circle.h"

double* descartes(Circle c1, Circle c2, Circle c3);

Circle* descartesComplex(Circle c1, Circle c2, Circle c3, double* k4);

/* Check if a potential new circle is valid
 * 
 * newCircle is the circle we're validating, c1, c2 and c3 are the circles we're checking for tangency
 * otherCircles is a list with all of the circles besides newCircle
 */
bool descartesValidateCircle(const Circle* otherCircles, Circle c4, Circle c1, Circle c2, Circle c3);
