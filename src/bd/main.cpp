#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>   // <--- Indispensable pour le hasard (time)
#include <cstdlib> // <--- Indispensable pour srand

#include "../../include/bd/Flock.h"
#include "../../include/bd/Settings.h"
#include "../../include/bd/Renderer.h"
#include "../../include/bd/CohesionRule.h"
#include "../../include/bd/SeparationRule.h"
#include "../../include/bd/AlignmentRule.h"

using namespace bd;

int main() {
    // 1. INITIALISER LE HASARD (IMPORTANT !)
    // Sans ça, les boids auront toujours les mêmes positions de départ (ligne)
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // 2. CONFIGURATION
    Settings settings;
    settings.windowWidth = 1200;
    settings.windowHeight = 800;
    settings.nbBoids = 100;
    settings.enableBounce = false; // On commence en mode Wrap

    // Paramètres
    settings.r = 60.0f;
    settings.dmin = 25.0f;
    settings.vmax = 5.0f;
    settings.wcoh = 0.008f;
    settings.wsep = 0.05f;
    settings.wali = 0.1f;

    sf::RenderWindow window(sf::VideoMode(settings.windowWidth, settings.windowHeight), "Test Phase 3 - Boids");
    window.setFramerateLimit(60);
    Renderer renderer(window);

    Flock flock;
    flock.initRandom(settings.nbBoids, settings.windowWidth, settings.windowHeight);

    // Règles
    DynamicArray<Rule*> rules;
    rules.push_back(new CohesionRule());
    rules.push_back(new SeparationRule());
    rules.push_back(new AlignmentRule());

    std::cout << "=== COMMANDES ===\n";
    std::cout << "[ESPACE] : Reinitialiser les positions\n";
    std::cout << "[B]      : Activer/Desactiver le REBOND (Bounce)\n";

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                // Reset
                if (event.key.code == sf::Keyboard::Space) {
                    flock.initRandom(settings.nbBoids, settings.windowWidth, settings.windowHeight);
                    std::cout << "Reset !\n";
                }

                // Toggle Rebond (Bounce)
                if (event.key.code == sf::Keyboard::B) {
                    settings.enableBounce = !settings.enableBounce; // Inverse Vrai/Faux

                    if (settings.enableBounce)
                        std::cout << "Mode: REBOND (Murs solides)\n";
                    else
                        std::cout << "Mode: WRAP (Teleportation)\n";
                }
            }
        }

        // Update
        flock.updateAll(rules, settings, 1.0f);

        // Draw
        window.clear(sf::Color::Black);
        for (int i = 0; i < flock.size(); ++i) {
            renderer.drawBoid(flock.getBoid(i));
        }
        window.display();
    }

    // Nettoyage
    for (size_t i = 0; i < rules.size(); ++i) delete rules[i];

    return 0;
}