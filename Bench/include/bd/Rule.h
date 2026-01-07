#ifndef PT_BOIDS_RULE_H
#define PT_BOIDS_RULE_H
#include "bd/Settings.h"
#include "bd/Vec2.h"


namespace bd {

    class Flock;

    /**
     * @brief Classe abstraite de base pour toutes les règles de comportement.
     *
     * Cette classe définit l'interface commune (pattern Strategy) que toutes les règles
     * (Cohésion, Séparation, Alignement, Obstacles, etc.) doivent implémenter.
     * Elle garantit que chaque règle peut calculer une force vectorielle applicable à un boid.
     */
    class Rule {
    public:
        /**
         * @brief Destructeur virtuel par défaut.
         * Nécessaire pour assurer la destruction correcte des objets dérivés via un pointeur de base.
         */
        virtual ~Rule() = default;

        /**
         * @brief Calcule la force exercée sur un boid donné.
         *
         * Méthode virtuelle pure qui doit être définie par chaque règle concrète.
         *
         * @param flock     Référence vers le flock contenant tous les boids (pour accéder aux voisins).
         * @param boidIndex L'index du boid actuel (pour éviter de se comparer à soi-même).
         * @param settings  Les paramètres globaux (poids, rayons de perception, etc.).
         * @return Vec2<float> Un vecteur force (steering force) à appliquer à l'accélération du boid.
         */
        virtual Vec2<float> computeForce(const Flock& flock,int boidIndex,const Settings& settings) const = 0;
    };

} // namespace bd

#endif //PT_BOIDS_RULE_H