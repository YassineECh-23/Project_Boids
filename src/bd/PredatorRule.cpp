#include "../../include/bd/PredatorRule.h"
#include "../../include/bd/Flock.h"

namespace bd {

    Vec2<float> PredatorRule::computeForce(const Flock& flock, int boidIndex, const Settings& settings) const {
        if (!settings.enablePredator) {
            return Vec2<float>(0.0f, 0.0f);
        }

        const Boid& me = flock.getBoid(boidIndex);
        Vec2<float> predatorPos(settings.predatorX, settings.predatorY);

        // Vecteur du prédateur VERS le boid (Fuir !)
        Vec2<float> away = me.getPosition() - predatorPos;
        float dist = away.length();

        // Si le prédateur est loin, pas de panique
        if (dist > settings.predatorRadius || dist <= 0.0001f) {
            return Vec2<float>(0.0f, 0.0f);
        }

        // Sinon : PANIQUE MAXIMALE
        // Vitesse désirée = Vmax dans la direction opposée
        Vec2<float> desired = away.normalized() * settings.vmax;

        return desired - me.getVelocity();
    }

} // namespace bd