
#ifndef PT_BOIDS_RULE_H
#define PT_BOIDS_RULE_H
#include "bd/Settings.h"
#include "bd/Vec2.h"


namespace bd {

    class Flock;

    class Rule {
    public:
        virtual ~Rule() = default;

        /**
         * Calcule la force exercée sur un boid donné.
         * @param flock : Référence vers le flock contenant tous les boids (pour accéder aux voisins)
         * @param boidIndex : L'index du boid actuel (pour ne pas se comparer à soi-même)
         * @param settings : Les paramètres globaux (poids, rayons, etc.)
         * @return Un vecteur force (Vec2)
         */
        virtual Vec2<float> computeForce(const Flock& flock,int boidIndex,const Settings& settings) const = 0;
    };

} // namespace bd

#endif //PT_BOIDS_RULE_H