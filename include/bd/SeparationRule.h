#ifndef PT_BOIDS_SEPARATIONRULE_H
#define PT_BOIDS_SEPARATIONRULE_H

#include "Rule.h"

namespace bd {

    /**
     * @brief Règle de séparation (Separation).
     *
     * Cette règle permet d'éviter les collisions et le surpeuplement local.
     * Elle génère une force répulsive si un boid se trouve trop près de ses voisins.
     * C'est ce qui maintient une distance minimale de confort entre les individus.
     */
    class SeparationRule : public Rule {
    public:
        /**
         * @brief Calcule la force de séparation pour un boid donné.
         *
         * Si des voisins sont détectés à une distance inférieure à la distance de sécurité
         * (définie par settings.dmin), une force est calculée pour s'éloigner d'eux.
         * La force est inversement proportionnelle à la distance (plus c'est proche, plus ça pousse fort).
         *
         * @param flock      Référence vers le groupe de boids.
         * @param boidIndex  Indice du boid courant.
         * @param settings   Paramètres globaux (distance min de séparation 'dmin', etc.).
         * @return Vec2<float> Le vecteur force de répulsion (steering force).
         */
        Vec2<float> computeForce(const Flock& flock, int boidIndex, const Settings& settings) const override;
    };

} // namespace bd

#endif //PT_BOIDS_SEPARATIONRULE_H