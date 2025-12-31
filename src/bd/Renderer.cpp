#include "../../include/bd/Renderer.h"
#include <cmath>      // atan2
#include <sstream>
#include <iostream>

namespace bd {

Renderer::Renderer(sf::RenderWindow& window) : window_(window) {
    // Triangle isocèle qui pointe vers la droite (0°)
    boidShape_.setPointCount(3);
    boidShape_.setPoint(0, sf::Vector2f(10, 0));   // Pointe
    boidShape_.setPoint(1, sf::Vector2f(-5, -5));  // Arrière gauche
    boidShape_.setPoint(2, sf::Vector2f(-5, 5));   // Arrière droite

    boidShape_.setFillColor(sf::Color::Cyan);
    boidShape_.setOutlineColor(sf::Color::White);
    boidShape_.setOutlineThickness(1.f);

    // Police (chez toi: ../assets marche)
    fontLoaded_ = font_.loadFromFile("../assets/Roboto-Regular.ttf");
    if (!fontLoaded_) {
        std::cerr << "[Renderer] Failed to load font ../assets/Roboto-Regular.ttf\n";
    }
}

void Renderer::drawBoid(const Boid& boid) {
    // ✅ TON API : getters
    Vec2<float> pos = boid.getPosition();
    Vec2<float> vel = boid.getVelocity();

    boidShape_.setPosition(pos.x, pos.y);

    float angle = std::atan2(vel.y, vel.x) * 180.0f / 3.14159f;
    boidShape_.setRotation(angle);

    window_.draw(boidShape_);
}

    void Renderer::drawFlock(const Flock& flock) {
    for (size_t i = 0; i < flock.size(); ++i) {
        drawBoid(flock.getBoid(i));  // ✅ API correcte de TON Flock
    }
}


void Renderer::drawUI(const Settings& settings) {
    if (!fontLoaded_) return;

    sf::Text txt;
    txt.setFont(font_);
    txt.setCharacterSize(14);
    txt.setFillColor(sf::Color::White);
    txt.setPosition(10.f, 10.f);

    std::ostringstream oss;
    oss << "nbBoids : " << settings.nbBoids << "\n"
        << "r      : " << settings.r << "\n"
        << "dmin   : " << settings.dmin << "\n"
        << "wcoh   : " << settings.wcoh << "\n"
        << "wsep   : " << settings.wsep << "\n"
        << "wali   : " << settings.wali << "\n";

    txt.setString(oss.str());
    window_.draw(txt);
}

void Renderer::drawDebug(const Settings& settings) {
    (void)settings;
    // tu peux garder vide, drawUI fait déjà l'affichage
}

} // namespace bd
