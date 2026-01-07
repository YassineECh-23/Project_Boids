#ifndef BD_SIMULATION_H
#define BD_SIMULATION_H

#include <SFML/Graphics.hpp>
#include "Settings.h"
#include "Flock.h"
#include "DynamicArray.h"
#include "Rule.h"
#include "Renderer.h"

namespace bd {

    /**
     * @brief Moteur principal de la simulation.
     *
     * La classe Simulation orchestre l'ensemble de l'application.
     * Elle est responsable de :
     * - La gestion de la fenêtre SFML.
     * - La boucle principale (Game Loop).
     * - La gestion du temps (Delta Time).
     * - L'instanciation des règles de comportement.
     * - La coordination entre les entrées utilisateur, la mise à jour physique et le rendu.
     */
    class Simulation {
    public:
        /**
         * @brief Constructeur.
         *
         * Initialise la simulation à partir des paramètres fournis.
         * Prépare la fenêtre, alloue les règles et initialise le flock.
         *
         * @param s Les paramètres de configuration (Settings).
         */
        explicit Simulation(const Settings& s);

        /**
         * @brief Destructeur.
         *
         * Libère les ressources dynamiques, notamment les pointeurs de règles
         * stockés dans le tableau dynamique.
         */
        ~Simulation();

        /**
         * @brief Lance la boucle principale de la simulation.
         *
         * Cette méthode est bloquante. Elle exécute la boucle "Input -> Update -> Render"
         * tant que la fenêtre reste ouverte. Elle calcule également le temps écoulé (dt)
         * entre deux images pour assurer une simulation fluide.
         */
        void run();

        /**
         * @brief Charge une simulation depuis un fichier de sauvegarde.
         *
         * Écrase l'état actuel (Flock et Settings) avec les données lues dans le fichier.
         *
         * @param filename Nom du fichier (chemin relatif ou absolu) contenant l’état sauvegardé.
         */
        void loadFromFile(const std::string& filename);

    private:
        sf::RenderWindow window;     ///< Fenêtre principale SFML pour l'affichage.
        Settings settings;           ///< Copie locale des paramètres globaux de la simulation.
        Flock flock;                 ///< Le groupe de boids (agents) simulés.
        DynamicArray<Rule*> rules;   ///< Liste polymorphique des règles de comportement actives.
        Renderer renderer;           ///< Objet responsable de l’affichage graphique des entités.

        /**
         * @brief Initialise les composants internes.
         *
         * Configure la fenêtre, instancie les règles (Cohésion, Séparation, Alignement, etc.)
         * et génère le flock initial.
         */
        void init();

        /**
         * @brief Gère les événements SFML.
         *
         * Traite les entrées clavier (Z, S, P, Espace...) et souris (Clic droit),
         * ainsi que la fermeture de la fenêtre.
         */
        void handleEvents();

        /**
         * @brief Met à jour la logique de la simulation.
         *
         * Appelle la méthode updateAll() du Flock en lui passant les règles et le temps écoulé.
         *
         * @param dt Temps écoulé depuis la dernière frame (Delta Time) en secondes.
         */
        void update(float dt);

        /**
         * @brief Dessine l’état courant de la simulation à l’écran.
         *
         * Efface la fenêtre, demande au Renderer de dessiner le flock et l'UI,
         * puis affiche le résultat (display).
         */
        void render();
    };

} // namespace bd

#endif // BD_SIMULATION_H