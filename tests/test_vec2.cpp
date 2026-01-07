#include <gtest/gtest.h>
#include "../include/bd/Vec2.h" // Vérifie que le chemin est bon par rapport à ton dossier tests

using namespace bd;

// Test du constructeur et des valeurs par défaut
TEST(Vec2Test, Constructeur) {
    Vec2<float> v1;
    EXPECT_EQ(v1.x, 0.0f);
    EXPECT_EQ(v1.y, 0.0f);

    Vec2<float> v2(3.0f, 4.0f);
    EXPECT_EQ(v2.x, 3.0f);
    EXPECT_EQ(v2.y, 4.0f);
}

// Test de la longueur (Pythagore)
TEST(Vec2Test, Longueur) {
    Vec2<float> v(3.0f, 4.0f);
    // EXPECT_FLOAT_EQ gère automatiquement les petites erreurs d'arrondi !
    EXPECT_FLOAT_EQ(v.length(), 5.0f);
    EXPECT_FLOAT_EQ(v.lengthSquared(), 25.0f);
}

// Test de la normalisation
TEST(Vec2Test, Normalisation) {
    Vec2<float> v(10.0f, 0.0f);
    Vec2<float> n = v.normalized();

    EXPECT_FLOAT_EQ(n.x, 1.0f);
    EXPECT_FLOAT_EQ(n.y, 0.0f);
    EXPECT_FLOAT_EQ(n.length(), 1.0f);

    // Test vecteur nul (sécurité)
    Vec2<float> zero(0.0f, 0.0f);
    Vec2<float> nZero = zero.normalized();
    EXPECT_EQ(nZero.x, 0.0f);
}

// Test des opérateurs arithmétiques (+, -, *)
TEST(Vec2Test, OperateursArithmetiques) {
    Vec2<float> a(1.0f, 2.0f);
    Vec2<float> b(3.0f, 4.0f);

    // Addition
    Vec2<float> c = a + b;
    // Grâce à ton opérateur == et <<, GTest peut comparer les objets directement !
    EXPECT_EQ(c, Vec2<float>(4.0f, 6.0f));

    // Soustraction
    EXPECT_EQ(b - a, Vec2<float>(2.0f, 2.0f));

    // Multiplication scalaire
    EXPECT_EQ(a * 2.0f, Vec2<float>(2.0f, 4.0f));
}

// Test des opérateurs composés (+=, -=)
TEST(Vec2Test, OperateursComposes) {
    Vec2<float> v(1.0f, 2.0f);
    v += Vec2<float>(1.0f, 1.0f);
    EXPECT_EQ(v, Vec2<float>(2.0f, 3.0f));

    v *= 2.0f;
    EXPECT_EQ(v, Vec2<float>(4.0f, 6.0f));
}

// Test du produit scalaire (Static)
TEST(Vec2Test, DotProduct) {
    Vec2<float> right(1.0f, 0.0f);
    Vec2<float> up(0.0f, 1.0f);

    // Vecteurs perpendiculaires = 0
    EXPECT_FLOAT_EQ(Vec2<float>::dot(right, up), 0.0f);

    // Vecteurs parallèles = 1 * 1 = 1
    EXPECT_FLOAT_EQ(Vec2<float>::dot(right, right), 1.0f);
}

// Test de la distance
TEST(Vec2Test, Distance) {
    Vec2<float> p1(0.0f, 0.0f);
    Vec2<float> p2(0.0f, 10.0f);

    EXPECT_FLOAT_EQ(Vec2<float>::distance(p1, p2), 10.0f);
}