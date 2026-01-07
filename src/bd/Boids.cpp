#include "../../include/bd/Boids.h"//
// Boid.cpp - Implémentation de la classe Boid
//
#include <cmath>

namespace bd {

    Boid::Boid()
        : position_(0.0f, 0.0f), velocity_(0.0f, 0.0f) {
    }

    Boid::Boid(const Vec2<float>& pos, const Vec2<float>& vel)
        : position_(pos), velocity_(vel) {
    }

    /**
     * Met à jour l'état du boid en fonction de la force totale appliquée.
     * - La force est interprétée comme une accélération
     * - L'accélération est limitée par maxAccel
     * - La vitesse est mise à jour puis limitée par vmax
     * - La position est ensuite mise à jour selon la vitesse
     *
     * @param totalForce Force résultante calculée par les règles (alignement, cohésion, séparation, obstacles, etc.)
     * @param settings   Paramètres globaux (vmax, maxAccel, etc.)
     * @param dt         Delta time (temps écoulé depuis la dernière frame)
     */
    void Boid::update(const Vec2<float>& totalForce,const Settings& settings,float dt) {
        Vec2<float> acceleration = totalForce;
        float accelMagnitude = acceleration.length();

        if (accelMagnitude > settings.maxAccel) {
            acceleration = acceleration.normalized() * settings.maxAccel;
        }
        velocity_ += acceleration * dt;
        float speed = velocity_.length();
        if (speed > settings.vmax) {
            velocity_ = velocity_.normalized() * settings.vmax;
        }
        position_ += velocity_ * dt;
    }

    /**
     * Gère le comportement du boid lorsqu'il atteint les limites de la fenêtre.
     * Deux modes possibles selon les paramètres :
     * - enableBounce = true  : rebond sur les bords
     * - enableBounce = false :  (sort d'un côté, réapparaît de l'autre)
     *
     * @param settings Paramètres globaux (dimensions fenêtre, mode rebond, etc.)
     */
    void Boid::handleBounds(const Settings& settings) {
    if (settings.enableBounce) {
        const float margin = 10.0f;


        if (position_.x < margin) {
            position_.x = margin;
            velocity_.x = std::abs(velocity_.x);
        }
        else if (position_.x > settings.windowWidth - margin) {
            position_.x = settings.windowWidth - margin;
            velocity_.x = -std::abs(velocity_.x);
        }


        if (position_.y < margin) {
            position_.y = margin;
            velocity_.y = std::abs(velocity_.y);
        }
        else if (position_.y > settings.windowHeight - margin) {
            position_.y = settings.windowHeight - margin;
            velocity_.y = -std::abs(velocity_.y);
        }
    }

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
