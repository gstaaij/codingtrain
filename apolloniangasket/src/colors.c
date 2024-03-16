#include "colors.h"

static Color backgroundColor = BACKGROUND_COLOR_LIGHT;
static Color foregroundColor = FOREGROUND_COLOR_LIGHT;

void colorSetDarkMode(bool isDarkMode) {
    backgroundColor = isDarkMode ? BACKGROUND_COLOR_DARK : BACKGROUND_COLOR_LIGHT;
    foregroundColor = isDarkMode ? FOREGROUND_COLOR_DARK : FOREGROUND_COLOR_LIGHT;
}

Color colorGetBackground() {
    return backgroundColor;
}

Color colorGetForeground() {
    return foregroundColor;
}
