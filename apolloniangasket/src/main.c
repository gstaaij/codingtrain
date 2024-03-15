#include <assert.h>
#include <stdint.h>
#include <sys/time.h>
#include "raylib.h"
#include "raymath.h"
#include "colors.h"
#include "circle.h"
#include "descartes.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define WIDTH 800.0
#define HEIGHT 800.0

#define QUEUE_ADD_TRIPLET(queue, c1, c2, c3) arrput(queue, c1); arrput(queue, c2); arrput(queue, c3)

Circle* allCircles = NULL;
Circle* queue = NULL;
Circle* nextQueue = NULL;

unsigned int currentSeed = 0;

void nextGeneration() {
    nextQueue = NULL;
    assert(arrlen(queue) % 3 == 0 && "queue length not a multiple of 3");
    for (int i = 0; i < arrlen(queue); i += 3) {
        Circle c1 = queue[i + 0];
        Circle c2 = queue[i + 1];
        Circle c3 = queue[i + 2];

        double* k4 = descartes(c1, c2, c3);
        Circle* newCircles = descartesComplex(c1, c2, c3, k4);

        for (int j = 0; j < arrlen(newCircles); ++j) {
            if (descartesValidateCircle(allCircles, newCircles[j], c1, c2, c3)) {
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

// If seed is 0, a random seed is used
// Returns the seed it used
unsigned int reset(unsigned int seed) {
    if (allCircles) {
        arrfree(allCircles);
        allCircles = NULL;
    }
    if (queue) {
        arrfree(queue);
        queue = NULL;
    }

    if (seed) {
        SetRandomSeed(seed);
    } else {
        struct timeval currentTime;
        gettimeofday(&currentTime, NULL);
        seed = currentTime.tv_usec % UINT32_MAX;
        SetRandomSeed(seed);
    }

    Circle c1 = circleCreate(-1/ (WIDTH / 2 - WIDTH/20), WIDTH/2, HEIGHT/2);
    
    double r2Min = 100;
    double r2Max = c1.radius / 2;
    double r2 = (double) GetRandomValue(0, INT32_MAX) / INT32_MAX * (r2Max - r2Min) + r2Min;

    double r = c1.radius - r2;
    double theta = (double) GetRandomValue(0, INT32_MAX) / INT32_MAX * 2 * PI;
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

    return seed;
}

int main() {
    InitWindow(WIDTH, HEIGHT, "Apollonian Gasket");

    SetTargetFPS(60);

    currentSeed = reset(0);

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BACKGROUND_COLOR);

            int fontSize = 24;
            int y = 5;
            DrawText("FPS", 5, y, fontSize, FOREGROUND_COLOR);
            y += fontSize;
            DrawText(TextFormat("%d", GetFPS()), 5, y, fontSize, FOREGROUND_COLOR);
            y += fontSize;
            DrawText("Seed", 5, y, fontSize, FOREGROUND_COLOR);
            y += fontSize;
            DrawText(TextFormat("%lu", currentSeed), 5, y, fontSize, FOREGROUND_COLOR);

            for (int i = 0; i < arrlen(allCircles); ++i) {
                circleDraw(&allCircles[i]);
            }

            nextGeneration();

            if (IsKeyPressed(KEY_SPACE))
                currentSeed = reset(0);

        EndDrawing();
    }

    arrfree(allCircles);
    arrfree(queue);

    CloseWindow();
    return 0;
}