#ifndef BD_INTERFACE_MENU_SFML_H
#define BD_INTERFACE_MENU_SFML_H

#include <SFML/Graphics.hpp>
#include <string>
#include "Settings.h"

namespace bd {

    class InterfaceMenuSFML {
    public:
        InterfaceMenuSFML();
        void run();

    private:
        enum class Screen { Main, Settings };

        sf::RenderWindow window;
        sf::Font font;
        bool fontLoaded = false;

        Screen screen = Screen::Main;
        Settings settings;

        // --- Main buttons ---
        sf::RectangleShape btnRun;
        sf::RectangleShape btnSettings;
        sf::RectangleShape btnQuit;

        sf::Text txtTitle;
        sf::Text txtRun;
        sf::Text txtSettings;
        sf::Text txtQuit;

        // --- Settings buttons ---
        sf::RectangleShape btnBack;
        sf::RectangleShape btnStart;
        sf::Text txtBack;
        sf::Text txtStart;

        // --- Settings editing ---
        int selectedIndex = -1;          // -1 = rien sélectionné
        std::string inputBuffer;         // ce que l'utilisateur tape
        sf::Text txtHint;

        void initUI();
        void handleEvents();
        void update();
        void render();

        void renderMain();
        void renderSettings();

        bool isClicked(const sf::RectangleShape& r, sf::Vector2f mouse) const;

        // actions
        void launchApp();

        // settings helpers
        void selectField(int index);
        void commitInput();
        void backspaceInput();
        void appendChar(char c);
    };

} // namespace bd

#endif
