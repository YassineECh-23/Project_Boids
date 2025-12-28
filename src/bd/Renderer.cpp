#include "../../include/bd/Renderer.h"
#include <cmath> // pour atan2

namespace bd {

    Renderer::Renderer(sf::RenderWindow& window) : window_(window) {
        // On construit un triangle isocèle qui pointe vers la droite (0°)
        // Sommet (tête) à (10, 0), base à (-5, -5) et (-5, 5)
        // La taille est arbitraire pour le test
        boidShape_.setPointCount(3);
        boidShape_.setPoint(0, sf::Vector2f(10, 0));  // Pointe
        boidShape_.setPoint(1, sf::Vector2f(-5, -5)); // Arrière gauche
        boidShape_.setPoint(2, sf::Vector2f(-5, 5));  // Arrière droite
        
        boidShape_.setFillColor(sf::Color::Cyan);
        boidShape_.setOutlineColor(sf::Color::White);
        boidShape_.setOutlineThickness(1);
    }

    void Renderer::drawBoid(const Boid& boid) {
        // 1. Récupérer position et vitesse
        Vec2<float> pos = boid.getPosition();
        Vec2<float> vel = boid.getVelocity();

        // 2. Appliquer la position
        boidShape_.setPosition(pos.x, pos.y);

        // 3. Calculer l'angle de rotation (en degrés)
        // atan2(y, x) donne l'angle en radians. 
        // Conversion : radians * 180 / PI
        float angle = std::atan2(vel.y, vel.x) * 180.0f / 3.14159f;
        
        boidShape_.setRotation(angle);

        // 4. Dessiner
        window_.draw(boidShape_);
    }

    void Renderer::drawDebug(const Settings& settings) {
        // On fera l'affichage de texte plus tard (besoin de charger une police)
    }

} // namespace bd