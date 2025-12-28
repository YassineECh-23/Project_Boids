#include <gtest/gtest.h>
#include "../include/bd/DynamicArray.h"
#include "../include/bd/Vec2.h" // On va tester avec ta classe Vec2 aussi !

using namespace bd;

// =============================================================================
// TEST AVEC DES ENTIERS (int)
// =============================================================================

TEST(DynamicArrayTest, ConstructeurVide) {
    DynamicArray<int> arr;
    EXPECT_EQ(arr.size(), 0);
    EXPECT_EQ(arr.capacity(), 0);
    EXPECT_TRUE(arr.empty());
}

TEST(DynamicArrayTest, PushBack_Et_Redimensionnement) {
    DynamicArray<int> arr;

    // 1. Ajout du premier élément -> Capacité passe à 4
    arr.push_back(10);
    EXPECT_EQ(arr.size(), 1);
    EXPECT_GE(arr.capacity(), 4); // GE = Greater or Equal
    EXPECT_EQ(arr[0], 10);

    // 2. Remplissage jusqu'à la limite
    arr.push_back(20);
    arr.push_back(30);
    arr.push_back(40);
    EXPECT_EQ(arr.size(), 4);
    EXPECT_EQ(arr.capacity(), 4);

    // 3. Déclenchement du resize (doublement)
    arr.push_back(50);
    EXPECT_EQ(arr.size(), 5);
    EXPECT_EQ(arr.capacity(), 8); // 4 * 2 = 8
    EXPECT_EQ(arr[4], 50);
}

TEST(DynamicArrayTest, AccesElements_Et_Exceptions) {
    DynamicArray<int> arr;
    arr.push_back(5);
    arr.push_back(10);

    // Lecture normale
    EXPECT_EQ(arr[0], 5);
    EXPECT_EQ(arr[1], 10);

    // Modification
    arr[0] = 99;
    EXPECT_EQ(arr[0], 99);

    // Test des limites (Doit lancer une exception)
    EXPECT_THROW(arr[2], std::out_of_range); // Index égal à size
    EXPECT_THROW(arr[100], std::out_of_range); // Index loin
}

TEST(DynamicArrayTest, RemoveAt) {
    DynamicArray<int> arr;
    arr.push_back(10); // 0
    arr.push_back(20); // 1
    arr.push_back(30); // 2
    arr.push_back(40); // 3

    // Suppression de l'élément à l'index 1 (le 20)
    arr.removeAt(1);

    // Vérifications
    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 30); // Le 30 a pris la place
    EXPECT_EQ(arr[2], 40);

    // Test exception suppression hors limites
    EXPECT_THROW(arr.removeAt(10), std::out_of_range);
}

TEST(DynamicArrayTest, CopieProfonde) {
    DynamicArray<int> arr1;
    arr1.push_back(1);
    arr1.push_back(2);

    // Constructeur de copie
    DynamicArray<int> arr2 = arr1;

    // Modifier arr2 ne doit pas toucher arr1
    arr2[0] = 999;

    EXPECT_EQ(arr1[0], 1);   // Intact
    EXPECT_EQ(arr2[0], 999); // Modifié
}

// =============================================================================
// TEST AVEC TON VEC2 (Test d'objets complexes)
// =============================================================================

TEST(DynamicArrayTest, FonctionneAvecVec2) {
    DynamicArray<Vec2<float>> boids;

    boids.push_back(Vec2<float>(1.0f, 2.0f));
    boids.push_back(Vec2<float>(3.0f, 4.0f));

    EXPECT_EQ(boids.size(), 2);

    // Vérification des valeurs
    EXPECT_FLOAT_EQ(boids[0].x, 1.0f);
    EXPECT_FLOAT_EQ(boids[1].y, 4.0f);

    // Test de la somme de vecteurs stockés
    Vec2<float> sum = boids[0] + boids[1];
    EXPECT_FLOAT_EQ(sum.x, 4.0f);
}