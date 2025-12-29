//
// Created by Yassine on 29/12/2025.
//

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

        // Initialisation
        void initRandom(int count, float width, float height);

        // Ajout manuel (utile pour les tests)
        void addBoid(const Boid& b);

        // Boucle principale de simulation
        // Prend un tableau de pointeurs de règles (polymorphisme)
        void updateAll(const DynamicArray<Rule*>& rules, const Settings& settings, float dt);

        // Accesseurs (nécessaires pour que les Règles puissent lire les positions)
        int size() const;
        const Boid& getBoid(int index) const;
        Boid& getBoid(int index); // Version modifiable

        // Gestion du redimensionnement (Interface utilisateur)
        void resize(int newCount, float width, float height);
    };

} // namespace bd

#endif //PT_BOIDS_FLOCK_H