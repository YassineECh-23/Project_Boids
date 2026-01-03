#include "../../include/bd/ObstacleRule.h"
#include "../../include/bd/Flock.h"

namespace bd {

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

        // Si je suis hors de la zone de danger, je m'en fiche
        if (dist > settings.obstacleRadius || dist <= 0.0001f) {
            return Vec2<float>(0.0f, 0.0f);
        }

        // Si je suis dans la zone : PANIQUE !
        // Je veux aller à Vmax dans la direction opposée à l'obstacle
        Vec2<float> desired = away.normalized() * settings.vmax;

        // Steering force = Desired - Velocity
        return desired - me.getVelocity();
    }

} // namespace bd