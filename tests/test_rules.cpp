#include <gtest/gtest.h>
#include "../include/bd/Flock.h"
#include "../include/bd/Settings.h"
#include "../include/bd/Vec2.h"
#include "../include/bd/CohesionRule.h"
#include "../include/bd/SeparationRule.h"
#include "../include/bd/AlignmentRule.h"

using namespace bd;

class RuleTest : public ::testing::Test {
protected:
    Flock flock;
    Settings settings;

    void SetUp() override {
        // Configuration standard pour les tests
        settings.r = 100.0f;    // Rayon de perception large
        settings.dmin = 20.0f;  // Distance min de séparation

        // On s'assure que le flock est vide au départ
        flock.initRandom(0, 800, 600,settings);
    }
};

// =============================================================================
// TEST COHÉSION (Attraction vers le centre du groupe)
// =============================================================================
TEST_F(RuleTest, Cohesion_AttractionVersVoisin) {
    CohesionRule rule;

    // Boid 0 : À l'origine (0,0)
    flock.addBoid(Boid(Vec2<float>(0.0f, 0.0f), Vec2<float>(0.0f, 0.0f)));

    // Boid 1 : En (10, 0)
    flock.addBoid(Boid(Vec2<float>(10.0f, 0.0f), Vec2<float>(0.0f, 0.0f)));

    // Le Boid 0 doit être attiré vers le Boid 1 (centre de masse = 10,0)
    // Force attendue = CentreMasse - MaPosition = (10,0) - (0,0) = (10,0)
    Vec2<float> force = rule.computeForce(flock, 0, settings);

    EXPECT_FLOAT_EQ(force.x, 10.0f);
    EXPECT_FLOAT_EQ(force.y, 0.0f);
}

// =============================================================================
// TEST SÉPARATION (Répulsion si trop proche)
// =============================================================================
TEST_F(RuleTest, Separation_RepulsionSiTropProche) {
    SeparationRule rule;
    settings.dmin = 50.0f; // On augmente dmin pour être sûr d'être dedans

    // Boid 0 : (0,0)
    flock.addBoid(Boid(Vec2<float>(0.0f, 0.0f), Vec2<float>(0.0f, 0.0f)));

    // Boid 1 : (5, 0) -> Très proche !
    flock.addBoid(Boid(Vec2<float>(5.0f, 0.0f), Vec2<float>(0.0f, 0.0f)));

    // Le Boid 0 doit être repoussé vers la GAUCHE (opposé au voisin)
    Vec2<float> force = rule.computeForce(flock, 0, settings);

    // La force doit être négative sur X (vers la gauche)
    EXPECT_LT(force.x, 0.0f);
    EXPECT_FLOAT_EQ(force.y, 0.0f);
}

TEST_F(RuleTest, Separation_PasDeForceSiLoin) {
    SeparationRule rule;
    settings.dmin = 5.0f; // Distance min très petite

    // Boid 0 : (0,0)
    flock.addBoid(Boid(Vec2<float>(0.0f, 0.0f), Vec2<float>(0.0f, 0.0f)));

    // Boid 1 : (10, 0) -> Plus loin que dmin (10 > 5)
    flock.addBoid(Boid(Vec2<float>(10.0f, 0.0f), Vec2<float>(0.0f, 0.0f)));

    // Aucune force de séparation ne doit être générée
    Vec2<float> force = rule.computeForce(flock, 0, settings);

    EXPECT_FLOAT_EQ(force.x, 0.0f);
    EXPECT_FLOAT_EQ(force.y, 0.0f);
}

// =============================================================================
// TEST ALIGNEMENT (Adoption de la vitesse moyenne)
// =============================================================================
TEST_F(RuleTest, Alignement_SuitLeVoisin) {
    AlignmentRule rule;
    settings.r = 100.0f;

    // Boid 0 : Immobile (0,0)
    flock.addBoid(Boid(Vec2<float>(0.0f, 0.0f), Vec2<float>(0.0f, 0.0f)));

    // Boid 1 : File vers le haut à toute vitesse (0, 10)
    flock.addBoid(Boid(Vec2<float>(5.0f, 0.0f), Vec2<float>(0.0f, 10.0f)));

    // Le Boid 0 doit vouloir aller vers le haut aussi
    // Force = VitesseMoyenneVoisins - MaVitesse
    // Force = (0, 10) - (0, 0) = (0, 10)
    Vec2<float> force = rule.computeForce(flock, 0, settings);

    EXPECT_FLOAT_EQ(force.x, 0.0f);
    EXPECT_FLOAT_EQ(force.y, 10.0f);
}