#ifndef BD_RENDERER_H
#define BD_RENDERER_H

#include <SFML/Graphics.hpp>

#include "Boids.h"
#include "Flock.h"
#include "Settings.h"

namespace bd {

    /**
     * @brief Classe responsable du rendu graphique de la simulation.
     *
     * Le Renderer s'occupe de dessiner les entités (Boids) et l'interface utilisateur (UI)
     * sur la fenêtre SFML. Il gère les formes géométriques et les polices d'écriture.
     */
    class Renderer {
    public:
        /**
         * @brief Constructeur.
         *
         * Initialise les formes graphiques (ex: triangle pour les boids) et charge
         * les ressources nécessaires comme la police d'écriture pour l'UI.
         *
         * @param window Référence vers la fenêtre SFML où le rendu sera effectué.
         */
        explicit Renderer(sf::RenderWindow& window);

        /**
         * @brief Dessine un boid individuel.
         *
         * Positionne et oriente la forme graphique selon la position et la vitesse du boid,
         * puis la dessine dans la fenêtre.
         *
         * @param boid Le boid à afficher.
         */
        void drawBoid(const Boid& boid);

        /**
         * @brief Dessine l'ensemble du groupe (Flock).
         *
         * Parcourt la liste des boids du flock et appelle drawBoid() pour chacun d'eux.
         *
         * @param flock Le groupe de boids à dessiner.
         */
        void drawFlock(const Flock& flock);

        /**
         * @brief Affiche l'interface utilisateur (UI) superposée.
         *
         * Affiche les valeurs actuelles des paramètres (nombre de boids, poids des règles, etc.)
         * sous forme de texte à l'écran.
         *
         * @param settings Les paramètres de la simulation à afficher.
         */
        void drawUI(const Settings& settings);

    private:
        sf::RenderWindow& window_;    ///< Référence vers la fenêtre de rendu SFML.
        sf::ConvexShape boidShape_;   ///< Forme géométrique (polygone) représentant un boid.

        // UI
        sf::Font font_;               ///< Police d'écriture utilisée pour le texte.
        bool fontLoaded_ = false;     ///< Indique si le chargement de la police a réussi.
    };

} // namespace bd

#endif