#pragma once
#include <stdbool.h>
#include "raylib.h"

// Uncomment the following line to enable dark mode
// #define DARK_MODE

// Light mode colors
#define BACKGROUND_COLOR_LIGHT (Color) { 210, 210, 210, 255 }
#define FOREGROUND_COLOR_LIGHT (Color) { 25, 25, 25, 255 }

// Dark mode colors
#define BACKGROUND_COLOR_DARK (Color) { 25, 25, 25, 255 }
#define FOREGROUND_COLOR_DARK (Color) { 230, 230, 230, 255 }

void colorSetDarkMode(bool isDarkMode);

Color colorGetBackground();
Color colorGetForeground();
