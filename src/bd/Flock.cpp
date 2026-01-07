#include "../../include/bd/Flock.h"

#include "../../include/bd/CohesionRule.h"
#include "../../include/bd/SeparationRule.h"
#include "../../include/bd/AlignmentRule.h"
#include "../../include/bd/ObstacleRule.h"
#include "../../include/bd/PredatorRule.h"

#include <cstdlib>

namespace bd {

    /**
     * Constructeur par défaut du flock.
     * Initialise un groupe de boids vide.
     */
    Flock::Flock() {}

    /**
     * Initialise le flock avec un nombre donné de boids placés aléatoirement.
     * Supprime les boids existants puis redimensionne le flock.
     *
     * @param count    Nombre de boids à créer
     * @param width    Largeur de la zone de simulation
     * @param height   Hauteur de la zone de simulation
     * @param settings Paramètres globaux de la simulation
     */
    void Flock::initRandom(int count, float width, float height, const Settings& settings) {
        boids_.clear();
        resize(count, width, height, settings);
    }

    /**
     * Ajoute un boid au flock.
     *
     * @param b Boid à ajouter
     */
    void Flock::addBoid(const Boid& b) {
        boids_.push_back(b);
    }

    /**
     * Retourne le nombre de boids dans le flock.
     */
    int Flock::size() const {
        return static_cast<int>(boids_.size());
    }

    /**
     * Accès en lecture seule à un boid du flock.
     *
     * @param index Indice du boid
     */
    const Boid& Flock::getBoid(int index) const {
        return boids_[static_cast<size_t>(index)];
    }

    /**
     * Accès modifiable à un boid du flock.
     *
     * @param index Indice du boid
     */
    Boid& Flock::getBoid(int index) {
        return boids_[static_cast<size_t>(index)];
    }

    /**
     * Met à jour l’ensemble des boids du flock.
     * Pour chaque boid :
     *  - calcule la force totale résultant des différentes règles
     *  - applique les pondérations associées à chaque règle
     *  - met à jour la position et la vitesse du boid
     *  - gère les collisions avec les bords
     *
     * @param rules    Liste des règles de comportement
     * @param settings Paramètres globaux de la simulation
     * @param dt       Delta time (temps écoulé depuis la dernière mise à jour)
     */
    void Flock::updateAll(const DynamicArray<Rule*>& rules,
                          const Settings& settings,
                          float dt) {

        for (int i = 0; i < size(); ++i) {
            Vec2<float> totalForce(0.0f, 0.0f);

            for (size_t r = 0; r < rules.size(); ++r) {
                Rule* rule = rules[r];
                Vec2<float> force = rule->computeForce(*this, i, settings);

                // Application des poids selon le type de règle
                if (dynamic_cast<CohesionRule*>(rule)) {
                    force *= settings.wcoh;
                }
                else if (dynamic_cast<SeparationRule*>(rule)) {
                    force *= settings.wsep;
                }
                else if (dynamic_cast<AlignmentRule*>(rule)) {
                    force *= settings.wali;
                }
                // Extensions Obstacles et Predators
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

    /**
     * Redimensionne le flock.
     * - Si newCount > taille actuelle : ajoute des boids aléatoires
     * - Si newCount < taille actuelle : supprime des boids
     *
     * @param newCount Nouveau nombre de boids souhaité
     * @param width    Largeur de la zone de simulation
     * @param height   Hauteur de la zone de simulation
     * @param settings Paramètres globaux (vmax, etc.)
     */
    void Flock::resize(int newCount, float width, float height, const Settings& settings) {
        int currentSize = size();

        if (newCount > currentSize) {
            // Ajout de boids
            int numberToAdd = newCount - currentSize;
            for (int i = 0; i < numberToAdd; ++i) {
                float posX = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * width;
                float posY = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * height;

                float velX = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f - 1.0f) * settings.vmax;
                float velY = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f - 1.0f) * settings.vmax;

                addBoid(Boid(Vec2<float>(posX, posY), Vec2<float>(velX, velY)));
            }
        }
        else if (newCount < currentSize) {
            // Suppression de boids
            int numberToRemove = currentSize - newCount;
            for (int i = 0; i < numberToRemove; ++i) {
                if (!boids_.empty()) {
                    boids_.removeAt(boids_.size() - 1);
                }
            }
        }
    }

    /**
     * Supprime tous les boids du flock.
     */
    void Flock::clear() {
        boids_.clear();
    }

} // namespace bd
