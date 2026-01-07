#ifndef PT_BOIDS_OBSTACLERULE_H
#define PT_BOIDS_OBSTACLERULE_H

#include "Rule.h"
#include "Settings.h"

namespace bd {

    /**
     * @brief Règle d'évitement d'obstacle (Obstacle Avoidance).
     *
     * Cette règle permet aux boids d'éviter une zone circulaire définie comme un obstacle.
     * Si un boid s'approche trop près du centre de l'obstacle, une force répulsive est générée
     * pour le pousser à s'éloigner.
     */
    class ObstacleRule : public Rule {
    public:
        /**
         * @brief Calcule la force d'évitement d'obstacle pour un boid donné.
         *
         * Si l'obstacle est activé et que le boid se trouve dans son rayon d'influence (settings.obstacleRadius),
         * une force est calculée pour diriger le boid à l'opposé de la position de l'obstacle (settings.obstacleX, settings.obstacleY).
         *
         * @param flock      Référence vers le groupe de boids.
         * @param boidIndex  Indice du boid courant.
         * @param settings   Paramètres globaux (position et rayon de l'obstacle).
         * @return Vec2<float> Le vecteur force d'évitement (steering force).
         */
        Vec2<float> computeForce(const Flock& flock, int boidIndex, const Settings& settings) const override;
    };

} // namespace bd

#endif //PT_BOIDS_OBSTACLERULE_H