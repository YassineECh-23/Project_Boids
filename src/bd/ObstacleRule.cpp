#include "../../include/bd/ObstacleRule.h"
#include "../../include/bd/Flock.h"

namespace bd {

    /**
     * Calcule la force d’évitement d’obstacle appliquée à un boid.
     * - Si l’option obstacle est désactivée : retourne (0,0)
     * - Si le boid est en dehors du rayon d’influence : retourne (0,0)
     * - Sinon : génère une force de "steering" qui pousse le boid à fuir l’obstacle
     *   en visant une vitesse désirée orientée à l’opposé de l’obstacle.
     *
     * @param flock      Le groupe de boids
     * @param boidIndex  Indice du boid courant
     * @param settings   Paramètres globaux (activation obstacle, position, rayon, vmax, etc.)
     * @return           Vecteur force d’évitement (steering) pour ce boid
     */
    Vec2<float> ObstacleRule::computeForce(const Flock& flock, int boidIndex, const Settings& settings) const {
        // Si l'obstacle n'est pas activé, aucune force
        if (!settings.enableObstacle) {
            return Vec2<float>(0.0f, 0.0f);
        }

        const Boid& me = flock.getBoid(boidIndex);
        Vec2<float> obstaclePos(settings.obstacleX, settings.obstacleY);

        // Vecteur qui va de l'obstacle VERS le boid (sens de la fuite)
        Vec2<float> away = me.getPosition() - obstaclePos;
        float dist = away.length();

        // Si le boid est hors de la zone d'influence (ou distance quasi nulle), aucune force
        if (dist > settings.obstacleRadius || dist <= 0.0001f) {
            return Vec2<float>(0.0f, 0.0f);
        }

        // Si le boid est dans la zone : on calcule une vitesse désirée à Vmax dans la direction de fuite
        Vec2<float> desired = away.normalized() * settings.vmax;

        // Force de steering : différence entre vitesse désirée et vitesse actuelle
        return desired - me.getVelocity();
    }

} // namespace bd
