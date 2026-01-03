#include "../../include/bd/Simulation.h"
#include "../../include/bd/CohesionRule.h"
#include "../../include/bd/SeparationRule.h"
#include "../../include/bd/AlignmentRule.h"

#include <iostream>
#include <ctime>
#include <cstdlib>

namespace bd {

Simulation::Simulation(const Settings& s)
    : window(),
      settings(s),
      flock(),
      rules(),
      renderer(window) {
    init();
}

Simulation::~Simulation() {
    for (size_t i = 0; i < rules.size(); ++i) delete rules[i];
}

void Simulation::init() {
    std::srand((unsigned)std::time(nullptr));

    // Fenêtre
    window.create(
        sf::VideoMode(
            static_cast<unsigned>(settings.windowWidth),
            static_cast<unsigned>(settings.windowHeight)
        ),
        "Boids - Simulation"
    );
    window.setFramerateLimit(60);

    // Boids
    flock.initRandom(settings.nbBoids, settings.windowWidth, settings.windowHeight, settings);

    // Règles (safe si init() est rappelé plus tard)
    for (size_t i = 0; i < rules.size(); ++i) delete rules[i];
    rules.clear();

    rules.push_back(new CohesionRule());
    rules.push_back(new SeparationRule());
    rules.push_back(new AlignmentRule());

    std::cout << "=== COMMANDES ===\n";
    std::cout << "[ESC]    : Quitter la simulation\n";
    std::cout << "[ESPACE] : Reinitialiser les positions\n";
    std::cout << "[B]      : Activer/Desactiver le REBOND (Bounce)\n";
}

void Simulation::run() {
    //  Identique à App : même vitesse / même comportement
    const float dtFixed = 1.0f;

    while (window.isOpen()) {
        handleEvents();
        update(dtFixed);   // logique uniquement
        render();          // affichage uniquement
    }
}

void Simulation::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape)
                window.close();

            if (event.key.code == sf::Keyboard::Space) {
                flock.initRandom(settings.nbBoids, settings.windowWidth, settings.windowHeight, settings);
            }

            if (event.key.code == sf::Keyboard::B) {
                settings.enableBounce = !settings.enableBounce;
            }
        }
    }
}

void Simulation::update(float dt) {
    flock.updateAll(rules, settings, dt);
}

void Simulation::render() {
    window.clear(sf::Color::Black);

    renderer.drawFlock(flock);
    renderer.drawUI(settings);

    window.display();
}

} // namespace bd
