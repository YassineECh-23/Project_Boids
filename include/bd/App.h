#pragma once
#include <SFML/Graphics.hpp>

#include "Flock.h"
#include "Settings.h"
#include "Renderer.h"
#include "DynamicArray.h"
#include "Rule.h"

namespace bd {

    struct App {
        Settings settings;
        sf::RenderWindow window;
        Renderer renderer;

        Flock flock;
        DynamicArray<Rule*> rules;

        App();          // construit window/renderer
        ~App();         // delete rules

        void init();    // srand + settings + flock + rules
        void handleEvents();
        void update(float dt);
        void render();
    };

} // namespace bd
