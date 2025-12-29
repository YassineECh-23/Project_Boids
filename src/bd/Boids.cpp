//
// Created by Yassine on 27/12/2025.
//

#include "../../include/bd/Boids.h"//
// Boid.cpp - Implémentation de la classe Boid
//
#include <cmath>

namespace bd {

// ============================================================
// CONSTRUCTEURS
// ============================================================

Boid::Boid()
    : position_(0.0f, 0.0f), velocity_(0.0f, 0.0f) {
}

Boid::Boid(const Vec2<float>& pos, const Vec2<float>& vel)
    : position_(pos), velocity_(vel) {
}

// ============================================================
// MISE À JOUR
// ============================================================

void Boid::update(const Vec2<float>& totalForce,
                  const Settings& settings,
                  float dt) {
    // ÉTAPE 1 : Limiter la force à maxAccel
    Vec2<float> acceleration = totalForce;
    float accelMagnitude = acceleration.length();

    if (accelMagnitude > settings.maxAccel) {
        // Normaliser et multiplier par maxAccel
        acceleration = acceleration.normalized() * settings.maxAccel;
    }

    // ÉTAPE 2 : Mettre à jour la vitesse
    velocity_ += acceleration * dt;

    // ÉTAPE 3 : Limiter la vitesse à vmax
    float speed = velocity_.length();
    if (speed > settings.vmax) {
        // Normaliser et multiplier par vmax
        velocity_ = velocity_.normalized() * settings.vmax;
    }

    // ÉTAPE 4 : Mettre à jour la position
    position_ += velocity_ * dt;
}

// ============================================================
// GESTION DES BORDS
// ============================================================

    void Boid::handleBounds(const Settings& settings) {

    // CAS 1 : MODE REBOND (Activé par la touche B)
    if (settings.enableBounce) {
        const float margin = 10.0f; // Marge pour éviter que ça colle au bord

        // Rebond axe X
        if (position_.x < margin) {
            position_.x = margin;
            velocity_.x = std::abs(velocity_.x);  // Force vers la droite (+)
        }
        else if (position_.x > settings.windowWidth - margin) {
            position_.x = settings.windowWidth - margin;
            velocity_.x = -std::abs(velocity_.x); // Force vers la gauche (-)
        }

        // Rebond axe Y
        if (position_.y < margin) {
            position_.y = margin;
            velocity_.y = std::abs(velocity_.y);  // Force vers le bas (+)
        }
        else if (position_.y > settings.windowHeight - margin) {
            position_.y = settings.windowHeight - margin;
            velocity_.y = -std::abs(velocity_.y); // Force vers le haut (-)
        }
    }

    // CAS 2 : MODE WRAP (Téléportation classique)
    else {
        if (position_.x < 0.0f)
            position_.x = settings.windowWidth;
        else if (position_.x > settings.windowWidth)
            position_.x = 0.0f;

        if (position_.y < 0.0f)
            position_.y = settings.windowHeight;
        else if (position_.y > settings.windowHeight)
            position_.y = 0.0f;
    }
}

} // namespace bd
