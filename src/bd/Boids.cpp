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

void Boid::handleBounds(float width, float height) {
    // Option choisie : WRAP (téléportation)
    // Le boid réapparaît de l'autre côté

    if (position_.x < 0.0f) {
        position_.x = width;
    } else if (position_.x > width) {
        position_.x = 0.0f;
    }

    if (position_.y < 0.0f) {
        position_.y = height;
    } else if (position_.y > height) {
        position_.y = 0.0f;
    }

    /* ALTERNATIVE : REBOND (décommentez si vous préférez)

    const float margin = 10.0f;

    if (position_.x < margin) {
        position_.x = margin;
        velocity_.x = std::abs(velocity_.x);  // Rebond vers la droite
    } else if (position_.x > width - margin) {
        position_.x = width - margin;
        velocity_.x = -std::abs(velocity_.x); // Rebond vers la gauche
    }

    if (position_.y < margin) {
        position_.y = margin;
        velocity_.y = std::abs(velocity_.y);  // Rebond vers le bas
    } else if (position_.y > height - margin) {
        position_.y = height - margin;
        velocity_.y = -std::abs(velocity_.y); // Rebond vers le haut
    }
    */
}

} // namespace bd
