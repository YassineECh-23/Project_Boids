#ifndef BD_INTERFACE_MENU_SFML_H
#define BD_INTERFACE_MENU_SFML_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Settings.h"

namespace bd {

    class InterfaceMenuSFML {
    public:
        InterfaceMenuSFML();
        void run();

    private:
        // AJOUT : Nouvel état "Load"
        enum class Screen { Main, Settings, Load };

        sf::RenderWindow window;
        sf::Font font;
        bool fontLoaded = false;

        Screen screen = Screen::Main;
        Settings settings;

        // --- Main buttons ---
        sf::RectangleShape btnRun;
        sf::RectangleShape btnLoad;
        sf::RectangleShape btnSettings;
        sf::RectangleShape btnQuit;

        sf::Text txtTitle;
        sf::Text txtRun;
        sf::Text txtLoad;
        sf::Text txtSettings;
        sf::Text txtQuit;

        // --- Settings buttons ---
        sf::RectangleShape btnBack;
        sf::RectangleShape btnStart;
        sf::Text txtBack;
        sf::Text txtStart;

        // --- Settings editing ---
        int selectedIndex = -1;
        std::string inputBuffer;
        sf::Text txtHint;
        sf::Text txtError;

        // --- LOAD SCREEN (NOUVEAU) ---
        std::vector<std::string> saveFiles; // Liste des fichiers trouvés
        sf::RectangleShape btnRefresh;      // Bouton pour recharger la liste
        sf::Text txtRefresh;

        void initUI();
        void updateLayout();

        void handleEvents();
        void update();
        void render();

        void renderMain();
        void renderSettings();
        void renderLoad(); // <--- Nouvelle méthode d'affichage

        bool isClicked(const sf::RectangleShape& r, sf::Vector2f mouse) const;

        void launchSimulation(const std::string& saveFile = ""); // Paramètre optionnel

        // settings helpers
        void selectField(int index);
        void commitInput();
        void backspaceInput();
        void appendChar(char c);

        // Helper pour rafraîchir la liste des sauvegardes
        void refreshSaves();
    };

} // namespace bd

#endif