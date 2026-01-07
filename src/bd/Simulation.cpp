#include "../../include/bd/Simulation.h"
#include "../../include/bd/CohesionRule.h"
#include "../../include/bd/SeparationRule.h"
#include "../../include/bd/AlignmentRule.h"
#include "../../include/bd/ObstacleRule.h"
#include "../../include/bd/PredatorRule.h"
#include "../../include/bd/SaveManager.h"

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>

namespace bd {

    /**
     * Constructeur de Simulation.
     * Initialise les attributs principaux (fenêtre, settings, flock, règles, renderer)
     * puis appelle init() pour créer la fenêtre SFML, initialiser le flock et les règles.
     *
     * @param s Paramètres de simulation à utiliser
     */
    Simulation::Simulation(const Settings& s)
        : window(),
          settings(s),
          flock(),
          rules(),
          renderer(window) {
        init();
    }

    /**
     * Destructeur de Simulation.
     * Libère la mémoire des règles créées dynamiquement (new ...).
     */
    Simulation::~Simulation() {
        for (size_t i = 0; i < rules.size(); ++i) delete rules[i];
    }

    /**
     * Initialise la simulation :
     * - initialise la seed du random
     * - crée la fenêtre SFML selon la taille dans settings
     * - initialise le flock si vide (boids aléatoires)
     * - reconstruit la liste des règles (cohésion, séparation, alignement, obstacle, prédateur)
     * - initialise la position de départ du prédateur
     * - affiche les commandes disponibles dans la console
     */
    void Simulation::init() {
        std::srand((unsigned)std::time(nullptr));

        window.create(
            sf::VideoMode(
                static_cast<unsigned>(settings.windowWidth),
                static_cast<unsigned>(settings.windowHeight)
            ),
            "Boids - Simulation"
        );
        window.setFramerateLimit(60);

        if (flock.size() == 0) {
            flock.initRandom(settings.nbBoids, settings.windowWidth, settings.windowHeight, settings);
        }

        // Nettoyage + reconstruction des règles
        for (size_t i = 0; i < rules.size(); ++i) delete rules[i];
        rules.clear();

        rules.push_back(new CohesionRule());
        rules.push_back(new SeparationRule());
        rules.push_back(new AlignmentRule());

        rules.push_back(new ObstacleRule());
        rules.push_back(new PredatorRule());

        // Position initiale du prédateur
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

    /**
     * Charge une configuration depuis un fichier (appelée depuis le menu LOAD).
     * - charge les settings via SaveManager
     * - réinitialise le flock avec le nouveau nbBoids et nouvelles dimensions
     * - redimensionne la fenêtre + la vue SFML pour correspondre aux nouvelles dimensions
     *
     * @param filename Chemin du fichier de configuration à charger
     */
    void Simulation::loadFromFile(const std::string& filename) {
        if (SaveManager::load(filename, settings, flock)) {
            flock.initRandom(settings.nbBoids, settings.windowWidth, settings.windowHeight, settings);

            window.setSize(sf::Vector2u(
                static_cast<unsigned>(settings.windowWidth),
                static_cast<unsigned>(settings.windowHeight)
            ));

            window.setView(sf::View(sf::FloatRect(0, 0, settings.windowWidth, settings.windowHeight)));

            std::cout << "Simulation reinitialisee avec la config : " << filename << std::endl;
        }
    }

    /**
     * Boucle principale de la simulation.
     * Tant que la fenêtre est ouverte :
     * - traite les événements
     * - met à jour la simulation
     * - dessine la scène
     */
    void Simulation::run() {
        const float dtFixed = 1.0f;
        while (window.isOpen()) {
            handleEvents();
            update(dtFixed);
            render();
        }
    }

    /**
     * Gère les événements SFML (clavier et souris) pendant la simulation :
     * - ESC : quitter
     * - SPACE : reset des positions des boids
     * - B : activer/désactiver le rebond sur les bords
     * - P : activer/désactiver le prédateur
     * - S : sauvegarde automatique des settings
     * - clic droit souris : placer un obstacle et l’activer
     */
    void Simulation::handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) window.close();

                if (event.key.code == sf::Keyboard::Space) {
                    flock.initRandom(settings.nbBoids, settings.windowWidth, settings.windowHeight, settings);
                }

                if (event.key.code == sf::Keyboard::B) {
                    settings.enableBounce = !settings.enableBounce;
                    std::cout << "Bounce: " << (settings.enableBounce ? "ON" : "OFF") << std::endl;
                }

                if (event.key.code == sf::Keyboard::P) {
                    settings.enablePredator = !settings.enablePredator;
                    std::cout << "Predateur: " << (settings.enablePredator ? "ON" : "OFF") << std::endl;
                }

                if (event.key.code == sf::Keyboard::S) {
                    SaveManager::saveAuto(settings, flock);
                }
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Right) {
                    settings.obstacleX = static_cast<float>(event.mouseButton.x);
                    settings.obstacleY = static_cast<float>(event.mouseButton.y);
                    settings.enableObstacle = true;
                    std::cout << "Obstacle place en (" << settings.obstacleX << ", " << settings.obstacleY << ")\n";
                }
            }
        }
    }

    /**
     * Met à jour l’état de la simulation :
     * - calcule et applique les forces aux boids via Flock::updateAll(...)
     * - si le prédateur est activé, met à jour sa position en se dirigeant
     *   vers le centre moyen du flock (comportement de poursuite simple)
     *
     * @param dt Delta time (temps écoulé depuis la dernière update)
     */
    void Simulation::update(float dt) {
        flock.updateAll(rules, settings, dt);

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
            float len = std::sqrt(dirX * dirX + dirY * dirY);

            if (len > 0) {
                dirX /= len;
                dirY /= len;
                settings.predatorX += dirX * settings.predatorSpeed * dt;
                settings.predatorY += dirY * settings.predatorSpeed * dt;
            }
        }
    }

    /**
     * Dessine une frame complète :
     * - efface l’écran
     * - dessine les boids via le renderer
     * - dessine l’obstacle si activé
     * - dessine le prédateur si activé
     * - dessine l’UI (valeurs des settings)
     * - affiche la frame (display)
     */
    void Simulation::render() {
        window.clear(sf::Color::Black);

        renderer.drawFlock(flock);

        if (settings.enableObstacle) {
            sf::CircleShape obs(15.0f);
            obs.setOrigin(15.0f, 15.0f);
            obs.setPosition(settings.obstacleX, settings.obstacleY);
            obs.setFillColor(sf::Color::Blue);
            window.draw(obs);
        }

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
