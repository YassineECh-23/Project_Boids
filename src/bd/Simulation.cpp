#include "../../include/bd/Simulation.h"
#include "../../include/bd/CohesionRule.h"
#include "../../include/bd/SeparationRule.h"
#include "../../include/bd/AlignmentRule.h"
#include "../../include/bd/ObstacleRule.h"
#include "../../include/bd/PredatorRule.h"

// Nécessaire pour la sauvegarde automatique et le chargement
#include "../../include/bd/SaveManager.h"

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
    // On vérifie si le flock est vide pour ne pas écraser un chargement fait via le menu
    if (flock.size() == 0) {
        flock.initRandom(settings.nbBoids, settings.windowWidth, settings.windowHeight, settings);
    }

    // Initialisation Règles
    for (size_t i = 0; i < rules.size(); ++i) delete rules[i];
    rules.clear();

    rules.push_back(new CohesionRule());
    rules.push_back(new SeparationRule());
    rules.push_back(new AlignmentRule());

    // Règles Phase 5 : Obstacle & Prédateur
    rules.push_back(new ObstacleRule());
    rules.push_back(new PredatorRule());

    // Position de départ du Prédateur
    settings.predatorX = 50.0f;
    settings.predatorY = 50.0f;

    std::cout << "=== COMMANDES SIMULATION ===\n";
    std::cout << "[ESC]        : Quitter\n";
    std::cout << "[ESPACE]     : Reset Positions\n";
    std::cout << "[S]          : SAUVEGARDE AUTO (dans /assets/saves/)\n";
    std::cout << "[B]          : Rebond ON/OFF\n";
    std::cout << "[CLIC DROIT] : Placer OBSTACLE\n";
    std::cout << "[P]          : Activer PREDATEUR\n";
}

// Fonction appelée par le bouton LOAD du menu
void Simulation::loadFromFile(const std::string& filename) {
    // On appelle la méthode statique de chargement du SaveManager
    if (SaveManager::load(filename, settings, flock)) {

        // Si la résolution a changé dans le fichier, on adapte la fenêtre
        window.setSize(sf::Vector2u(static_cast<unsigned>(settings.windowWidth),
                                    static_cast<unsigned>(settings.windowHeight)));
        window.setView(sf::View(sf::FloatRect(0, 0, settings.windowWidth, settings.windowHeight)));

        std::cout << "Simulation chargee avec succes : " << filename << std::endl;
    }
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

            // --- SAUVEGARDE AUTOMATIQUE (Touche S) ---
            if (event.key.code == sf::Keyboard::S) {
                // C'est ici qu'on appelle saveAuto au lieu de save
                SaveManager::saveAuto(settings, flock);
            }
        }

        // --- SOURIS ---
        if (event.type == sf::Event::MouseButtonPressed) {
            // Clic Droit : Placer Obstacle
            if (event.mouseButton.button == sf::Mouse::Right) {
                settings.obstacleX = static_cast<float>(event.mouseButton.x);
                settings.obstacleY = static_cast<float>(event.mouseButton.y);
                settings.enableObstacle = true;
                std::cout << "Obstacle place en (" << settings.obstacleX << ", " << settings.obstacleY << ")\n";
            }
        }
    }
}

void Simulation::update(float dt) {
    // 1. Mise à jour des Boids
    flock.updateAll(rules, settings, dt);

    // 2. IA du Prédateur (il chasse le centre du troupeau)
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

    // Dessin des boids
    renderer.drawFlock(flock);

    // Dessin de l'Obstacle (Cercle Bleu)
    if (settings.enableObstacle) {
        sf::CircleShape obs(15.0f);
        obs.setOrigin(15.0f, 15.0f);
        obs.setPosition(settings.obstacleX, settings.obstacleY);
        obs.setFillColor(sf::Color::Blue);
        window.draw(obs);
    }

    // Dessin du Prédateur (Cercle Rouge)
    if (settings.enablePredator) {
        sf::CircleShape pred(10.0f);
        pred.setOrigin(10.0f, 10.0f);
        pred.setPosition(settings.predatorX, settings.predatorY);
        pred.setFillColor(sf::Color::Red);
        window.draw(pred);
    }

    // Interface
    renderer.drawUI(settings);

    window.display();
}

} // namespace bd