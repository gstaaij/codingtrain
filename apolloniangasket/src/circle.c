#include "circle.h"
#include <raylib.h>
#include <math.h>
#include "colors.h"

#define CIRCLE_STROKE_WIDTH 2.0

Circle circleCreate(double bend, double x, double y) {
    return (Circle){
        .center = (Complex) { x, y },
        .bend = bend,
        .radius = fabs(1.0 / bend),
    };
}

void circleDraw(Circle* this) {
    DrawRing(
        (Vector2) { this->center.a, this->center.b },
        this->radius - CIRCLE_STROKE_WIDTH/2,
        this->radius + CIRCLE_STROKE_WIDTH/2,
        0, 360,
        1,
        FOREGROUND_COLOR
    );
}

bool circleIsTangent(const Circle c1, const Circle c2, const double epsilon) {
    double d = complexDistance(c1.center, c2.center);

    double r1 = c1.radius;
    double r2 = c2.radius;

    // Tangency check based on distances and radii
    return (fabs(d -     (r1 + r2)) < epsilon ||
            fabs(d - fabs(r2 - r1)) < epsilon );
}
