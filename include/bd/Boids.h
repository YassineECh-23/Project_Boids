//
// Boid.h - Classe représentant un agent individuel
// Un boid a une position et une vitesse
//

#ifndef BD_BOID_H
#define BD_BOID_H

#include "Vec2.h"
#include "Settings.h"

namespace bd {

/**
 * @brief Agent individuel (boid).
 *
 * Un boid possède :
 * - Une position (Vec2<float>)
 * - Une vitesse (Vec2<float>)
 *
 * Il suit les règles de comportement (cohésion, séparation, alignement)
 * appliquées par le Flock.
 */
class Boid {
private:
    Vec2<float> position_;
    Vec2<float> velocity_;

public:
    /**
     * @brief Constructeur par défaut.
     * Initialise la position et la vitesse à (0,0).
     */
    Boid();

    /**
     * @brief Constructeur avec position et vitesse initiales.
     * @param pos La position de départ du boid.
     * @param vel La vitesse de départ du boid.
     */
    Boid(const Vec2<float>& pos, const Vec2<float>& vel);


    /**
     * @brief Met à jour la vitesse et la position du boid.
     *
     * Applique la force résultante (steering force) pour modifier l'accélération,
     * puis la vitesse et enfin la position, en respectant les contraintes physiques.
     *
     * Algorithme :
     * 1. Limiter totalForce à maxAccel.
     * 2. velocity += totalForce * dt.
     * 3. Limiter velocity à vmax.
     * 4. position += velocity * dt.
     *
     * @param totalForce La somme vectorielle de toutes les forces appliquées.
     * @param settings   Les paramètres globaux (vmax, maxAccel, etc.).
     * @param dt         Le pas de temps (delta time) pour l'intégration.
     */
    void update(const Vec2<float>& totalForce,
                const Settings& settings,
                float dt);

    /**
     * @brief Gère les interactions avec les bords de la fenêtre.
     *
     * Selon la configuration (Settings) :
     * - WRAP : Le boid traverse l'écran et réapparaît de l'autre côté.
     * - REBOND : Le boid rebondit contre les parois (inverse sa vitesse).
     *
     * @param settings Les paramètres contenant les dimensions de la fenêtre et le mode de gestion.
     */
    void handleBounds(const Settings& settings);


    /**
     * @brief Récupère la position actuelle.
     * @return const Vec2<float>& Référence constante vers le vecteur position.
     */
    const Vec2<float>& getPosition() const { return position_; }

    /**
     * @brief Récupère la vitesse actuelle.
     * @return const Vec2<float>& Référence constante vers le vecteur vitesse.
     */
    const Vec2<float>& getVelocity() const { return velocity_; }

    /**
     * @brief Modifie la position du boid.
     * @param pos Nouvelle position.
     */
    void setPosition(const Vec2<float>& pos) { position_ = pos; }

    /**
     * @brief Modifie la vitesse du boid.
     * @param vel Nouvelle vitesse.
     */
    void setVelocity(const Vec2<float>& vel) { velocity_ = vel; }
};

} // namespace bd

#endif // BD_BOID_H