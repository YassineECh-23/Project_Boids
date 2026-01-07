#ifndef PT_BOIDS_FLOCK_H
#define PT_BOIDS_FLOCK_H

#include "DynamicArray.h"
#include "Boids.h"
#include "Rule.h"
#include "Settings.h"

namespace bd {

    /**
     * @brief Classe gérant un ensemble de boids (groupe/nuée).
     *
     * Le Flock est le conteneur principal des agents. Il est responsable de :
     * - Stocker tous les boids.
     * - Les initialiser et les gérer (ajout, suppression).
     * - Appliquer les règles de comportement à l'ensemble du groupe.
     */
    class Flock {
    private:
        DynamicArray<Boid> boids_;

    public:
        /**
         * @brief Constructeur par défaut.
         * Crée un Flock vide.
         */
        Flock();

        /**
         * @brief Destructeur par défaut.
         */
        ~Flock() = default;

        /**
         * @brief Initialise le flock avec un nombre donné de boids aléatoires.
         *
         * Vide le flock existant et crée 'count' nouveaux boids avec des positions
         * et des vitesses aléatoires comprises dans les limites données.
         *
         * @param count    Nombre de boids à créer.
         * @param width    Largeur de la zone d'apparition.
         * @param height   Hauteur de la zone d'apparition.
         * @param settings Paramètres globaux (utilisés pour la vitesse max, etc.).
         */
        void initRandom(int count, float width, float height, const Settings& settings);

        /**
         * @brief Ajoute manuellement un boid au groupe.
         * @param b Le boid à ajouter.
         */
        void addBoid(const Boid& b);

        /**
         * @brief Met à jour l'ensemble des boids du flock.
         *
         * Cette méthode est le cœur de la simulation à chaque frame.
         * Pour chaque boid, elle :
         * 1. Calcule la somme des forces de toutes les règles actives.
         * 2. Applique les pondérations définies dans Settings.
         * 3. Met à jour la physique (position, vitesse) du boid.
         * 4. Gère les collisions avec les bords.
         *
         * @param rules    La liste des règles (Rule*) à appliquer.
         * @param settings Les paramètres globaux de la simulation.
         * @param dt       Le pas de temps (delta time).
         */
        void updateAll(const DynamicArray<Rule*>& rules, const Settings& settings, float dt);

        /**
         * @brief Retourne le nombre de boids dans le groupe.
         * @return int Le nombre de boids.
         */
        int size() const;

        /**
         * @brief Accès en lecture seule à un boid spécifique.
         * @param index L'indice du boid dans le tableau.
         * @return const Boid& Référence constante vers le boid.
         */
        const Boid& getBoid(int index) const;

        /**
         * @brief Accès modifiable à un boid spécifique.
         * @param index L'indice du boid dans le tableau.
         * @return Boid& Référence modifiable vers le boid.
         */
        Boid& getBoid(int index);

        /**
         * @brief Redimensionne le flock pour atteindre un nombre cible de boids.
         *
         * - Si newCount > taille actuelle : Ajoute des boids aléatoires.
         * - Si newCount < taille actuelle : Supprime les derniers boids.
         *
         * @param newCount Le nouveau nombre de boids désiré.
         * @param width    Largeur pour l'apparition des nouveaux boids.
         * @param height   Hauteur pour l'apparition des nouveaux boids.
         * @param settings Paramètres pour l'initialisation des nouveaux boids.
         */
        void resize(int newCount, float width, float height, const Settings& settings);

        /**
         * @brief Vide le flock. Supprime tous les boids.
         */
        void clear();
    };

} // namespace bd

#endif //PT_BOIDS_FLOCK_H