#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <sys/time.h>
#include <math.h>
#include "raylib.h"
#include "colors.h"
#include "circle.h"
#include "descartes.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define WIDTH 800.0
#define HEIGHT 800.0

#define QUEUE_ADD_TRIPLET(queue, c1, c2, c3) arrput(queue, c1); arrput(queue, c2); arrput(queue, c3)

// All circles in the gasket
Circle* allCircles = NULL;
// Queue for circles to process for the next generation
Circle* queue = NULL;
// The next queue (this needs to be global because)
Circle* nextQueue = NULL;

// Whether we're done generating
bool done = false;

// The current seed. Used for displaying the text
unsigned int currentSeed = 0;

// If seed is 0, a random seed is used
// Returns the seed it used
unsigned int reset(unsigned int seed) {
    // Remove all circles by freeing the dynamic array for the circles and for the queue
    if (allCircles) {
        arrfree(allCircles);
        allCircles = NULL;
    }
    if (queue) {
        arrfree(queue);
        queue = NULL;
    }

    // Reset the done flag
    done = false;

    // Set the seed
    if (seed) {
        SetRandomSeed(seed);
    } else {
        struct timeval currentTime;
        gettimeofday(&currentTime, NULL);
        seed = currentTime.tv_usec % UINT32_MAX;
        SetRandomSeed(seed);
    }

    TraceLog(LOG_DEBUG, "====================");

    // The first circle centered in the window
    double scaleValue = fmin(WIDTH, HEIGHT);
    double r1 = scaleValue/2 - scaleValue/20;
    Circle c1 = circleCreate(-1/r1, WIDTH/2, HEIGHT/2);
    
    double r2Min = scaleValue / 8;
    double r2Max = c1.radius / 2;
    // A random radius for the second circle
    double r2 = (double) GetRandomValue(0, INT32_MAX) / INT32_MAX * (r2Max - r2Min) + r2Min;

    double r = c1.radius - r2;
    TraceLog(LOG_DEBUG, "radius of c2: %lf", r);
    double theta = (double) GetRandomValue(0, INT32_MAX) / INT32_MAX * 2 * PI;
    // For positioning the second (and third) circle
    Complex v = (Complex) { r * cos(theta), r * sin(theta) };

    // Second circle, positioned randomly within the first
    Circle c2 = circleCreate(1 / r2, WIDTH/2 + v.a, HEIGHT/2 + v.b);
    TraceLog(LOG_DEBUG, "position of c2: %lf + %lf * i", c2.center.a, c2.center.b);

    double r3 = r;
    theta += PI;
    r = c1.radius - r3;
    TraceLog(LOG_DEBUG, "radius of c3: %lf", r);
    v = (Complex) { r * cos(theta), r * sin(theta) };

    // Third circle positioned relative to the first and second
    Circle c3 = circleCreate(1 / r3, WIDTH/2 + v.a, HEIGHT/2 + v.b);
    TraceLog(LOG_DEBUG, "position of c3: %lf + %lf * i", c3.center.a, c3.center.b);

    // Add all the circles to the dynamic array
    arrput(allCircles, c1);
    arrput(allCircles, c2);
    arrput(allCircles, c3);
    // Initial triplet for generating next generation of circles
    QUEUE_ADD_TRIPLET(queue, c1, c2, c3);

    return seed;
}

void nextGeneration() {
    nextQueue = NULL;
    assert(arrlen(queue) % 3 == 0 && "queue length not a multiple of 3");
    for (int i = 0; i < arrlen(queue); i += 3) {
        Circle c1 = queue[i + 0];
        Circle c2 = queue[i + 1];
        Circle c3 = queue[i + 2];

        // Calculate curvatures for potential new circles
        double* k4 = descartes(c1, c2, c3);
        // Generate new circles based on Descartes' theorem
        Circle* newCircles = descartesComplex(c1, c2, c3, k4);

        for (int j = 0; j < arrlen(newCircles); ++j) {
            if (descartesValidateCircle(allCircles, newCircles[j], c1, c2, c3)) {
                arrput(allCircles, newCircles[j]);
                // New triplets formed with the new circle for the next generation
                QUEUE_ADD_TRIPLET(nextQueue, c1, c2, newCircles[j]);
                QUEUE_ADD_TRIPLET(nextQueue, c1, c3, newCircles[j]);
                QUEUE_ADD_TRIPLET(nextQueue, c2, c3, newCircles[j]);
            }
        }

        arrfree(newCircles);
    }
    // The memory for the previous queue should be freed
    arrfree(queue);
    queue = nextQueue;
}

