#include <SFML/Graphics.hpp>
#include "../include/bd/Boids.h"
#include "../include/bd/Settings.h"
#include "../include/bd/Renderer.h"

int main() {
    // 1. Configuration
    bd::Settings settings;
    sf::RenderWindow window(sf::VideoMode(settings.windowWidth, settings.windowHeight), "Boids ");
    window.setFramerateLimit(60); // Important pour ne pas tourner à 2000 FPS

    // 2. Création des objets
    bd::Renderer renderer(window);

    // Créons un boid au centre, qui part vers la droite et un peu le bas
    bd::Boid monBoid(bd::Vec2<float>(400, 300), bd::Vec2<float>(3.0f, 1.5f));

    // Force nulle pour l'instant (il va juste avancer en ligne droite)
    bd::Vec2<float> forceNulle(0, 0);

    // 3. Boucle principale
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // --- MISE À JOUR (Logique) ---
        // dt = 1.0f pour simplifier (idéalement on utilise une horloge)
        monBoid.update(forceNulle, settings, 1.0f);
        monBoid.handleBounds(settings.windowWidth, settings.windowHeight);

        // --- DESSIN (Rendu) ---
        window.clear(sf::Color::Black);

        renderer.drawBoid(monBoid);

        window.display();
    }

    return 0;
}