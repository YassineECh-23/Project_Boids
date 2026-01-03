#include "../../include/bd/Flock.h"

// Inclusion de TOUTES les règles pour pouvoir appliquer les poids spécifiques
#include "../../include/bd/CohesionRule.h"
#include "../../include/bd/SeparationRule.h"
#include "../../include/bd/AlignmentRule.h"
#include "../../include/bd/ObstacleRule.h"
#include "../../include/bd/PredatorRule.h"

#include <cstdlib> // pour rand()

namespace bd {

    Flock::Flock() {}

    void Flock::initRandom(int count, float width, float height, const Settings& settings) {
        boids_.clear();
        resize(count, width, height, settings);
    }

    void Flock::addBoid(const Boid& b) {
        boids_.push_back(b);
    }

    int Flock::size() const {
        return static_cast<int>(boids_.size());
    }

    const Boid& Flock::getBoid(int index) const {
        return boids_[static_cast<size_t>(index)];
    }

    Boid& Flock::getBoid(int index) {
        return boids_[static_cast<size_t>(index)];
    }

    // ============================================================
    // UPDATE ALL : APPLICATION DES RÈGLES ET DES POIDS
    // ============================================================
    void Flock::updateAll(const DynamicArray<Rule*>& rules,
                          const Settings& settings,
                          float dt) {

        for (int i = 0; i < size(); ++i) {
            Vec2<float> totalForce(0.0f, 0.0f);

            for (size_t r = 0; r < rules.size(); ++r) {
                Rule* rule = rules[r];
                Vec2<float> force = rule->computeForce(*this, i, settings);

                // --- APPLICATION DES POIDS (PHASE 2, 4 & 5) ---

                // Règles de base
                if (dynamic_cast<CohesionRule*>(rule)) {
                    force *= settings.wcoh;
                }
                else if (dynamic_cast<SeparationRule*>(rule)) {
                    force *= settings.wsep;
                }
                else if (dynamic_cast<AlignmentRule*>(rule)) {
                    force *= settings.wali;
                }
                // Règles Phase 5 (Extensions)
                else if (dynamic_cast<ObstacleRule*>(rule)) {
                    force *= settings.wobstacle;
                }
                else if (dynamic_cast<PredatorRule*>(rule)) {
                    force *= settings.wpredator;
                }

                totalForce += force;
            }

            boids_[static_cast<size_t>(i)].update(totalForce, settings, dt);
            boids_[static_cast<size_t>(i)].handleBounds(settings);
        }
    }

    // ============================================================
    // RESIZE : AJOUT/SUPPRESSION DYNAMIQUE (PHASE 4)
    // ============================================================
    void Flock::resize(int newCount, float width, float height, const Settings& settings) {
        int currentSize = size();

        if (newCount > currentSize) {
            // AJOUT
            int numberToAdd = newCount - currentSize;
            for (int i = 0; i < numberToAdd; ++i) {
                float posX = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * width;
                float posY = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * height;

                // Vitesse aléatoire bornée par vmax
                float velX = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f - 1.0f) * settings.vmax;
                float velY = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f - 1.0f) * settings.vmax;

                addBoid(Boid(Vec2<float>(posX, posY), Vec2<float>(velX, velY)));
            }
        }
        else if (newCount < currentSize) {
            // SUPPRESSION
            int numberToRemove = currentSize - newCount;
            for (int i = 0; i < numberToRemove; ++i) {
                if (!boids_.empty()) {
                    // Suppression du dernier élément
                    boids_.removeAt(boids_.size() - 1);
                }
            }
        }
    }

} // namespace bd