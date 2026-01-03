#include "../../include/bd/Simulation.h"
#include "../../include/bd/CohesionRule.h"
#include "../../include/bd/SeparationRule.h"
#include "../../include/bd/AlignmentRule.h"
#include "../../include/bd/ObstacleRule.h"
#include "../../include/bd/PredatorRule.h"

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath> // pour sqrt

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

    // Création fenêtre
    window.create(
        sf::VideoMode(
            static_cast<unsigned>(settings.windowWidth),
            static_cast<unsigned>(settings.windowHeight)
        ),
        "Boids - Simulation"
    );
    window.setFramerateLimit(60);

    // Initialisation Boids
    flock.initRandom(settings.nbBoids, settings.windowWidth, settings.windowHeight, settings);

    // Initialisation Règles
    for (size_t i = 0; i < rules.size(); ++i) delete rules[i];
    rules.clear();

    rules.push_back(new CohesionRule());
    rules.push_back(new SeparationRule());
    rules.push_back(new AlignmentRule());
    // Phase 5 : Obstacle & Prédateur
    rules.push_back(new ObstacleRule());
    rules.push_back(new PredatorRule());

    // Position de départ du Prédateur
    settings.predatorX = 50.0f;
    settings.predatorY = 50.0f;

    std::cout << "=== COMMANDES ===\n";
    std::cout << "[ESC]        : Quitter\n";
    std::cout << "[ESPACE]     : Reset Positions\n";
    std::cout << "[B]          : Rebond ON/OFF\n";
    std::cout << "[CLIC DROIT] : Placer OBSTACLE\n";
    std::cout << "[P]          : Activer PREDATEUR\n";
}

void Simulation::run() {
    const float dtFixed = 1.0f;
    while (window.isOpen()) {
        handleEvents();
        update(dtFixed);
        render();
    }
}

void Simulation::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();

        // --- CLAVIER ---
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) window.close();

            // Reset
            if (event.key.code == sf::Keyboard::Space) {
                flock.initRandom(settings.nbBoids, settings.windowWidth, settings.windowHeight, settings);
            }

            // Rebond
            if (event.key.code == sf::Keyboard::B) {
                settings.enableBounce = !settings.enableBounce;
                std::cout << "Bounce: " << (settings.enableBounce ? "ON" : "OFF") << std::endl;
            }

            // Prédateur (Touche P)
            if (event.key.code == sf::Keyboard::P) {
                settings.enablePredator = !settings.enablePredator;
                std::cout << "Predateur: " << (settings.enablePredator ? "ON" : "OFF") << std::endl;
            }

            // Gestion Flèches ou Scroll pour Resize (Phase 4 optionnelle) ici...
        }

        // --- SOURIS ---
        if (event.type == sf::Event::MouseButtonPressed) {
            // Clic Droit : Placer Obstacle
            if (event.mouseButton.button == sf::Mouse::Right) {
                settings.obstacleX = static_cast<float>(event.mouseButton.x);
                settings.obstacleY = static_cast<float>(event.mouseButton.y);
                settings.enableObstacle = true;
                std::cout << "Obstacle place." << std::endl;
            }
        }
    }
}

void Simulation::update(float dt) {
    // 1. Mise à jour des Boids
    flock.updateAll(rules, settings, dt);

    // 2. IA du Prédateur (Chasse le centre du troupeau)
    if (settings.enablePredator && flock.size() > 0) {
        float avgX = 0.0f;
        float avgY = 0.0f;
        for (int i = 0; i < flock.size(); ++i) {
            avgX += flock.getBoid(i).getPosition().x;
            avgY += flock.getBoid(i).getPosition().y;
        }
        avgX /= static_cast<float>(flock.size());
        avgY /= static_cast<float>(flock.size());

        float dirX = avgX - settings.predatorX;
        float dirY = avgY - settings.predatorY;
        float len = std::sqrt(dirX*dirX + dirY*dirY);

        if (len > 0) {
            dirX /= len;
            dirY /= len;
            settings.predatorX += dirX * settings.predatorSpeed * dt;
            settings.predatorY += dirY * settings.predatorSpeed * dt;
        }
    }
}

void Simulation::render() {
    window.clear(sf::Color::Black);

    renderer.drawFlock(flock);

    // Dessin DEBUG temporaire (en attendant que Renderer le fasse)

    // Obstacle (Bleu)
    if (settings.enableObstacle) {
        sf::CircleShape obs(15.0f);
        obs.setOrigin(15.0f, 15.0f);
        obs.setPosition(settings.obstacleX, settings.obstacleY);
        obs.setFillColor(sf::Color::Blue);
        window.draw(obs);
    }

    // Prédateur (Rouge)
    if (settings.enablePredator) {
        sf::CircleShape pred(10.0f);
        pred.setOrigin(10.0f, 10.0f);
        pred.setPosition(settings.predatorX, settings.predatorY);
        pred.setFillColor(sf::Color::Red);
        window.draw(pred);
    }

    renderer.drawUI(settings);
    window.display();
}

} // namespace bd