#include "../../include/bd/InterfaceMenuSFML.h"

int main() {
    bd::InterfaceMenuSFML menu;
    menu.run();
    return 0;
}

/*
============================================================
 ANCIEN MAIN (CONSERVÉ, NON MODIFIÉ)
============================================================

#include "../../include/bd/App.h"

int main() {
    bd::App app;
    app.init();

    while (app.window.isOpen()) {
        app.handleEvents();
        app.update(1.0f);
        app.render();
    }
    return 0;
}
*/
