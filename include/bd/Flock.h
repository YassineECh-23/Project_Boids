#ifndef PT_BOIDS_FLOCK_H
#define PT_BOIDS_FLOCK_H

#include "DynamicArray.h"
#include "Boids.h"
#include "Rule.h"
#include "Settings.h"

namespace bd {

    class Flock {
    private:
        DynamicArray<Boid> boids_;

    public:
        Flock();
        ~Flock() = default;

        // Initialisation aléatoire
        void initRandom(int count, float width, float height, const Settings& settings);

        // Ajout manuel
        void addBoid(const Boid& b);

        // Boucle principale de simulation
        void updateAll(const DynamicArray<Rule*>& rules, const Settings& settings, float dt);

        // Accesseurs
        int size() const;
        const Boid& getBoid(int index) const;
        Boid& getBoid(int index);

        // Redimensionnement et nettoyage
        void resize(int newCount, float width, float height, const Settings& settings);
        void clear();
    };

} // namespace bd

#endif //PT_BOIDS_FLOCK_H