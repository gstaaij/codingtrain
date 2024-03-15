#include "descartes.h"
#include <math.h>
#include "stb_ds.h"

// Constants for validating the circles
#define VALIDATE_EPSILON 0.1
#define MIN_CIRCLE_SIZE 1.0

double* descartes(Circle c1, Circle c2, Circle c3) {
    double k1 = c1.bend;
    double k2 = c2.bend;
    double k3 = c3.bend;

    double sum = k1 + k2 + k3;
    double product = k1 * k2 + k2 * k3 + k3 * k1;
    // Because of floating point errors, this value can get below 0. If it is, set it to zero.
    if (product < 0) product = 0;
    double root = 2 * sqrt(product);

    double* result = NULL;
    arrput(result, sum + root);
    arrput(result, sum - root);
    return result;
}

Circle* descartesComplex(Circle c1, Circle c2, Circle c3, double* k4) {
    double k1 = c1.bend;
    double k2 = c2.bend;
    double k3 = c3.bend;
    Complex z1 = c1.center;
    Complex z2 = c2.center;
    Complex z3 = c3.center;

    Complex zk1 = complexScale(z1, k1);
    Complex zk2 = complexScale(z2, k2);
    Complex zk3 = complexScale(z3, k3);

    Complex sum = complexAdd(complexAdd(zk1, zk2), zk3);

    Complex root = complexAdd(complexAdd(complexMult(zk1, zk2), complexMult(zk2, zk3)), complexMult(zk3, zk1));
    root = complexScale(complexSqrt(root), 2);


    Complex center1 = complexScale(complexAdd(sum, root), 1 / k4[0]);
    Complex center2 = complexScale(complexSub(sum, root), 1 / k4[0]);
    Complex center3 = complexScale(complexAdd(sum, root), 1 / k4[1]);
    Complex center4 = complexScale(complexSub(sum, root), 1 / k4[1]);

    Circle* result = NULL;
    arrput(result, circleCreate(k4[0], center1.a, center1.b));
    arrput(result, circleCreate(k4[0], center2.a, center2.b));
    arrput(result, circleCreate(k4[1], center3.a, center3.b));
    arrput(result, circleCreate(k4[1], center4.a, center4.b));
    return result;
}

// c4 is the circle we're verifying, c1, c2 and c3 are the circles we're checking for tangentialness
// otherCircles is a list with all of the circles besides c4
bool descartesValidateCircle(const Circle* otherCircles, Circle c4, Circle c1, Circle c2, Circle c3) {
    if (c4.radius < MIN_CIRCLE_SIZE) return false;

    for (int i = 0; i < arrlen(otherCircles); ++i) {
        Circle other = otherCircles[i];
        double d = complexDistance(c4.center, other.center);
        double radiusDiff = fabs(c4.radius - other.radius);
        if (d < VALIDATE_EPSILON && radiusDiff < VALIDATE_EPSILON) return false;
    }

    // Check if all 4 circles are mutually tangential
    if (!circleIsTangent(c4, c1, VALIDATE_EPSILON)) return false;
    if (!circleIsTangent(c4, c2, VALIDATE_EPSILON)) return false;
    if (!circleIsTangent(c4, c3, VALIDATE_EPSILON)) return false;

    return true;
}
