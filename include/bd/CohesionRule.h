#ifndef PT_BOIDS_COHESIONRULE_H
#define PT_BOIDS_COHESIONRULE_H

#include "Rule.h"

namespace bd {

    /**
     * @brief Règle de cohésion (Cohesion).
     *
     * Cette règle pousse un boid à se diriger vers la position moyenne (centre de masse)
     * de ses voisins locaux. Cela permet au groupe de rester ensemble et d'éviter
     * que des individus ne s'isolent.
     */
    class CohesionRule : public Rule {
    public:
        /**
         * @brief Calcule la force de cohésion appliquée à un boid.
         *
         * La force est calculée en déterminant le centre de masse des voisins
         * dans le rayon de perception, puis en créant un vecteur directeur
         * depuis la position du boid vers ce centre.
         *
         * @param flock      Référence constante vers l'ensemble du groupe (Flock).
         * @param boidIndex  L'indice du boid pour lequel on calcule la force.
         * @param settings   Les paramètres globaux (rayon de perception, poids, etc.).
         * @return Vec2<float> Le vecteur force de cohésion (steering force).
         */
        Vec2<float> computeForce(const Flock& flock, int boidIndex, const Settings& settings) const override;
    };

} // namespace bd


#endif //PT_BOIDS_COHESIONRULE_H