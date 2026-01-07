#include "../../include/bd/Renderer.h"
#include <cmath>      // atan2
#include <sstream>
#include <iostream>

namespace bd {

    /**
     * Constructeur du Renderer.
     * Initialise la forme graphique utilisée pour représenter un boid (triangle orienté),
     * configure ses couleurs, et charge la police utilisée pour l’affichage de l’UI.
     *
     * @param window Référence vers la fenêtre SFML dans laquelle on dessine.
     */
    Renderer::Renderer(sf::RenderWindow& window) : window_(window) {
        // Triangle isocèle qui pointe vers la droite (0°)
        boidShape_.setPointCount(3);
        boidShape_.setPoint(0, sf::Vector2f(10, 0));   // Pointe
        boidShape_.setPoint(1, sf::Vector2f(-5, -5));  // Arrière gauche
        boidShape_.setPoint(2, sf::Vector2f(-5, 5));   // Arrière droite

        boidShape_.setFillColor(sf::Color::Cyan);
        boidShape_.setOutlineColor(sf::Color::White);
        boidShape_.setOutlineThickness(1.f);

        // Chargement de la police pour l'affichage des textes (UI)
        fontLoaded_ = font_.loadFromFile("../assets/Roboto-Regular.ttf");
        if (!fontLoaded_) {
            std::cerr << "[Renderer] Failed to load font ../assets/Roboto-Regular.ttf\n";
        }
    }

    /**
     * Dessine un boid.
     * - Positionne le triangle sur la position du boid
     * - Oriente le triangle selon la direction de la vitesse (atan2)
     * - Dessine la forme dans la fenêtre SFML
     *
     * @param boid Boid à dessiner
     */
    void Renderer::drawBoid(const Boid& boid) {
        Vec2<float> pos = boid.getPosition();
        Vec2<float> vel = boid.getVelocity();

        boidShape_.setPosition(pos.x, pos.y);

        float angle = std::atan2(vel.y, vel.x) * 180.0f / 3.14159f;
        boidShape_.setRotation(angle);

        window_.draw(boidShape_);
    }

    /**
     * Dessine tous les boids d’un flock.
     * Parcourt le flock et appelle drawBoid() pour chaque boid.
     *
     * @param flock Groupe de boids à dessiner
     */
    void Renderer::drawFlock(const Flock& flock) {
        for (size_t i = 0; i < flock.size(); ++i) {
            drawBoid(flock.getBoid(i));
        }
    }

    /**
     * Affiche une UI simple contenant quelques paramètres de simulation.
     * Ne fait rien si la police n’a pas été chargée correctement.
     *
     * @param settings Paramètres à afficher (nbBoids, r, dmin, poids des règles...)
     */
    void Renderer::drawUI(const Settings& settings) {
        if (!fontLoaded_) return;

        sf::Text txt;
        txt.setFont(font_);
        txt.setCharacterSize(14);
        txt.setFillColor(sf::Color::White);
        txt.setPosition(10.f, 10.f);

        std::ostringstream oss;
        oss << "nbBoids : " << settings.nbBoids << "\n"
            << "r      : " << settings.r << "\n"
            << "dmin   : " << settings.dmin << "\n"
            << "wcoh   : " << settings.wcoh << "\n"
            << "wsep   : " << settings.wsep << "\n"
            << "wali   : " << settings.wali << "\n";

        txt.setString(oss.str());
        window_.draw(txt);
    }


} // namespace bd
