#include "../../include/bd/InterfaceMenuSFML.h"
#include "../../include/bd/Simulation.h"

#include <iostream>
#include <sstream>
#include <cctype> // isdigit

namespace bd {

InterfaceMenuSFML::InterfaceMenuSFML()
    : window(sf::VideoMode(900, 600), "Boids - Menu") {
    window.setFramerateLimit(60);
    initUI();
}

void InterfaceMenuSFML::initUI() {

    fontLoaded = font.loadFromFile("../assets/Roboto-Regular.ttf");
    if (!fontLoaded) {
        std::cerr << "Failed to load font ../assets/Roboto-Regular.ttf\n";
    }

    // Title
    txtTitle.setString("BOIDS");
    txtTitle.setCharacterSize(56);
    txtTitle.setPosition(360.f, 60.f);
    if (fontLoaded) txtTitle.setFont(font);

    // Main buttons
    auto setupBtn = [&](sf::RectangleShape& b, float x, float y) {
        b.setSize({260.f, 70.f});
        b.setPosition(x, y);
        b.setFillColor(sf::Color(40, 40, 40));
        b.setOutlineThickness(2.f);
        b.setOutlineColor(sf::Color(180, 180, 180));
    };

    setupBtn(btnRun, 320.f, 210.f);
    setupBtn(btnSettings, 320.f, 305.f);
    setupBtn(btnQuit, 320.f, 400.f);

    auto setupText = [&](sf::Text& t, const std::string& s, float x, float y, unsigned size) {
        t.setString(s);
        t.setCharacterSize(size);
        t.setPosition(x, y);
        if (fontLoaded) t.setFont(font);
    };

    setupText(txtRun, "RUN", 430.f, 228.f, 26);
    setupText(txtSettings, "SETTINGS", 392.f, 323.f, 26);
    setupText(txtQuit, "QUIT", 425.f, 418.f, 26);

    // Settings buttons
    btnBack.setSize({180.f, 55.f});
    btnBack.setPosition(40.f, 520.f);
    btnBack.setFillColor(sf::Color(40, 40, 40));
    btnBack.setOutlineThickness(2.f);
    btnBack.setOutlineColor(sf::Color(180, 180, 180));

    btnStart.setSize({180.f, 55.f});
    btnStart.setPosition(680.f, 520.f);
    btnStart.setFillColor(sf::Color(40, 40, 40));
    btnStart.setOutlineThickness(2.f);
    btnStart.setOutlineColor(sf::Color(180, 180, 180));

    setupText(txtBack, "BACK", 95.f, 532.f, 20);
    setupText(txtStart, "RUN", 750.f, 532.f, 20);

    // Hint
    txtHint.setString("Clique un champ puis tape un nombre. Enter = valider. ESC = retour.");
    txtHint.setCharacterSize(18);
    txtHint.setPosition(140.f, 110.f);
    if (fontLoaded) txtHint.setFont(font);
}

bool InterfaceMenuSFML::isClicked(const sf::RectangleShape& r, sf::Vector2f mouse) const {
    return r.getGlobalBounds().contains(mouse);
}

void InterfaceMenuSFML::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

void InterfaceMenuSFML::handleEvents() {
    sf::Event e{};
    while (window.pollEvent(e)) {
        if (e.type == sf::Event::Closed) window.close();

        // --- Keyboard for settings screen ---
        if (screen == Screen::Settings) {
            if (e.type == sf::Event::KeyPressed) {
                if (e.key.code == sf::Keyboard::Escape) {
                    screen = Screen::Main;
                    selectedIndex = -1;
                    inputBuffer.clear();
                }
                if (e.key.code == sf::Keyboard::Enter) {
                    commitInput();
                }
                if (e.key.code == sf::Keyboard::BackSpace) {
                    backspaceInput();
                }
            }

            if (e.type == sf::Event::TextEntered) {
                // Autoriser chiffres, '.' et '-'
                char c = static_cast<char>(e.text.unicode);
                if (c >= 32 && c <= 126) {
                    if (std::isdigit((unsigned char)c) || c == '.' || c == '-') {
                        appendChar(c);
                    }
                }
            }
        }

        // --- Mouse clicks ---
        if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mouse = window.mapPixelToCoords({e.mouseButton.x, e.mouseButton.y});

            if (screen == Screen::Main) {
                if (isClicked(btnRun, mouse)) {
                    if (screen == Screen::Settings && selectedIndex != -1) {
                        commitInput();          //
                        selectedIndex = -1;
                        inputBuffer.clear();
                    }
                    launchSimulation();

                } else if (isClicked(btnSettings, mouse)) {
                    screen = Screen::Settings;
                    selectedIndex = -1;
                    inputBuffer.clear();
                } else if (isClicked(btnQuit, mouse)) {
                    window.close();
                }
            } else { // Settings screen
                if (isClicked(btnBack, mouse)) {
                    screen = Screen::Main;
                    selectedIndex = -1;
                    inputBuffer.clear();
                } else if (isClicked(btnStart, mouse)) {
                    if (screen == Screen::Settings && selectedIndex != -1) {
                        commitInput();          //
                        selectedIndex = -1;
                        inputBuffer.clear();
                    }
                    launchSimulation();

                } else {
                    // Selection d'un champ (zone cliquable sur la colonne valeur)
                    float y0 = 150.f;
                    for (int i = 0; i < 6; ++i) {
                        float y = y0 + i * 55.f;
                        sf::FloatRect valueRect(380.f, y, 220.f, 40.f);
                        if (valueRect.contains(mouse)) {
                            selectField(i);
                        }
                    }
                }
            }
        }
    }
}

