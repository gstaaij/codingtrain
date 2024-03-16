#pragma once

// Uncomment the following line to enable dark mode
// #define DARK_MODE

// Light mode colors
#define BACKGROUND_COLOR (Color) { 210, 210, 210, 255 }
#define FOREGROUND_COLOR (Color) { 25, 25, 25, 255 }

// Dark mode colors
#define BACKGROUND_COLOR_DARK (Color) { 25, 25, 25, 255 }
#define FOREGROUND_COLOR_DARK (Color) { 230, 230, 230, 255 }

#ifdef DARK_MODE
    #undef BACKGROUND_COLOR
    #undef FOREGROUND_COLOR

    #define BACKGROUND_COLOR BACKGROUND_COLOR_DARK
    #define FOREGROUND_COLOR FOREGROUND_COLOR_DARK
#endif
