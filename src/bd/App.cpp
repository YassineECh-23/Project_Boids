#include "../../include/bd/App.h"
#include "../../include/bd/CohesionRule.h"
#include "../../include/bd/SeparationRule.h"
#include "../../include/bd/AlignmentRule.h"

#include <ctime>
#include <cstdlib>
#include <iostream>
/////////
namespace bd {

App::App()
: settings(),
  window(sf::VideoMode((unsigned)settings.windowWidth, (unsigned)settings.windowHeight), "Test Phase 3 - Boids"),
  renderer(window),
  flock(),
  rules()
{
    window.setFramerateLimit(60);
}

App::~App() {
    for (size_t i = 0; i < rules.size(); ++i) delete rules[i];
}

void App::init() {
    std::srand((unsigned)std::time(nullptr));

    // Tu gardes exactement TES valeurs
    settings.windowWidth = 1200;
    settings.windowHeight = 800;
    settings.nbBoids = 100;
    settings.enableBounce = false;

    settings.r = 60.0f;
    settings.dmin = 25.0f;
    settings.vmax = 5.0f;
    settings.wcoh = 0.008f;
    settings.wsep = 0.05f;
    settings.wali = 0.1f;

    // IMPORTANT : recréer window si tu changes width/height après ctor
    window.create(sf::VideoMode((unsigned)settings.windowWidth, (unsigned)settings.windowHeight), "Test Phase 3 - Boids");
    window.setFramerateLimit(60);

    flock.initRandom(settings.nbBoids, settings.windowWidth, settings.windowHeight);

    // règles
    rules.push_back(new CohesionRule());
    rules.push_back(new SeparationRule());
    rules.push_back(new AlignmentRule());

    std::cout << "=== COMMANDES ===\n";
    std::cout << "[ESPACE] : Reinitialiser les positions\n";
    std::cout << "[B]      : Activer/Desactiver le REBOND (Bounce)\n";
}

void App::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Space) {
                flock.initRandom(settings.nbBoids, settings.windowWidth, settings.windowHeight);
                std::cout << "Reset !\n";
            }
            if (event.key.code == sf::Keyboard::B) {
                settings.enableBounce = !settings.enableBounce;
                if (settings.enableBounce)
                    std::cout << "Mode: REBOND (Murs solides)\n";
                else
                    std::cout << "Mode: WRAP (Teleportation)\n";
            }
        }
    }
}

void App::update(float dt) {
    flock.updateAll(rules, settings, dt);
}

void App::render() {
    window.clear(sf::Color::Black);
    for (int i = 0; i < flock.size(); ++i) {
        renderer.drawBoid(flock.getBoid(i));
    }
    window.display();
}

} // namespace bd
