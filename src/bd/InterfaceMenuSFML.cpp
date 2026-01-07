#include "../../include/bd/InterfaceMenuSFML.h"
#include "../../include/bd/Simulation.h"
#include "../../include/bd/SaveManager.h"

#include <iostream>
#include <sstream>
#include <cctype>
#include <filesystem>

namespace bd {

InterfaceMenuSFML::InterfaceMenuSFML()
    : window(sf::VideoMode(800, 600), "Boids - Menu") {
    window.setFramerateLimit(60);
    initUI();
}

void InterfaceMenuSFML::initUI() {
    fontLoaded = font.loadFromFile("assets/Roboto-Regular.ttf");
    if (!fontLoaded) std::cerr << "Failed to load font\n";

    txtTitle.setString("BOIDS");
    txtTitle.setCharacterSize(56);
    if (fontLoaded) txtTitle.setFont(font);

    // Style Boutons
    auto setupBtnStyle = [&](sf::RectangleShape& b) {
        b.setSize({260.f, 70.f});
        b.setFillColor(sf::Color(40, 40, 40));
        b.setOutlineThickness(2.f);
        b.setOutlineColor(sf::Color(180, 180, 180));
    };

    setupBtnStyle(btnRun);
    setupBtnStyle(btnLoad);
    setupBtnStyle(btnSettings);
    setupBtnStyle(btnQuit);

    // Style Textes
    auto setupTextStyle = [&](sf::Text& t, const std::string& s, unsigned size) {
        t.setString(s);
        t.setCharacterSize(size);
        if (fontLoaded) t.setFont(font);
    };

    setupTextStyle(txtRun, "RUN", 26);
    setupTextStyle(txtLoad, "LOAD", 26);
    setupTextStyle(txtSettings, "SETTINGS", 26);
    setupTextStyle(txtQuit, "QUIT", 26);

    // Boutons Back/Start (communs à Settings et Load)
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
    txtHint.setString("Clique un champ puis tape un nombre. Enter = valider.");
    txtHint.setCharacterSize(18);
    if (fontLoaded) txtHint.setFont(font);

    txtError.setCharacterSize(20);
    txtError.setFillColor(sf::Color::Red);
    if (fontLoaded) txtError.setFont(font);

    // Bouton Refresh (Load screen)
    btnRefresh.setSize({180.f, 40.f});
    btnRefresh.setFillColor(sf::Color(60, 60, 60));
    setupTextStyle(txtRefresh, "REFRESH", 18);

    updateLayout();
}

void InterfaceMenuSFML::refreshSaves() {
    saveFiles = SaveManager::listSaves();
}

void InterfaceMenuSFML::updateLayout() {
    sf::Vector2u size = window.getSize();
    float winW = static_cast<float>(size.x);
    float winH = static_cast<float>(size.y);
    float centerX = winW / 2.0f;

    // Titre
    sf::FloatRect tr = txtTitle.getLocalBounds();
    txtTitle.setOrigin(tr.left + tr.width / 2.0f, tr.top + tr.height / 2.0f);
    txtTitle.setPosition(centerX, winH * 0.15f);

    // Main Menu Buttons
    float btnW = btnRun.getSize().x;
    float btnH = btnRun.getSize().y;
    float spacing = 15.f;
    float startY = winH * 0.30f;

    auto centerBtn = [&](sf::RectangleShape& b, sf::Text& t, float y) {
        b.setPosition(centerX - btnW / 2.0f, y);
        sf::FloatRect tRect = t.getLocalBounds();
        t.setOrigin(tRect.left + tRect.width / 2.0f, tRect.top + tRect.height / 2.0f);
        t.setPosition(centerX, y + btnH / 2.0f);
    };

    centerBtn(btnRun, txtRun, startY);
    centerBtn(btnLoad, txtLoad, startY + btnH + spacing);
    centerBtn(btnSettings, txtSettings, startY + (btnH + spacing) * 2);
    centerBtn(btnQuit, txtQuit, startY + (btnH + spacing) * 3);

    // Footer Buttons (Back / Start)
    float footerY = winH - 80.f;
    btnBack.setPosition(winW * 0.1f, footerY);
    btnStart.setPosition(winW * 0.9f - btnStart.getSize().x, footerY);

    sf::FloatRect br = txtBack.getLocalBounds();
    txtBack.setOrigin(br.left + br.width / 2.0f, br.top + br.height / 2.0f);
    txtBack.setPosition(btnBack.getPosition().x + btnBack.getSize().x / 2.0f,
                        btnBack.getPosition().y + btnBack.getSize().y / 2.0f);

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

        if (e.type == sf::Event::Resized) {
            sf::FloatRect visibleArea(0, 0, (float)e.size.width, (float)e.size.height);
            window.setView(sf::View(visibleArea));
            updateLayout();
        }

        // --- Clavier (Settings) ---
        if (screen == Screen::Settings) {
            if (e.type == sf::Event::KeyPressed) {
                if (e.key.code == sf::Keyboard::Escape) { screen = Screen::Main; }
                if (e.key.code == sf::Keyboard::Enter) { commitInput(); }
                if (e.key.code == sf::Keyboard::BackSpace) { backspaceInput(); }
            }
            if (e.type == sf::Event::TextEntered) {
                char c = static_cast<char>(e.text.unicode);
                if (c >= 32 && c <= 126) if (std::isdigit((unsigned char)c) || c == '.' || c == '-') appendChar(c);
            }
        }

        // --- Mouse Clicks ---
        if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mouse = window.mapPixelToCoords({e.mouseButton.x, e.mouseButton.y});

            // 1. MENU PRINCIPAL
            if (screen == Screen::Main) {
                if (isClicked(btnRun, mouse)) {
                    launchSimulation();
                }
                else if (isClicked(btnLoad, mouse)) {
                    screen = Screen::Load;
                    refreshSaves(); // Charger la liste
                }
                else if (isClicked(btnSettings, mouse)) {
                    screen = Screen::Settings;
                    selectedIndex = -1; inputBuffer.clear(); txtError.setString("");
                }
                else if (isClicked(btnQuit, mouse)) {
                    window.close();
                }
            }
            // 2. ECRAN SETTINGS
            else if (screen == Screen::Settings) {
                if (isClicked(btnBack, mouse)) screen = Screen::Main;
                else if (isClicked(btnStart, mouse)) { if(selectedIndex!=-1) commitInput(); if(txtError.getString().isEmpty()) launchSimulation(); }
                else {
                    float winW = static_cast<float>(window.getSize().x);
                    float centerX = winW / 2.0f;
                    float y0 = 130.f; float dy = 45.f;
                    for (int i = 0; i < 8; ++i) {
                        if (sf::FloatRect(centerX, y0 + i * dy, 220.f, 35.f).contains(mouse)) selectField(i);
                    }
                }
            }
            // 3. ECRAN LOAD
            else if (screen == Screen::Load) {
                if (isClicked(btnBack, mouse)) {
                    screen = Screen::Main;
                }
                else {
                    // Vérifier clic sur un fichier
                    float winW = static_cast<float>(window.getSize().x);
                    float centerX = winW / 2.0f;
                    float startY = 120.f;
                    float itemH = 40.f;
                    float spacing = 10.f;

                    for (size_t i = 0; i < saveFiles.size(); ++i) {
                        float y = startY + i * (itemH + spacing);
                        sf::FloatRect itemRect(centerX - 250.f, y, 500.f, itemH);
                        if (itemRect.contains(mouse)) {
                            // Charger ce fichier spécifique
                            launchSimulation(saveFiles[i]);
                        }
                    }
                }
            }
        }
    }
}

