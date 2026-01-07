#ifndef PT_BOIDS_ALIGNMENTRULE_H
#define PT_BOIDS_ALIGNMENTRULE_H


#include "Rule.h"

namespace bd {

    /**
     * @brief Règle d'alignement (Alignment).
     *
     * Cette classe implémente la règle qui pousse un boid à aligner sa vitesse
     * (direction et intensité) sur celle de ses voisins locaux.
     * Cela permet au groupe de se déplacer de manière coordonnée dans la même direction.
     */
    class AlignmentRule : public Rule {
    public:
        /**
         * @brief Calcule la force d'alignement appliquée à un boid.
         *
         * La force est calculée en faisant la moyenne des vitesses des voisins
         * dans le rayon de perception, puis en soustrayant la vitesse actuelle du boid.
         *
         * @param flock      Référence constante vers l'ensemble du groupe (Flock).
         * @param boidIndex  L'indice du boid pour lequel on calcule la force.
         * @param settings   Les paramètres globaux (rayon de perception, poids, etc.).
         * @return Vec2<float> Le vecteur force d'alignement (steering force).
         */
        Vec2<float> computeForce(const Flock& flock,int boidIndex,const Settings& settings) const override;
    };

} // namespace bd

#endif //PT_BOIDS_ALIGNMENTRULE_H