#include "../../include/bd/CohesionRule.h"
#include "../../include/bd/Flock.h" // Nécessaire pour flock.getBoid()
#include "../../include/bd/Rule.h"

namespace bd {

    /**
     * Calcule la force de cohésion appliquée à un boid.
     * Cette règle attire le boid courant vers le centre de masse
     * de ses voisins situés dans un rayon donné.
     *
     * @param flock      Le groupe de boids
     * @param boidIndex  Indice du boid courant dans le flock
     * @param settings   Paramètres globaux de la simulation (rayon r, etc.)
     * @return           Vecteur représentant la force de cohésion
     */
    Vec2<float> CohesionRule::computeForce(const Flock& flock,int boidIndex,const Settings& settings) const {
        Vec2<float> centerOfMass(0.0f, 0.0f);
        int neighborCount = 0;

        const Boid& me = flock.getBoid(boidIndex);

        for (int i = 0; i < flock.size(); ++i) {
            if (i == boidIndex) continue;

            const Boid& other = flock.getBoid(i);
            float dist = Vec2<float>::distance(me.getPosition(), other.getPosition());

            if (dist > 0 && dist < settings.r) {
                centerOfMass += other.getPosition();
                neighborCount++;
            }
        }

        if (neighborCount > 0) {
            centerOfMass /= static_cast<float>(neighborCount);
            // Force de cohésion : déplacement vers le centre de masse
            return centerOfMass - me.getPosition();
        }
        // Aucun voisin : aucune force appliquée
        return Vec2<float>(0.0f, 0.0f);
    }

} // namespace bd