void InterfaceMenuSFML::appendChar(char c) { if (selectedIndex != -1 && inputBuffer.size() < 12) inputBuffer.push_back(c); }
void InterfaceMenuSFML::backspaceInput() { if (selectedIndex != -1 && !inputBuffer.empty()) inputBuffer.pop_back(); }
void InterfaceMenuSFML::selectField(int index) {
    selectedIndex = index; inputBuffer.clear(); txtError.setString("");
    std::ostringstream oss;
    switch (index) {
        case 0: oss << settings.nbBoids; break;
        case 1: oss << settings.r; break;
        case 2: oss << settings.dmin; break;
        case 3: oss << settings.wcoh; break;
        case 4: oss << settings.wsep; break;
        case 5: oss << settings.wali; break;
        case 6: oss << (int)settings.windowWidth; break;
        case 7: oss << (int)settings.windowHeight; break;
    }
    inputBuffer = oss.str();
}
void InterfaceMenuSFML::commitInput() {
    if (selectedIndex == -1 || inputBuffer.empty()) return;
    try {
        if(selectedIndex == 0) settings.nbBoids = std::stoi(inputBuffer);
        else if (selectedIndex >= 6) { float v = std::stof(inputBuffer); if(selectedIndex==6) settings.windowWidth=v; else settings.windowHeight=v; }
        else { float v = std::stof(inputBuffer);
            if(selectedIndex==1) settings.r=v; else if(selectedIndex==2) settings.dmin=v;
            else if(selectedIndex==3) settings.wcoh=v; else if(selectedIndex==4) settings.wsep=v; else if(selectedIndex==5) settings.wali=v;
        }
    } catch(...) {}
}

