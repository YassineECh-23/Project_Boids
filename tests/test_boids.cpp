//
// Created by Yassine on 28/12/2025.
//

#include <gtest/gtest.h>
#include "../include/bd/Boids.h"
#include "../include/bd/Settings.h"
#include "../include/bd/Vec2.h"

using namespace bd;

class BoidTest : public ::testing::Test {
protected:
    // On crée une instance de Settings par défaut pour les tests
    Settings settings;

    void SetUp() override {
        // Initialisation commune si besoin
        // Par défaut: vmax = 4.0, maxAccel = 0.1, etc.
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

    // dt = 1.0 pour simplifier le calcul mental
    b.update(noForce, settings, 1.0f);

    // Nouvelle pos = 100 + 2*1 = 102
    EXPECT_FLOAT_EQ(b.getPosition().x, 102.0f);
    EXPECT_FLOAT_EQ(b.getPosition().y, 100.0f);
}

// Test de l'application d'une force (acceleration)
TEST_F(BoidTest, UpdateAvecForce) {
    Boid b(Vec2<float>(0.0f, 0.0f), Vec2<float>(1.0f, 0.0f));

    // Force valide (inférieure à maxAccel qui est 0.1 par défaut)
    Vec2<float> force(0.05f, 0.0f);

    b.update(force, settings, 1.0f);

    // La vitesse doit avoir augmenté de 0.05
    EXPECT_FLOAT_EQ(b.getVelocity().x, 1.05f);
}

// Test de la limitation de vitesse (vmax)
TEST_F(BoidTest, LimiteVitesseMax) {
    settings.vmax = 5.0f;

    // Boid déjà à la vitesse max
    Boid b(Vec2<float>(0.0f, 0.0f), Vec2<float>(5.0f, 0.0f));

    // On essaie d'accélérer encore
    Vec2<float> force(0.1f, 0.0f); // Force dans le même sens

    // On applique plusieurs fois pour être sûr
    b.update(force, settings, 1.0f);
    b.update(force, settings, 1.0f);

    // La vitesse ne doit pas dépasser 5.0
    EXPECT_LE(b.getVelocity().length(), settings.vmax + 0.001f); // LE = Less or Equal
    EXPECT_FLOAT_EQ(b.getVelocity().length(), 5.0f);
}

// Test de la limitation de l'accélération (maxAccel)
TEST_F(BoidTest, LimiteAccelerationMax) {
    settings.maxAccel = 0.1f;

    Boid b(Vec2<float>(0.0f, 0.0f), Vec2<float>(0.0f, 0.0f));
    Vec2<float> hugeForce(100.0f, 0.0f); // Force énorme

    float dt = 1.0f;
    b.update(hugeForce, settings, dt);

    // La vitesse acquise ne doit être que de maxAccel * dt
    // Car accel = clamp(force) -> 0.1
    // vel += accel * dt -> 0.1 * 1.0 = 0.1
    EXPECT_NEAR(b.getVelocity().x, 0.1f, 0.0001f);
}

// Test de la gestion des bords (Teleportation / Wrap)
TEST_F(BoidTest, HandleBoundsWrap) {
    float width = 800.0f;
    float height = 600.0f;

    // Cas 1 : Sortie à gauche
    Boid bLeft(Vec2<float>(-5.0f, 300.0f), Vec2<float>(-1.0f, 0.0f));
    bLeft.handleBounds(width, height);
    EXPECT_FLOAT_EQ(bLeft.getPosition().x, width); // Doit être à 800

    // Cas 2 : Sortie à droite
    Boid bRight(Vec2<float>(805.0f, 300.0f), Vec2<float>(1.0f, 0.0f));
    bRight.handleBounds(width, height);
    EXPECT_FLOAT_EQ(bRight.getPosition().x, 0.0f); // Doit être à 0

    // Cas 3 : Sortie en haut
    Boid bTop(Vec2<float>(400.0f, -5.0f), Vec2<float>(0.0f, -1.0f));
    bTop.handleBounds(width, height);
    EXPECT_FLOAT_EQ(bTop.getPosition().y, height);

    // Cas 4 : Sortie en bas
    Boid bBottom(Vec2<float>(400.0f, 605.0f), Vec2<float>(0.0f, 1.0f));
    bBottom.handleBounds(width, height);
    EXPECT_FLOAT_EQ(bBottom.getPosition().y, 0.0f);
}