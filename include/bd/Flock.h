#ifndef PT_BOIDS_FLOCK_H
#define PT_BOIDS_FLOCK_H

#include "DynamicArray.h"
#include "Boids.h" // J'ai gardé "Boids.h" car c'est le nom dans ton arborescence
#include "Rule.h"
#include "Settings.h"

namespace bd {

    class Flock {
    private:
        DynamicArray<Boid> boids_;

    public:
        Flock();
        ~Flock() = default;

        // MODIFICATION : Ajout de 'const Settings& settings' pour récupérer vmax
        void initRandom(int count, float width, float height, const Settings& settings);

        // Ajout manuel
        void addBoid(const Boid& b);

        // Boucle principale de simulation
        void updateAll(const DynamicArray<Rule*>& rules, const Settings& settings, float dt);

        // Accesseurs
        int size() const;
        const Boid& getBoid(int index) const;
        Boid& getBoid(int index);

        // MODIFICATION : Ajout de 'const Settings& settings' pour gérer l'ajout de boids (vmax)
        void resize(int newCount, float width, float height, const Settings& settings);
    };

} // namespace bd

#endif //PT_BOIDS_FLOCK_H