void InterfaceMenuSFML::update() {
    sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    auto paint = [&](sf::RectangleShape& b) {
        b.setFillColor(b.getGlobalBounds().contains(mouse) ? sf::Color(70,70,70) : sf::Color(40,40,40));
    };

    if (screen == Screen::Main) {
        paint(btnRun); paint(btnLoad); paint(btnSettings); paint(btnQuit);
    }
    else if (screen == Screen::Settings) {
        paint(btnBack); paint(btnStart);
    }
    else if (screen == Screen::Load) {
        paint(btnBack);
        // On ne peint pas les items de liste ici (fait dans renderLoad pour simplification)
    }
}

void InterfaceMenuSFML::render() {
    window.clear(sf::Color(15, 15, 20));
    if (screen == Screen::Main) renderMain();
    else if (screen == Screen::Settings) renderSettings();
    else if (screen == Screen::Load) renderLoad();
    window.display();
}

void InterfaceMenuSFML::renderMain() {
    window.draw(txtTitle);
    window.draw(btnRun); window.draw(btnLoad); window.draw(btnSettings); window.draw(btnQuit);
    window.draw(txtRun); window.draw(txtLoad); window.draw(txtSettings); window.draw(txtQuit);
}

void InterfaceMenuSFML::renderSettings() {
    // (Garde le code existant de renderSettings ici)
    float winW = static_cast<float>(window.getSize().x);
    float centerX = winW / 2.0f;
    sf::FloatRect hintRect = txtHint.getLocalBounds(); txtHint.setOrigin(hintRect.left + hintRect.width/2.0f, 0); txtHint.setPosition(centerX, 100.f); window.draw(txtHint);
    sf::FloatRect errRect = txtError.getLocalBounds(); txtError.setOrigin(errRect.left + errRect.width/2.0f, 0); txtError.setPosition(centerX, 75.f); window.draw(txtError);
    sf::Text title; title.setString("SETTINGS"); title.setCharacterSize(34); if (fontLoaded) title.setFont(font);
    sf::FloatRect tr = title.getLocalBounds(); title.setOrigin(tr.left + tr.width/2.0f, tr.top + tr.height/2.0f); title.setPosition(centerX, 45.f); window.draw(title);

    const char* names[8] = {"nbBoids", "r", "dmin", "wcoh", "wsep", "wali", "Width", "Height"};
    float y0 = 130.f; float dy = 45.f;
    for (int i = 0; i < 8; ++i) {
        float y = y0 + i * dy;
        sf::Text label; label.setString(names[i]); label.setCharacterSize(20); if(fontLoaded) label.setFont(font); label.setPosition(centerX - 220.f, y+5.f); window.draw(label);
        sf::RectangleShape box({220.f, 35.f}); box.setPosition(centerX, y); box.setFillColor(sf::Color(30, 30, 35));
        if (i == selectedIndex) box.setOutlineColor(sf::Color(255, 200, 80)); else box.setOutlineColor(sf::Color(120, 120, 120));
        box.setOutlineThickness(2.f); window.draw(box);
        std::ostringstream oss;
        if(i==selectedIndex) oss << inputBuffer;
        else {
             switch(i) { case 0: oss<<settings.nbBoids; break; case 1: oss<<settings.r; break; case 2: oss<<settings.dmin; break;
                         case 3: oss<<settings.wcoh; break; case 4: oss<<settings.wsep; break; case 5: oss<<settings.wali; break;
                         case 6: oss<<(int)settings.windowWidth; break; case 7: oss<<(int)settings.windowHeight; break; }
        }
        sf::Text val; val.setString(oss.str()); val.setCharacterSize(20); val.setPosition(centerX+15.f, y+4.f); if(fontLoaded) val.setFont(font); window.draw(val);
    }
    window.draw(btnBack); window.draw(btnStart); window.draw(txtBack); window.draw(txtStart);
}

