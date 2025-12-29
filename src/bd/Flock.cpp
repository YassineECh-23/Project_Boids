//
// Flock.cpp - Gestion de la meute de Boids
//

#include "../../include/bd/Flock.h"

// IMPORTANT : On inclut les règles concrètes pour que le code puisse
// faire le "dynamic_cast" et appliquer les bons poids (wcoh, wsep, wali)
#include "../../include/bd/CohesionRule.h"
#include "../../include/bd/SeparationRule.h"
#include "../../include/bd/AlignmentRule.h"

#include <cstdlib> // pour rand()

namespace bd {

    // =========================================================================
    // CONSTRUCTEUR
    // =========================================================================
    Flock::Flock() {
        // DynamicArray s'initialise tout seul à vide
    }

    // =========================================================================
    // INITIALISATION ALÉATOIRE
    // =========================================================================
    void Flock::initRandom(int count, float width, float height) {
        boids_.clear();
        resize(count, width, height);
    }

    // =========================================================================
    // AJOUT MANUEL
    // =========================================================================
    void Flock::addBoid(const Boid& b) {
        boids_.push_back(b);
    }

    // =========================================================================
    // ACCESSEURS (Requis par les Règles et le Main)
    // =========================================================================
    int Flock::size() const {
        return static_cast<int>(boids_.size());
    }

    const Boid& Flock::getBoid(int index) const {
        return boids_[static_cast<size_t>(index)];
    }

    Boid& Flock::getBoid(int index) {
        return boids_[static_cast<size_t>(index)];
    }

    // =========================================================================
    // MISE À JOUR PRINCIPALE (Update All)
    // =========================================================================
    void Flock::updateAll(const DynamicArray<Rule*>& rules,
                          const Settings& settings,
                          float dt) {

        for (int i = 0; i < size(); ++i) {
            Vec2<float> totalForce(0.0f, 0.0f);

            // Appliquer chaque règle
            for (size_t r = 0; r < rules.size(); ++r) {
                Rule* rule = rules[r];

                // Calcul de la force brute de la règle
                Vec2<float> force = rule->computeForce(*this, i, settings);

                // Application du poids selon le type de règle (Polymorphisme)
                if (dynamic_cast<CohesionRule*>(rule)) {
                    force *= settings.wcoh;
                }
                else if (dynamic_cast<SeparationRule*>(rule)) {
                    force *= settings.wsep;
                }
                else if (dynamic_cast<AlignmentRule*>(rule)) {
                    force *= settings.wali;
                }

                totalForce += force;
            }

            // Mise à jour physique du boid
            boids_[static_cast<size_t>(i)].update(totalForce, settings, dt);

            // GESTION DES BORDS (MODIFIÉ)
            // On passe 'settings' complet pour gérer l'option Rebond/Wrap
            boids_[static_cast<size_t>(i)].handleBounds(settings);
        }
    }

    // =========================================================================
    // REDIMENSIONNEMENT
    // =========================================================================
    void Flock::resize(int newCount, float width, float height) {
        int currentSize = size();

        if (newCount > currentSize) {
            // Ajouter des boids
            for (int i = 0; i < (newCount - currentSize); ++i) {
                float posX = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * width;
                float posY = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * height;

                float velX = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f - 1.0f) * 2.0f;
                float velY = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f - 1.0f) * 2.0f;

                addBoid(Boid(Vec2<float>(posX, posY), Vec2<float>(velX, velY)));
            }
        }
        else if (newCount < currentSize) {
            // Retirer des boids en trop
            int toRemove = currentSize - newCount;
            for (int i = 0; i < toRemove; ++i) {
                if (!boids_.empty()) {
                    boids_.removeAt(boids_.size() - 1);
                }
            }
        }
    }

} // namespace bd