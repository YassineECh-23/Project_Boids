#ifndef PT_BOIDS_PREDATORRULE_H
#define PT_BOIDS_PREDATORRULE_H


#include "Rule.h"
#include "Settings.h"

namespace bd {

    /**
     * @brief Règle de fuite de prédateur (Predator Avoidance).
     *
     * Cette règle simule la peur des boids face à un prédateur.
     * Si un prédateur est actif et proche, les boids vont chercher à fuir
     * dans la direction opposée le plus vite possible (vitesse max).
     * Cette règle a souvent un poids (weight) très élevé pour être prioritaire sur les autres.
     */
    class PredatorRule : public Rule {
    public:
        /**
         * @brief Calcule la force de fuite face au prédateur.
         *
         * Si le prédateur est activé (settings.enablePredator) et que le boid se trouve
         * dans son rayon de menace (settings.predatorRadius), une force de fuite est générée.
         * La force dirige le boid à l'opposé du prédateur.
         *
         * @param flock      Référence vers le groupe de boids.
         * @param boidIndex  Indice du boid courant.
         * @param settings   Paramètres globaux (position du prédateur, rayon, etc.).
         * @return Vec2<float> Le vecteur force de fuite (steering force).
         */
        Vec2<float> computeForce(const Flock& flock, int boidIndex, const Settings& settings) const override;
    };

} // namespace bd


#endif //PT_BOIDS_PREDATORRULE_H