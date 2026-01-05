#include "../../include/bd/InterfaceMenuSFML.h"
#include "../../include/bd/Simulation.h"

#include <iostream>
#include <sstream>
#include <cctype>

namespace bd {

InterfaceMenuSFML::InterfaceMenuSFML()
    : window(sf::VideoMode(800, 600), "Boids - Menu") {
    window.setFramerateLimit(60);
    initUI();
}

void InterfaceMenuSFML::initUI() {

    fontLoaded = font.loadFromFile("../assets/Roboto-Regular.ttf");
    if (!fontLoaded) {
        std::cerr << "Failed to load font ../assets/Roboto-Regular.ttf\n";
    }

    // -- Configuration initiale des textes et boutons --

    // Title
    txtTitle.setString("BOIDS");
    txtTitle.setCharacterSize(56);
    if (fontLoaded) txtTitle.setFont(font);

    // Style commun des boutons
    auto setupBtnStyle = [&](sf::RectangleShape& b) {
        b.setSize({260.f, 70.f});
        b.setFillColor(sf::Color(40, 40, 40));
        b.setOutlineThickness(2.f);
        b.setOutlineColor(sf::Color(180, 180, 180));
    };

    setupBtnStyle(btnRun);
    setupBtnStyle(btnSettings);
    setupBtnStyle(btnQuit);

    // Textes des boutons
    auto setupTextStyle = [&](sf::Text& t, const std::string& s, unsigned size) {
        t.setString(s);
        t.setCharacterSize(size);
        if (fontLoaded) t.setFont(font);
    };

    setupTextStyle(txtRun, "RUN", 26);
    setupTextStyle(txtSettings, "SETTINGS", 26);
    setupTextStyle(txtQuit, "QUIT", 26);

    // Boutons Back / Start
    btnBack.setSize({180.f, 55.f});
    btnBack.setFillColor(sf::Color(40, 40, 40));
    btnBack.setOutlineThickness(2.f);
    btnBack.setOutlineColor(sf::Color(180, 180, 180));

    btnStart.setSize({180.f, 55.f});
    btnStart.setFillColor(sf::Color(40, 40, 40));
    btnStart.setOutlineThickness(2.f);
    btnStart.setOutlineColor(sf::Color(180, 180, 180));

    setupTextStyle(txtBack, "BACK", 20);
    setupTextStyle(txtStart, "RUN", 20);

    // Hint & Error
    txtHint.setString("Clique un champ puis tape un nombre. Enter = valider. ESC = retour.");
    txtHint.setCharacterSize(18);
    if (fontLoaded) txtHint.setFont(font);

    txtError.setCharacterSize(20);
    txtError.setFillColor(sf::Color::Red);
    if (fontLoaded) txtError.setFont(font);

    // --- APPLICATION DU CENTRAGE INITIAL ---
    updateLayout();
}

// --- GESTION DU REDIMENSIONNEMENT ---
void InterfaceMenuSFML::updateLayout() {
    sf::Vector2u size = window.getSize();
    float winW = static_cast<float>(size.x);
    float winH = static_cast<float>(size.y);
    float centerX = winW / 2.0f;

    // 1. Centrer le Titre
    sf::FloatRect tr = txtTitle.getLocalBounds();
    txtTitle.setOrigin(tr.left + tr.width / 2.0f, tr.top + tr.height / 2.0f);
    txtTitle.setPosition(centerX, winH * 0.15f);

    // 2. Centrer le Menu Principal
    float btnW = btnRun.getSize().x;
    float btnH = btnRun.getSize().y;
    float spacing = 20.f;
    float startY = winH * 0.35f;

    auto centerBtn = [&](sf::RectangleShape& b, sf::Text& t, float y) {
        b.setPosition(centerX - btnW / 2.0f, y);
        // Centrer le texte dans le bouton
        sf::FloatRect tRect = t.getLocalBounds();
        t.setOrigin(tRect.left + tRect.width / 2.0f, tRect.top + tRect.height / 2.0f);
        t.setPosition(centerX, y + btnH / 2.0f);
    };

    centerBtn(btnRun, txtRun, startY);
    centerBtn(btnSettings, txtSettings, startY + btnH + spacing);
    centerBtn(btnQuit, txtQuit, startY + (btnH + spacing) * 2);

    // 3. Boutons Footer (Back / Start)
    float footerY = winH - 80.f;

    // Back à 10% gauche
    btnBack.setPosition(winW * 0.1f, footerY);
    // Start à 10% droite (aligné à droite)
    btnStart.setPosition(winW * 0.9f - btnStart.getSize().x, footerY);

    // Centrer texte Back
    sf::FloatRect br = txtBack.getLocalBounds();
    txtBack.setOrigin(br.left + br.width / 2.0f, br.top + br.height / 2.0f);
    txtBack.setPosition(btnBack.getPosition().x + btnBack.getSize().x / 2.0f,
                        btnBack.getPosition().y + btnBack.getSize().y / 2.0f);

    // Centrer texte Start
    sf::FloatRect sr = txtStart.getLocalBounds();
    txtStart.setOrigin(sr.left + sr.width / 2.0f, sr.top + sr.height / 2.0f);
    txtStart.setPosition(btnStart.getPosition().x + btnStart.getSize().x / 2.0f,
                         btnStart.getPosition().y + btnStart.getSize().y / 2.0f);
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

        // --- GESTION RESIZE ---
        if (e.type == sf::Event::Resized) {
            // Ajuster la vue pour éviter l'étirement
            sf::FloatRect visibleArea(0, 0, (float)e.size.width, (float)e.size.height);
            window.setView(sf::View(visibleArea));
            // Recalculer les positions
            updateLayout();
        }

        // --- Keyboard for settings screen ---
        if (screen == Screen::Settings) {
            if (e.type == sf::Event::KeyPressed) {
                if (e.key.code == sf::Keyboard::Escape) {
                    screen = Screen::Main;
                    selectedIndex = -1;
                    inputBuffer.clear();
                    txtError.setString("");
                }
                if (e.key.code == sf::Keyboard::Enter) {
                    commitInput();
                }
                if (e.key.code == sf::Keyboard::BackSpace) {
                    backspaceInput();
                }
            }

            if (e.type == sf::Event::TextEntered) {
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
                    launchSimulation();
                } else if (isClicked(btnSettings, mouse)) {
                    screen = Screen::Settings;
                    selectedIndex = -1;
                    inputBuffer.clear();
                    txtError.setString("");
                } else if (isClicked(btnQuit, mouse)) {
                    window.close();
                }
            } else { // Settings screen
                if (isClicked(btnBack, mouse)) {
                    screen = Screen::Main;
                    selectedIndex = -1;
                    inputBuffer.clear();
                    txtError.setString("");
                } else if (isClicked(btnStart, mouse)) {
                    if (selectedIndex != -1) commitInput();
                    if (txtError.getString().isEmpty()) {
                        launchSimulation();
                    }
                } else {
                    // Clic sur champs : on doit recalculer les zones car elles bougent
                    float winW = static_cast<float>(window.getSize().x);
                    float centerX = winW / 2.0f;

                    // --- NOUVEAU : Paramètres d'espacement (doit matcher renderSettings) ---
                    float y0 = 130.f;
                    float dy = 45.f; // Espacement réduit

                    for (int i = 0; i < 8; ++i) { // <--- Boucle jusqu'à 8
                        float y = y0 + i * dy;
                        // La boite est centrée en X, largeur 220, hauteur ~35
                        sf::FloatRect valueRect(centerX, y, 220.f, 35.f);
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
    if (selectedIndex == -1) return;
    if (inputBuffer.size() < 12) inputBuffer.push_back(c);
}

void InterfaceMenuSFML::backspaceInput() {
    if (selectedIndex == -1) return;
    if (!inputBuffer.empty()) inputBuffer.pop_back();
}

void InterfaceMenuSFML::selectField(int index) {
    selectedIndex = index;
    inputBuffer.clear();

    txtError.setString("");
    std::ostringstream oss;
    switch (index) {
        case 0: oss << settings.nbBoids; break;
        case 1: oss << settings.r; break;
        case 2: oss << settings.dmin; break;
        case 3: oss << settings.wcoh; break;
        case 4: oss << settings.wsep; break;
        case 5: oss << settings.wali; break;
        // --- NOUVEAU : Résolution ---
        case 6: oss << (int)settings.windowWidth; break;
        case 7: oss << (int)settings.windowHeight; break;
    }
    inputBuffer = oss.str();
}

void InterfaceMenuSFML::commitInput() {
    if (selectedIndex == -1) return;
    if (inputBuffer.empty() || inputBuffer == "-" || inputBuffer == ".") return;

    txtError.setString("");

    try {
        // --- CAS nbBoids ---
        if (selectedIndex == 0) {
            int v = std::stoi(inputBuffer);
            if (v < settings.nbBoidsMin || v > settings.nbBoidsMax) {
                txtError.setString("Erreur: Min = " + std::to_string(settings.nbBoidsMin) +
                                   ", Max = " + std::to_string(settings.nbBoidsMax));
                return;
            }
            settings.nbBoids = v;
        }
        // --- CAS Largeur / Hauteur (indices 6 et 7) ---
        else if (selectedIndex == 6 || selectedIndex == 7) {
            float v = std::stof(inputBuffer);
            float minVal = (selectedIndex == 6) ? settings.widthMin : settings.heightMin;
            float maxVal = (selectedIndex == 6) ? settings.widthMax : settings.heightMax;

            if (v < minVal || v > maxVal) {
                txtError.setString("Erreur: Min = " + std::to_string((int)minVal) +
                                   ", Max = " + std::to_string((int)maxVal));
                return;
            }

            if (selectedIndex == 6) settings.windowWidth = v;
            else settings.windowHeight = v;
        }
        // --- AUTRES PARAMETRES ---
        else {
            float v = std::stof(inputBuffer);
            float minVal = 0.f, maxVal = 0.f;

            if (selectedIndex == 1) { minVal = settings.rMin; maxVal = settings.rMax; }
            else if (selectedIndex == 2) { minVal = settings.dminMin; maxVal = settings.dminMax; }
            else { minVal = settings.weightMin; maxVal = settings.weightMax; }

            if (v < minVal || v > maxVal) {
                std::string sMin = std::to_string(minVal);
                std::string sMax = std::to_string(maxVal);
                // Nettoyage string
                sMin.erase(sMin.find_last_not_of('0') + 1, std::string::npos);
                sMax.erase(sMax.find_last_not_of('0') + 1, std::string::npos);
                if (sMin.back() == '.') sMin.pop_back();
                if (sMax.back() == '.') sMax.pop_back();

                txtError.setString("Erreur: Valeur attendue entre " + sMin + " et " + sMax);
                return;
            }

            if (selectedIndex == 1) settings.r = v;
            else if (selectedIndex == 2) settings.dmin = v;
            else if (selectedIndex == 3) settings.wcoh = v;
            else if (selectedIndex == 4) settings.wsep = v;
            else if (selectedIndex == 5) settings.wali = v;
        }

        std::ostringstream oss;
        if (selectedIndex == 0) oss << settings.nbBoids;
        else if (selectedIndex == 1) oss << settings.r;
        else if (selectedIndex == 2) oss << settings.dmin;
        else if (selectedIndex == 3) oss << settings.wcoh;
        else if (selectedIndex == 4) oss << settings.wsep;
        else if (selectedIndex == 5) oss << settings.wali;
        else if (selectedIndex == 6) oss << (int)settings.windowWidth;
        else if (selectedIndex == 7) oss << (int)settings.windowHeight;

        inputBuffer = oss.str();

    } catch (...) {
        txtError.setString("Erreur: Format nombre invalide");
    }
}

void InterfaceMenuSFML::update() {
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
    // Récupérer le centre pour positionner les éléments dynamiquement
    float winW = static_cast<float>(window.getSize().x);
    float centerX = winW / 2.0f;

    // Centrer Hint et Error
    sf::FloatRect hintRect = txtHint.getLocalBounds();
    txtHint.setOrigin(hintRect.left + hintRect.width/2.0f, 0);
    txtHint.setPosition(centerX, 100.f); // Un peu remonté
    window.draw(txtHint);

    sf::FloatRect errRect = txtError.getLocalBounds();
    txtError.setOrigin(errRect.left + errRect.width/2.0f, 0);
    txtError.setPosition(centerX, 75.f); // Un peu remonté
    window.draw(txtError);

    // Titre Settings
    sf::Text title;
    title.setString("SETTINGS");
    title.setCharacterSize(34);
    if (fontLoaded) title.setFont(font);

    sf::FloatRect tr = title.getLocalBounds();
    title.setOrigin(tr.left + tr.width/2.0f, tr.top + tr.height/2.0f);
    title.setPosition(centerX, 45.f); // Un peu remonté
    window.draw(title);

    // --- LISTE DES PARAMETRES (8 lignes maintenant) ---
    const char* names[8] = {"nbBoids", "r", "dmin", "wcoh", "wsep", "wali", "Width", "Height"};

    // Paramètres d'espacement (resserrés)
    float y0 = 130.f;
    float dy = 45.f;

    for (int i = 0; i < 8; ++i) { // <--- Boucle jusqu'à 8
        float y = y0 + i * dy;

        // Label à gauche du centre
        sf::Text label;
        label.setString(names[i]);
        label.setCharacterSize(20); // Un peu plus petit
        if (fontLoaded) label.setFont(font);

        label.setPosition(centerX - 220.f, y + 5.f);
        window.draw(label);

        // Zone valeur (input box) au centre
        sf::RectangleShape box({220.f, 35.f}); // Hauteur réduite à 35
        box.setPosition(centerX, y);
        box.setFillColor(sf::Color(30, 30, 35));

        if (i == selectedIndex) {
            box.setOutlineColor(sf::Color(255, 200, 80));
            if (!txtError.getString().isEmpty()) {
                 box.setOutlineColor(sf::Color::Red);
            }
        } else {
            box.setOutlineColor(sf::Color(120, 120, 120));
        }

        box.setOutlineThickness(2.f);
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
                case 6: oss << (int)settings.windowWidth; break; // Cast int
                case 7: oss << (int)settings.windowHeight; break;// Cast int
            }
        }

        sf::Text val;
        val.setString(oss.str());
        val.setCharacterSize(20);
        // Texte un peu décalé dans la boite
        val.setPosition(centerX + 15.f, y + 4.f);
        if (fontLoaded) val.setFont(font);
        window.draw(val);
    }

    // Boutons de navigation (déjà positionnés par updateLayout)
    window.draw(btnBack);
    window.draw(btnStart);
    window.draw(txtBack);
    window.draw(txtStart);
}

void InterfaceMenuSFML::launchSimulation() {
    window.setVisible(false);

    bd::Simulation sim(settings);
    sim.run();

    window.setVisible(true);
}

} // namespace bd