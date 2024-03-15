#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"
#include "colors.h"
#include "circle.h"

#define WIDTH 800.0
#define HEIGHT 800.0

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

#define VALIDATE_EPSILON 0.1
bool validate(const Circle* allCircles, Circle c4, Circle c1, Circle c2, Circle c3) {
    if (c4.radius < 2) return false;

    for (int i = 0; i < arrlen(allCircles); ++i) {
        Circle other = allCircles[i];
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

#define QUEUE_ADD_TRIPLET(queue, c1, c2, c3) arrput(queue, c1); arrput(queue, c2); arrput(queue, c3)

Circle* allCircles = NULL;
Circle* queue = NULL;
Circle* nextQueue = NULL;

void nextGeneration() {
    nextQueue = NULL;
    for (int i = 0; i < arrlen(queue); i += 3) {
        assert(arrlen(queue) > i + 2 && "queue length not a multiple of 3");
        Circle c1 = queue[i + 0];
        Circle c2 = queue[i + 1];
        Circle c3 = queue[i + 2];

        double* k4 = descartes(c1, c2, c3);
        Circle* newCircles = descartesComplex(c1, c2, c3, k4);

        for (int j = 0; j < arrlen(newCircles); ++j) {
            if (validate(allCircles, newCircles[j], c1, c2, c3)) {
                arrput(allCircles, newCircles[j]);
                QUEUE_ADD_TRIPLET(nextQueue, c1, c2, newCircles[j]);
                QUEUE_ADD_TRIPLET(nextQueue, c1, c3, newCircles[j]);
                QUEUE_ADD_TRIPLET(nextQueue, c2, c3, newCircles[j]);
            }
        }

        arrfree(newCircles);
    }
    arrfree(queue);
    queue = nextQueue;
}

int main() {
    InitWindow(WIDTH, HEIGHT, "Apollonian Gasket");

    SetTargetFPS(60);

    srand(100);

    Circle c1 = circleCreate(-1/ (WIDTH / 2 - WIDTH/20), WIDTH/2, HEIGHT/2);
    
    double r2Min = 100;
    double r2Max = c1.radius / 2;
    double r2 = (double) rand() / RAND_MAX * (r2Max - r2Min) + r2Min;

    double r = c1.radius - r2;
    double theta = (double) rand() / RAND_MAX * 2 * PI;
    Complex v = (Complex) { r * cos(theta), r * sin(theta) };
    Circle c2 = circleCreate(1 / r2, WIDTH/2 + v.a, HEIGHT/2 + v.b);

    double r3 = r;
    theta += PI;
    r = c1.radius - r3;
    v = (Complex) { r * cos(theta), r * sin(theta) };
    Circle c3 = circleCreate(1 / r3, WIDTH/2 + v.a, HEIGHT/2 + v.b);

    arrput(allCircles, c1);
    arrput(allCircles, c2);
    arrput(allCircles, c3);
    QUEUE_ADD_TRIPLET(queue, c1, c2, c3);

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BACKGROUND_COLOR);

            DrawText("FPS", 5, 5, 24, FOREGROUND_COLOR);
            DrawText(TextFormat("%d", GetFPS()), 5, 29, 24, FOREGROUND_COLOR);

            for (int i = 0; i < arrlen(allCircles); ++i) {
                circleDraw(&allCircles[i]);
            }

            nextGeneration();

        EndDrawing();
    }

    arrfree(allCircles);
    arrfree(queue);

    CloseWindow();
    return 0;
}