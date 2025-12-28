#ifndef BD_RENDERER_H
#define BD_RENDERER_H

#include <SFML/Graphics.hpp>
#include "Boids.h"
#include "Settings.h"

namespace bd {

    class Renderer {
    public:
        Renderer(sf::RenderWindow& window);

        // Dessine un seul boid
        void drawBoid(const Boid& boid);

        // (Optionnel pour ce test) Affiche les infos de debug
        void drawDebug(const Settings& settings);

    private:
        sf::RenderWindow& window_;
        sf::ConvexShape boidShape_; // Forme du boid (triangle)
    };

} // namespace bd

#endif // BD_RENDERER_H