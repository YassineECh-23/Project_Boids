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
        /**
         * Constructeur.
         * Initialise la simulation à partir des paramètres fournis.
         */
        explicit Simulation(const Settings& s);

        /**
         * Destructeur.
         * Libère les ressources utilisées par la simulation.
         */
        ~Simulation();

        /**
         * Lance la boucle principale de la simulation.
         */
        void run();

        /**
         * Charge une simulation depuis un fichier de sauvegarde.
         *
         * @param filename Nom du fichier contenant l’état sauvegardé
         */
        void loadFromFile(const std::string& filename);

    private:
        sf::RenderWindow window;     // Fenêtre principale SFML
        Settings settings;           // Paramètres globaux de la simulation
        Flock flock;                 // Groupe de boids simulés
        DynamicArray<Rule*> rules;   // Ensemble des règles de comportement
        Renderer renderer;           // Responsable de l’affichage graphique

        /**
         * Initialise la fenêtre, le flock, les règles et le renderer.
         */
        void init();

        /**
         * Gère les événements SFML (clavier, souris, fermeture).
         */
        void handleEvents();

        /**
         * Met à jour la simulation.
         *
         * @param dt Temps écoulé depuis la dernière frame (delta time)
         */
        void update(float dt);

        /**
         * Dessine l’état courant de la simulation à l’écran.
         */
        void render();
    };

} // namespace bd

#endif // BD_SIMULATION_H