void InterfaceMenuSFML::renderLoad() {
    float winW = static_cast<float>(window.getSize().x);
    float centerX = winW / 2.0f;

    // Titre
    sf::Text title;
    title.setString("LOAD GAME");
    title.setCharacterSize(34);
    if (fontLoaded) title.setFont(font);
    sf::FloatRect tr = title.getLocalBounds();
    title.setOrigin(tr.left + tr.width / 2.0f, tr.top + tr.height / 2.0f);
    title.setPosition(centerX, 50.f);
    window.draw(title);

    // Liste des fichiers
    float startY = 120.f;
    float itemH = 40.f;
    float spacing = 10.f;
    sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (saveFiles.empty()) {
        sf::Text emptyTxt;
        emptyTxt.setString("Aucune sauvegarde trouvee.");
        emptyTxt.setCharacterSize(20);
        if (fontLoaded) emptyTxt.setFont(font);
        sf::FloatRect er = emptyTxt.getLocalBounds();
        emptyTxt.setOrigin(er.left + er.width/2.0f, 0);
        emptyTxt.setPosition(centerX, 200.f);
        window.draw(emptyTxt);
    } else {
        for (size_t i = 0; i < saveFiles.size(); ++i) {
            float y = startY + i * (itemH + spacing);

            // Fond de l'item
            sf::RectangleShape itemRect({500.f, itemH});
            itemRect.setPosition(centerX - 250.f, y);

            // Hover effect
            if (itemRect.getGlobalBounds().contains(mouse)) {
                itemRect.setFillColor(sf::Color(70, 70, 80));
                itemRect.setOutlineColor(sf::Color::White);
            } else {
                itemRect.setFillColor(sf::Color(40, 40, 50));
                itemRect.setOutlineColor(sf::Color(100, 100, 100));
            }
            itemRect.setOutlineThickness(1.f);
            window.draw(itemRect);

            // Nom du fichier (on extrait juste le nom pour faire joli)
            std::string pathStr = saveFiles[i];
            std::string filename = std::filesystem::path(pathStr).filename().string();

            sf::Text txtFile;
            txtFile.setString(filename);
            txtFile.setCharacterSize(18);
            if (fontLoaded) txtFile.setFont(font);
            txtFile.setPosition(centerX - 230.f, y + 8.f);
            window.draw(txtFile);
        }
    }

    // Bouton retour
    window.draw(btnBack);
    window.draw(txtBack);
}

void InterfaceMenuSFML::launchSimulation(const std::string& saveFile) {
    window.setVisible(false);
    bd::Simulation sim(settings); // settings par défaut

    if (!saveFile.empty()) {
        sim.loadFromFile(saveFile);
    }

    sim.run();
    window.setVisible(true);
}

} // namespace bd