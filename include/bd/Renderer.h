#ifndef BD_RENDERER_H
#define BD_RENDERER_H

#include <SFML/Graphics.hpp>

#include "Boids.h"
#include "Flock.h"
#include "Settings.h"

namespace bd {

    class Renderer {
    public:
        explicit Renderer(sf::RenderWindow& window);

        // Dessine 1 boid
        void drawBoid(const Boid& boid);

        //  PHASE 3 : Dessine tout le flock
        void drawFlock(const Flock& flock);

        //  UI : afficher les paramètres
        void drawUI(const Settings& settings);

        // Ancien debug (tu peux garder)
        void drawDebug(const Settings& settings);

    private:
        sf::RenderWindow& window_;
        sf::ConvexShape boidShape_;

        // UI
        sf::Font font_;
        bool fontLoaded_ = false;
    };

} // namespace bd

#endif
