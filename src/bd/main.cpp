#include "../../include/bd/App.h"

int main() {
    bd::App app;
    app.init();

    while (app.window.isOpen()) {
        app.handleEvents();
        app.update(1.0f);   // tu gardes ton dt fixe
        app.render();
    }
    return 0;
}
