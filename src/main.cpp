#include <cstdlib>
#include "common/main_window.h"

int main () {
    SceneWindow window;

    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}