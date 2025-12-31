#ifndef BD_SIMULATION_H
#define BD_SIMULATION_H

#include <SFML/Graphics.hpp>
#include "Settings.h"
#include "Flock.h"
#include "DynamicArray.h"
#include "Rule.h"
#include "Renderer.h"

namespace bd {

    class Simulation {
    public:
        explicit Simulation(const Settings& s);
        ~Simulation();

        void run();

    private:
        sf::RenderWindow window;
        Settings settings;
        Flock flock;
        DynamicArray<Rule*> rules;
        Renderer renderer;

        void init();
        void handleEvents();
        void update(float dt);
        void render();
    };

} // namespace bd

#endif