void InterfaceMenuSFML::appendChar(char c) {
    if (selectedIndex == -1) return; // rien sélectionné
    if (inputBuffer.size() < 12) inputBuffer.push_back(c);
}

void InterfaceMenuSFML::backspaceInput() {
    if (selectedIndex == -1) return;
    if (!inputBuffer.empty()) inputBuffer.pop_back();
}

void InterfaceMenuSFML::selectField(int index) {
    selectedIndex = index;
    inputBuffer.clear();

    // Pré-remplir avec la valeur actuelle (optionnel, mais pratique)
    std::ostringstream oss;
    switch (index) {
        case 0: oss << settings.nbBoids; break;
        case 1: oss << settings.r; break;
        case 2: oss << settings.dmin; break;
        case 3: oss << settings.wcoh; break;
        case 4: oss << settings.wsep; break;
        case 5: oss << settings.wali; break;
    }
    inputBuffer = oss.str();
}

void InterfaceMenuSFML::commitInput() {
    if (selectedIndex == -1) return;
    if (inputBuffer.empty() || inputBuffer == "-" || inputBuffer == ".") return;

    try {
        if (selectedIndex == 0) {
            int v = std::stoi(inputBuffer);
            if (v < 1) v = 1;
            settings.nbBoids = v;
        } else {
            float v = std::stof(inputBuffer);
            if (selectedIndex == 1) settings.r = v;
            if (selectedIndex == 2) settings.dmin = v;
            if (selectedIndex == 3) settings.wcoh = v;
            if (selectedIndex == 4) settings.wsep = v;
            if (selectedIndex == 5) settings.wali = v;
        }
    } catch (...) {
        // si l'utilisateur tape n'importe quoi, on ignore
    }
}

void InterfaceMenuSFML::update() {
    // Hover effect simple
    sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    auto paint = [&](sf::RectangleShape& b) {
        bool hover = b.getGlobalBounds().contains(mouse);
        b.setFillColor(hover ? sf::Color(70, 70, 70) : sf::Color(40, 40, 40));
    };

    if (screen == Screen::Main) {
        paint(btnRun);
        paint(btnSettings);
        paint(btnQuit);
    } else {
        paint(btnBack);
        paint(btnStart);
    }
}

void InterfaceMenuSFML::render() {
    window.clear(sf::Color(15, 15, 20));
    if (screen == Screen::Main) renderMain();
    else renderSettings();
    window.display();
}

void InterfaceMenuSFML::renderMain() {
    window.draw(txtTitle);
    window.draw(btnRun);
    window.draw(btnSettings);
    window.draw(btnQuit);
    window.draw(txtRun);
    window.draw(txtSettings);
    window.draw(txtQuit);
}

void InterfaceMenuSFML::renderSettings() {
    window.draw(txtHint);

    // Titre
    sf::Text title;
    title.setString("SETTINGS");
    title.setCharacterSize(34);
    title.setPosition(360.f, 55.f);
    if (fontLoaded) title.setFont(font);
    window.draw(title);

    // lignes
    const char* names[6] = {"nbBoids", "r", "dmin", "wcoh", "wsep", "wali"};
    float y0 = 150.f;

    for (int i = 0; i < 6; ++i) {
        float y = y0 + i * 55.f;

        sf::Text label;
        label.setString(names[i]);
        label.setCharacterSize(22);
        label.setPosition(160.f, y + 5.f);
        if (fontLoaded) label.setFont(font);
        window.draw(label);

        // zone valeur (cliquable)
        sf::RectangleShape box({220.f, 40.f});
        box.setPosition(380.f, y);
        box.setFillColor(sf::Color(30, 30, 35));
        box.setOutlineThickness(2.f);
        box.setOutlineColor(i == selectedIndex ? sf::Color(255, 200, 80) : sf::Color(120, 120, 120));
        window.draw(box);

        std::ostringstream oss;
        if (i == selectedIndex) {
            oss << inputBuffer;
        } else {
            switch (i) {
                case 0: oss << settings.nbBoids; break;
                case 1: oss << settings.r; break;
                case 2: oss << settings.dmin; break;
                case 3: oss << settings.wcoh; break;
                case 4: oss << settings.wsep; break;
                case 5: oss << settings.wali; break;
            }
        }

        sf::Text val;
        val.setString(oss.str());
        val.setCharacterSize(22);
        val.setPosition(395.f, y + 5.f);
        if (fontLoaded) val.setFont(font);
        window.draw(val);
    }

    window.draw(btnBack);
    window.draw(btnStart);
    window.draw(txtBack);
    window.draw(txtStart);
}

/*void InterfaceMenuSFML::launchApp() {
    // On cache la fenêtre menu pendant l'App
    window.setVisible(false);

    bd::App app;
    app.init();

    while (app.window.isOpen()) {
        app.handleEvents();
        app.update(1.0f);
        app.render();
    }

    window.setVisible(true);
}*/
    void InterfaceMenuSFML::launchSimulation() {
    window.setVisible(false);

    bd::Simulation sim(settings); //
    sim.run();

    window.setVisible(true);
}

} // namespace bd
