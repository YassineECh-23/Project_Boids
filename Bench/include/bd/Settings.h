#ifndef BD_SETTINGS_H
#define BD_SETTINGS_H

namespace bd {

    /**
     * @brief Structure contenant tous les paramètres de configuration de la simulation.
     *
     * Cette structure regroupe les constantes et variables qui influencent le comportement
     * des boids, les limites de l'environnement, les poids des règles et l'état des
     * extensions (obstacles, prédateurs). Elle est partagée entre le moteur de simulation
     * et l'interface graphique.
     */
    struct Settings {
        // ============================================================
        // PARAMÈTRES PAR DÉFAUT
        // ============================================================

        /** @brief Nombre d'agents (boids) actifs dans la simulation. */
        int nbBoids = 50;
        /** @brief Vitesse maximale de déplacement d'un boid (en pixels/frame). */
        float vmax = 4.0f;
        /** @brief Force maximale applicable pour changer de direction (maniabilité). */
        float maxAccel = 0.1f;
        /** @brief Rayon de perception locale : distance pour considérer un autre boid comme voisin. */
        float r = 50.0f;
        /** @brief Distance minimale de confort : en dessous, la règle de séparation s'active. */
        float dmin = 20.0f;

        // Poids des règles
        /** @brief Poids de la règle de Cohésion (attraction vers le centre du groupe). */
        float wcoh = 0.01f;
        /** @brief Poids de la règle de Séparation (répulsion des voisins trop proches). */
        float wsep = 0.05f;
        /** @brief Poids de la règle d'Alignement (mimiétisme de la vitesse des voisins). */
        float wali = 0.125f;

        // ============================================================
        // BORNES POUR L'INTERFACE (modifiables en temps réel)
        // ============================================================

        /** @brief Valeur minimale autorisée pour le nombre de boids via l'interface. */
        int nbBoidsMin = 10;
        /** @brief Valeur maximale autorisée pour le nombre de boids via l'interface. */
        int nbBoidsMax = 200;

        /** @brief Valeur minimale pour le rayon de perception. */
        float rMin = 10.0f;
        /** @brief Valeur maximale pour le rayon de perception. */
        float rMax = 100.0f;

        /** @brief Valeur minimale pour la distance de séparation. */
        float dminMin = 5.0f;
        /** @brief Valeur maximale pour la distance de séparation. */
        float dminMax = 50.0f;

        /** @brief Poids minimum pour les règles (0 = règle désactivée). */
        float weightMin = 0.0f;
        /** @brief Poids maximum pour les règles. */
        float weightMax = 1.0f;

        // ============================================================
        // PARAMÈTRES DE LA FENÊTRE
        // ============================================================

        /** @brief Largeur de la fenêtre de simulation (en pixels). */
        float windowWidth = 800.0f;
        /** @brief Hauteur de la fenêtre de simulation (en pixels). */
        float windowHeight = 600.0f;

        // Limites de sécurité pour la résolution
        /** @brief Largeur minimale acceptée pour la fenêtre. */
        float widthMin = 800.0f;
        /** @brief Largeur maximale acceptée pour la fenêtre. */
        float widthMax = 2560.0f;
        /** @brief Hauteur minimale acceptée pour la fenêtre. */
        float heightMin = 600.0f;
        /** @brief Hauteur maximale acceptée pour la fenêtre. */
        float heightMax = 1440.0f;

        /**
         * @brief Mode de gestion des bords.
         * - false : Wrap (téléportation de l'autre côté).
         * - true : Bounce (rebond contre les murs).
         */
        bool enableBounce = false;

        // ============================================================
        // EXTENSIONS : OBSTACLE ET PRÉDATEUR
        // ============================================================

        /** @brief Active ou désactive la présence d'un obstacle statique (Clic Droit). */
        bool enableObstacle = false;
        /** @brief Poids de la force d'évitement d'obstacle (prioritaire si élevé). */
        float wobstacle = 5.0f;

        /** @brief Position X de l'obstacle. */
        float obstacleX = 0.0f;
        /** @brief Position Y de l'obstacle. */
        float obstacleY = 0.0f;
        /** @brief Rayon d'action de l'obstacle (zone à éviter). */
        float obstacleRadius = 150.0f;


        /** @brief Active ou désactive le prédateur mobile (Touche P). */
        bool enablePredator = false;
        /** @brief Poids de la force de fuite face au prédateur (Très élevé pour panique). */
        float wpredator = 8.0f;

        /** @brief Position X actuelle du prédateur. */
        float predatorX = 0.0f;
        /** @brief Position Y actuelle du prédateur. */
        float predatorY = 0.0f;
        /** @brief Rayon de menace du prédateur (distance à laquelle les boids paniquent). */
        float predatorRadius = 150.0f;
        /** @brief Vitesse de déplacement du prédateur. */
        float predatorSpeed = 2.5f;
    };

} // namespace bd

#endif // BD_SETTINGS_H