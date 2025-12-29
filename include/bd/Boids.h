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
 * @brief Agent individuel (boid)
 *
 * Un boid possède :
 * - Une position (Vec2<float>)
 * - Une vitesse (Vec2<float>)
 *
 * Il suit les règles de comportement appliquées par Flock.
 */
class Boid {
private:
    Vec2<float> position_;
    Vec2<float> velocity_;

public:
    // ============================================================
    // CONSTRUCTEURS
    // ============================================================

    /// Constructeur par défaut
    Boid();

    /// Constructeur avec position et vitesse
    Boid(const Vec2<float>& pos, const Vec2<float>& vel);

    // ============================================================
    // MISE À JOUR
    // ============================================================

    /**
     * @brief Met à jour la vitesse et la position
     *
     * Algorithme (page 2 du sujet) :
     * 1. Limiter totalForce à maxAccel
     * 2. velocity += totalForce * dt
     * 3. Limiter velocity à vmax
     * 4. position += velocity * dt
     */
    void update(const Vec2<float>& totalForce,
                const Settings& settings,
                float dt);

    /**
     * @brief Gère les bords de la fenêtre
     *
     * Options :
     * - WRAP : téléporte de l'autre côté
     * - REBOND : inverse la vitesse
     */
    void handleBounds(const Settings& settings);

    // ============================================================
    // ACCESSEURS
    // ============================================================

    const Vec2<float>& getPosition() const { return position_; }
    const Vec2<float>& getVelocity() const { return velocity_; }

    void setPosition(const Vec2<float>& pos) { position_ = pos; }
    void setVelocity(const Vec2<float>& vel) { velocity_ = vel; }
};

} // namespace bd

#endif // BD_BOID_H