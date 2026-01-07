#ifndef BD_INTERFACE_MENU_SFML_H
#define BD_INTERFACE_MENU_SFML_H

#include <SFML/Graphics.hpp>
#include <string>
#include "DynamicArray.h"
#include "Settings.h"

namespace bd {

    /**
     * Classe gérant l’interface graphique du menu principal avec SFML.
     *  - le menu principal
     *  - l’écran des paramètres
     *  - l’écran de chargement des sauvegardes
     */
    class InterfaceMenuSFML {
    public:
        InterfaceMenuSFML();

        void run();

    private:

        enum class Screen { Main, Settings, Load };

        sf::RenderWindow window;   // Fenêtre principale SFML
        sf::Font font;             // Police utilisée pour les textes
        bool fontLoaded = false;   // Indique si la police a été chargée correctement

        Screen screen = Screen::Main; // Écran actuellement affiché
        Settings settings;            // Paramètres de la simulation

        // --- Boutons du menu principal ---
        sf::RectangleShape btnRun;       // Bouton "Run"
        sf::RectangleShape btnLoad;      // Bouton "Load"
        sf::RectangleShape btnSettings;  // Bouton "Settings"
        sf::RectangleShape btnQuit;      // Bouton "Quit"

        sf::Text txtTitle;      // Titre du menu
        sf::Text txtRun;        // Texte du bouton Run
        sf::Text txtLoad;       // Texte du bouton Load
        sf::Text txtSettings;   // Texte du bouton Settings
        sf::Text txtQuit;       // Texte du bouton Quit

        // --- Boutons de l'écran Settings ---
        sf::RectangleShape btnBack;   // Bouton retour au menu principal
        sf::RectangleShape btnStart;  // Bouton lancer la simulation
        sf::Text txtBack;             // Texte du bouton Back
        sf::Text txtStart;            // Texte du bouton Start

        // --- Gestion de l’édition des paramètres ---
        int selectedIndex = -1;     // Champ actuellement sélectionné
        std::string inputBuffer;    // Buffer de saisie clavier
        sf::Text txtHint;           // Texte d’aide utilisateur
        sf::Text txtError;          // Message d’erreur éventuel

        // --- Écran LOAD ---
       DynamicArray<std::string> saveFiles; // Liste des fichiers de sauvegarde trouvés
        sf::RectangleShape btnRefresh;      // Bouton pour rafraîchir la liste
        sf::Text txtRefresh;                // Texte du bouton Refresh

        /**
         * Initialise tous les éléments graphiques (boutons, textes, tailles).
         */
        void initUI();

        /**
         * Met à jour la disposition des éléments selon la taille de la fenêtre.
         */
        void updateLayout();

        /**
         * Gère les événements SFML (clavier, souris, fermeture).
         */
        void handleEvents();

        /**
         * Met à jour la logique interne (sélection, saisie, navigation).
         */
        void update();

        /**
         * Dessine l’interface à l’écran.
         */
        void render();

        /**
         * Affiche le menu principal.
         */
        void renderMain();

        /**
         * Affiche l’écran des paramètres.
         */
        void renderSettings();

        /**
         * Affiche l’écran de chargement des sauvegardes.
         */
        void renderLoad();

        /**
         * Vérifie si un rectangle SFML a été cliqué par la souris.
         */
        bool isClicked(const sf::RectangleShape& r, sf::Vector2f mouse) const;

        /**
         * Lance la simulation avec ou sans fichier de sauvegarde.
         */
        void launchSimulation(const std::string& saveFile = "");

        // --- Aides pour l’édition des paramètres ---

        /**
         * Sélectionne un champ de paramètre à éditer.
         */
        void selectField(int index);

        /**
         * Valide la saisie utilisateur et l’applique aux paramètres.
         */
        void commitInput();

        /**
         * Supprime le dernier caractère saisi.
         */
        void backspaceInput();

        /**
         * Ajoute un caractère à la saisie utilisateur.
         */
        void appendChar(char c);

        /**
         * Recharge la liste des fichiers de sauvegarde disponibles.
         */
        void refreshSaves();
    };

} // namespace bd

#endif // BD_INTERFACE_MENU_SFML_H
