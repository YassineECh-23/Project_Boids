#include "../../include/bd/AlignmentRule.h"
#include "../../include/bd/Flock.h"

namespace bd {

    /**
     * Calcule la force d’alignement appliquée à un boid.
     * Cette règle vise à aligner la vitesse du boid courant
     * avec la vitesse moyenne de ses voisins proches.
     */
    Vec2<float> AlignmentRule::computeForce(
        const Flock& flock,
        int boidIndex,
        const Settings& settings
    ) const {

        // Vecteur représentant la vitesse moyenne des voisins
        Vec2<float> avgVelocity(0.0f, 0.0f);

        // Compteur de voisins pris en compte
        int neighborCount = 0;

        // Référence vers le boid courant
        const Boid& me = flock.getBoid(boidIndex);

        // Parcours de tous les boids du flock
        for (int i = 0; i < flock.size(); ++i) {
            if (i == boidIndex) continue;
            const Boid& other = flock.getBoid(i);
            float dist = Vec2<float>::distance(
                me.getPosition(),
                other.getPosition()
            );
            if (dist > 0 && dist < settings.r) {
                avgVelocity += other.getVelocity();
                neighborCount++;
            }
        }
        if (neighborCount > 0) {
            avgVelocity /= static_cast<float>(neighborCount);
            return avgVelocity - me.getVelocity();
        }
        return Vec2<float>(0.0f, 0.0f);
    }

} // namespace bd
