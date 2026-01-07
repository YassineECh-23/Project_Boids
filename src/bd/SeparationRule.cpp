#include "../../include/bd/SeparationRule.h"
#include "../../include/bd/Flock.h"

namespace bd {

    /**
     * Calcule la force de séparation appliquée à un boid.
     * Cette règle pousse le boid à s’éloigner de ses voisins trop proches
     * afin d’éviter les collisions et les regroupements excessifs.
     *
     * @param flock      Le groupe de boids
     * @param boidIndex  Indice du boid courant
     * @param settings   Paramètres globaux (distance minimale dmin, etc.)
     * @return           Vecteur représentant la force de séparation
     */
    Vec2<float> SeparationRule::computeForce(const Flock& flock,int boidIndex,const Settings& settings) const {

        Vec2<float> force(0.0f, 0.0f);
        int neighborCount = 0;

        const Boid& me = flock.getBoid(boidIndex);

        // Parcours de tous les boids du flock
        for (int i = 0; i < flock.size(); ++i) {

            // On ignore le boid lui-même
            if (i == boidIndex) continue;

            const Boid& other = flock.getBoid(i);

            // Calcul de la distance entre le boid courant et le voisin
            float dist = Vec2<float>::distance(
                me.getPosition(),
                other.getPosition()
            );

            // Si le voisin est trop proche
            if (dist > 0 && dist < settings.dmin) {

                // Vecteur de fuite pondéré par la distance (plus c'est proche, plus c'est fort)
                Vec2<float> diff = me.getPosition() - other.getPosition();
                diff /= (dist * dist);

                force += diff;
                neighborCount++;
            }
        }
        return force;
    }

} // namespace bd
