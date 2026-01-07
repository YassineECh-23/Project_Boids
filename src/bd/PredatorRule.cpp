#include "../../include/bd/PredatorRule.h"
#include "../../include/bd/Flock.h"

namespace bd {

    /**
     * Calcule la force de fuite appliquée à un boid face à un prédateur.
     * - Si le prédateur est désactivé : aucune force
     * - Si le boid est hors du rayon de danger : aucune force
     * - Sinon : application d’une force de steering pour fuir le prédateur
     *
     * @param flock      Le groupe de boids
     * @param boidIndex  Indice du boid courant
     * @param settings   Paramètres globaux (activation, position, rayon, vmax, etc.)
     * @return           Vecteur force de fuite (steering) pour ce boid
     */
    Vec2<float> PredatorRule::computeForce(const Flock& flock,int boidIndex,const Settings& settings) const {
        // Si le prédateur n'est pas activé, aucune force appliquée
        if (!settings.enablePredator) {
            return Vec2<float>(0.0f, 0.0f);
        }

        const Boid& me = flock.getBoid(boidIndex);
        Vec2<float> predatorPos(settings.predatorX, settings.predatorY);

        // Vecteur allant du prédateur vers le boid (direction de fuite)
        Vec2<float> away = me.getPosition() - predatorPos;
        float dist = away.length();

        // Si le prédateur est trop loin (ou distance quasi nulle), aucune force
        if (dist > settings.predatorRadius || dist <= 0.0001f) {
            return Vec2<float>(0.0f, 0.0f);
        }

        // Calcul de la vitesse désirée à Vmax dans la direction opposée au prédateur
        Vec2<float> desired = away.normalized() * settings.vmax;

        // Force de steering = vitesse désirée - vitesse actuelle
        return desired - me.getVelocity();
    }

} // namespace bd
