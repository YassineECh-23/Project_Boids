//
// Flock.cpp - Gestion du troupeau (Phase 4 : Resize & Calibration)
//

#include "../../include/bd/Flock.h"

// Inclusion des règles pour le dynamic_cast (application des poids)
#include "../../include/bd/CohesionRule.h"
#include "../../include/bd/SeparationRule.h"
#include "../../include/bd/AlignmentRule.h"

#include <cstdlib> // pour rand()

namespace bd {

    // =========================================================================
    // CONSTRUCTEUR
    // =========================================================================
    Flock::Flock() {
        // DynamicArray s'initialise automatiquement
    }

    // =========================================================================
    // INITIALISATION ALÉATOIRE
    // =========================================================================
    void Flock::initRandom(int count, float width, float height, const Settings& settings) {
        boids_.clear();
        resize(count, width, height, settings);
    }

    // =========================================================================
    // AJOUT MANUEL
    // =========================================================================
    void Flock::addBoid(const Boid& b) {
        boids_.push_back(b);
    }

    // =========================================================================
    // ACCESSEURS
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
    // Update All
    // =========================================================================
    void Flock::updateAll(const DynamicArray<Rule*>& rules,
                          const Settings& settings,
                          float dt) {

        for (int i = 0; i < size(); ++i) {
            Vec2<float> totalForce(0.0f, 0.0f);

            // 1. Calcul des forces selon les règles
            for (size_t r = 0; r < rules.size(); ++r) {
                Rule* rule = rules[r];

                // Calcul de la force brute (direction)
                Vec2<float> force = rule->computeForce(*this, i, settings);

                // 2. Application des POIDS (Calibration Phase 4)
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

            // 3. Mise à jour physique du boid
            boids_[static_cast<size_t>(i)].update(totalForce, settings, dt);

            // 4. Gestion des bords (Rebonds ou Traversée)
            boids_[static_cast<size_t>(i)].handleBounds(settings);
        }
    }

    // =========================================================================
    // resize
    // =========================================================================
    void Flock::resize(int newCount, float width, float height, const Settings& settings) {
        int currentSize = size();

        // CAS 1 : AJOUT DE BOIDS
        if (newCount > currentSize) {
            int numberToAdd = newCount - currentSize;

            for (int i = 0; i < numberToAdd; ++i) {
                // Position aléatoire
                float posX = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * width;
                float posY = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * height;

                // Vitesse aléatoire (Bornée par settings.vmax)
                // Formule : (rand(0..1) * 2 - 1) donne [-1, 1], puis * vmax
                float velX = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f - 1.0f) * settings.vmax;
                float velY = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f - 1.0f) * settings.vmax;

                addBoid(Boid(Vec2<float>(posX, posY), Vec2<float>(velX, velY)));
            }
        }
        // CAS 2 : SUPPRESSION DE BOIDS
        else if (newCount < currentSize) {
            int numberToRemove = currentSize - newCount;

            for (int i = 0; i < numberToRemove; ++i) {
                if (!boids_.empty()) {
                    // Utilisation de removeAt car pop_back n'existe pas dans ton DynamicArray
                    // On supprime le dernier élément
                    boids_.removeAt(boids_.size() - 1);
                }
            }
        }
    }

} // namespace bd