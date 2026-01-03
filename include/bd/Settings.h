//
// Settings.h - Paramètres globaux de la simulation
// Valeurs par défaut du cahier des charges (page 2)
//

#ifndef BD_SETTINGS_H
#define BD_SETTINGS_H

namespace bd {

    struct Settings {
        // ============================================================
        // PARAMÈTRES PAR DÉFAUT DU SUJET
        // ============================================================

        int nbBoids = 50;              // Nombre de boids
        float vmax = 4.0f;             // Vitesse maximale (unités/pas)
        float maxAccel = 0.1f;         // Accélération max (unités/pas²)
        float r = 50.0f;               // Rayon de perception (pixels)
        float dmin = 20.0f;            // Distance min séparation (pixels)

        // Poids des règles
        float wcoh = 0.01f;            // Cohésion
        float wsep = 0.05f;            // Séparation
        float wali = 0.125f;           // Alignement

        // ============================================================
        // BORNES POUR L'INTERFACE (modifiables en temps réel)
        // ============================================================

        int nbBoidsMin = 10;
        int nbBoidsMax = 200;

        float rMin = 10.0f;
        float rMax = 100.0f;

        float dminMin = 5.0f;
        float dminMax = 50.0f;

        float weightMin = 0.0f;
        float weightMax = 1.0f;

        // ============================================================
        // PARAMÈTRES DE LA FENÊTRE
        // ============================================================

        float windowWidth = 800.0f;
        float windowHeight = 600.0f;

        //  rebond
        bool enableBounce = false;
        // ============================================================
        // EXTENSIONS : OBSTACLE (PHASE 5)
        // ============================================================
        bool enableObstacle = false;   // Activé via Clic Droit
        float wobstacle = 5.0f;        // Poids très fort pour que la fuite soit prioritaire

        float obstacleX = 0.0f;
        float obstacleY = 0.0f;
        float obstacleRadius = 150.0f; // Rayon de la zone de danger (assez grand)

        // --- EXTENSION : PRÉDATEUR (PHASE 5) ---
        bool enablePredator = false;    // Activé via la touche P
        float wpredator = 8.0f;         // Poids TRÈS fort (Panique totale)

        float predatorX = 0.0f;
        float predatorY = 0.0f;
        float predatorRadius = 150.0f;  // Rayon de peur
        float predatorSpeed = 3.5f;     // Vitesse du prédateur (un peu moins vite que vmax=4.0)
    };

} // namespace bd

#endif // BD_SETTINGS_HIDS_SETTINGS_H