// Stolen from nob (https://github.com/tsoding/musializer/blob/master/src/nob.h)
char* shift_args(int* argc, char*** argv) {
    assert(*argc > 0);
    char* result = **argv;
    (*argv) += 1;
    (*argc) -= 1;
    return result;
}

void printUsageAndFail(char* program) {
    TraceLog(LOG_ERROR, "Usage: %s [options] [seed]", program);
    TraceLog(LOG_ERROR, "Possible options:");
    TraceLog(LOG_ERROR, "    --debug     Enables debug information");
    TraceLog(LOG_ERROR, "    --dark      Enables dark mode");
    TraceLog(LOG_FATAL, "");
}

int main(int argc, char** argv) {
    // Handling arguments
    char* program = shift_args(&argc, &argv);
    if (argc > 0) {
        char* arg = shift_args(&argc, &argv);

        if (strcmp(arg, "--debug") == 0) {
            SetTraceLogLevel(LOG_DEBUG);
            if (argc > 0) arg = shift_args(&argc, &argv);
            else          arg = NULL;
        }

        if (arg && strcmp(arg, "--dark") == 0) {
            colorSetDarkMode(true);

            if (argc > 0) arg = shift_args(&argc, &argv);
            else          arg = NULL;
        }
        
        if (arg) {
            for (char* x = arg; *x != 0; ++x) {
                if (!isdigit(*x)) {
                    TraceLog(LOG_ERROR, "\"%s\" is not a number\n", arg);
                    printUsageAndFail(program);
                }
            }
            currentSeed = strtoul(arg, 0L, 10);
        }
    }

    if (argc > 0) {
        TraceLog(LOG_ERROR, "Too many arguments provided\n");
        printUsageAndFail(program);
    }
    

    InitWindow(WIDTH, HEIGHT, "Apollonian Gasket");

    SetTargetFPS(60);

    // Setup
    currentSeed = reset(currentSeed);

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(colorGetBackground());

            // Display all circles
            for (int i = 0; i < arrlen(allCircles); ++i) {
                circleDraw(&allCircles[i], colorGetForeground());
            }

            if (!done) {
                // The amount of circles before generating new ones
                int lenBefore = arrlen(allCircles);

                // Generate the next generation of circles
                nextGeneration();

                // The amount of circles that has been generated this frame
                int lenDiff = arrlen(allCircles) - lenBefore;

                // We are done if no new circles have generated
                if (lenDiff == 0) {
                    done = true;

                    // The queue isn't needed anymore, we can free it just in case
                    arrfree(queue);
                }
            }

            int fontSize = 24;
            int y = 5;
            // Display some information, like FPS and the current seed
            DrawText("FPS", 5, y, fontSize, colorGetForeground());
            y += fontSize;
            DrawText(TextFormat("%d", GetFPS()), 5, y, fontSize, colorGetForeground());
            y += fontSize * 1.5;
            DrawText("Seed", 5, y, fontSize, colorGetForeground());
            y += fontSize;
            DrawText(TextFormat("%lu", currentSeed), 5, y, fontSize, colorGetForeground());
            // Display "Done!" if we are done generating
            if (done) {
                y += fontSize * 1.5;
                DrawText("Done!", 5, y, fontSize, colorGetForeground());
            }

            // Start over with a new random seed if the user presses the space bar
            if (IsKeyPressed(KEY_SPACE))
                currentSeed = reset(0);

        EndDrawing();
    }

    // Free all of the memory used by the circles
    arrfree(allCircles);
    arrfree(queue);

    CloseWindow();
    return 0;
}