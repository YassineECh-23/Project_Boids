//
// tests/test_boids.cpp
//

#include <gtest/gtest.h>
#include "../include/bd/Boids.h"
#include "../include/bd/Settings.h"
#include "../include/bd/Vec2.h"

using namespace bd;

class BoidTest : public ::testing::Test {
protected:
    Settings settings;

    void SetUp() override {
        // Configuration par défaut pour les tests
        settings.vmax = 4.0f;
        settings.maxAccel = 0.1f;
        settings.windowWidth = 800.0f;
        settings.windowHeight = 600.0f;
        settings.enableBounce = false;
    }
};

// Test des constructeurs
TEST_F(BoidTest, ConstructeurParDefaut) {
    Boid b;
    EXPECT_FLOAT_EQ(b.getPosition().x, 0.0f);
    EXPECT_FLOAT_EQ(b.getPosition().y, 0.0f);
    EXPECT_FLOAT_EQ(b.getVelocity().x, 0.0f);
    EXPECT_FLOAT_EQ(b.getVelocity().y, 0.0f);
}

TEST_F(BoidTest, ConstructeurAvecParametres) {
    Vec2<float> pos(10.0f, 20.0f);
    Vec2<float> vel(1.0f, 2.0f);
    Boid b(pos, vel);

    EXPECT_EQ(b.getPosition(), pos);
    EXPECT_EQ(b.getVelocity(), vel);
}

// Test de mise à jour simple sans forces
TEST_F(BoidTest, UpdateSansForce) {
    Boid b(Vec2<float>(100.0f, 100.0f), Vec2<float>(2.0f, 0.0f));
    Vec2<float> noForce(0.0f, 0.0f);

    b.update(noForce, settings, 1.0f);

    EXPECT_FLOAT_EQ(b.getPosition().x, 102.0f);
    EXPECT_FLOAT_EQ(b.getPosition().y, 100.0f);
}

// Test de l'application d'une force
TEST_F(BoidTest, UpdateAvecForce) {
    Boid b(Vec2<float>(0.0f, 0.0f), Vec2<float>(1.0f, 0.0f));
    Vec2<float> force(0.05f, 0.0f); // Force < maxAccel

    b.update(force, settings, 1.0f);

    EXPECT_FLOAT_EQ(b.getVelocity().x, 1.05f);
}

// Test de la limitation de vitesse (vmax)
TEST_F(BoidTest, LimiteVitesseMax) {
    settings.vmax = 5.0f;
    Boid b(Vec2<float>(0.0f, 0.0f), Vec2<float>(5.0f, 0.0f));
    Vec2<float> force(0.1f, 0.0f); // Force qui accélère encore

    b.update(force, settings, 1.0f);

    // La vitesse ne doit pas dépasser 5.0
    EXPECT_LE(b.getVelocity().length(), settings.vmax + 0.001f);
}

// Test de la gestion des bords (Teleportation / Wrap)
TEST_F(BoidTest, HandleBoundsWrap) {
    // On s'assure qu'on est en mode WRAP
    settings.enableBounce = false;
    settings.windowWidth = 800.0f;
    settings.windowHeight = 600.0f;

    // Cas 1 : Sortie à gauche -> Doit aller à droite (800)
    Boid bLeft(Vec2<float>(-5.0f, 300.0f), Vec2<float>(-1.0f, 0.0f));
    bLeft.handleBounds(settings); // <--- CORRECTION ICI (on passe settings)
    EXPECT_FLOAT_EQ(bLeft.getPosition().x, 800.0f);

    // Cas 2 : Sortie à droite -> Doit aller à gauche (0)
    Boid bRight(Vec2<float>(805.0f, 300.0f), Vec2<float>(1.0f, 0.0f));
    bRight.handleBounds(settings);
    EXPECT_FLOAT_EQ(bRight.getPosition().x, 0.0f);
}

// NOUVEAU TEST : Gestion des bords (Rebond / Bounce)
TEST_F(BoidTest, HandleBoundsBounce) {
    // On active le mode REBOND
    settings.enableBounce = true;
    settings.windowWidth = 800.0f;

    // Marge définie dans le code = 10.0f
    float margin = 10.0f;

    // On place un boid trop à gauche qui va vers la gauche
    Boid b(Vec2<float>(5.0f, 300.0f), Vec2<float>(-10.0f, 0.0f));

    b.handleBounds(settings);

    // 1. Il doit être replacé à la marge (10.0f)
    EXPECT_FLOAT_EQ(b.getPosition().x, margin);

    // 2. Sa vitesse doit être inversée (positive)
    EXPECT_GT(b.getVelocity().x, 0.0f); // Doit être > 0
    EXPECT_FLOAT_EQ(b.getVelocity().x, 10.0f);
}