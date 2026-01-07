#include <benchmark/benchmark.h>

// Nos fichiers d'en-tête
#include "Flock.h"
#include "Settings.h"
#include "CohesionRule.h"
#include "SeparationRule.h"
#include "AlignmentRule.h"
#include "DynamicArray.h" 

// ----------------------------------------------------------------------------
// Benchmark 1 : Simulation complète (Update global)
// Permet de voir l'impact du nombre de boids sur les FPS
// ----------------------------------------------------------------------------
static void BM_FlockUpdate(benchmark::State& state) {
    bd::Settings settings;
    bd::Flock flock;

    // Récupération du paramètre N (défini par Range plus bas)
    int nbBoids = state.range(0);
    
    // Setup d'une population aléatoire pour le test
    flock.initRandom(nbBoids, 1920.0f, 1080.0f, settings);

    // On utilise notre DynamicArray pour stocker les règles actives
    bd::DynamicArray<bd::Rule*> rules;
    
    // Ajout des 3 règles de base
    rules.push_back(new bd::CohesionRule());
    rules.push_back(new bd::SeparationRule());
    rules.push_back(new bd::AlignmentRule());

    // Boucle principale mesurée par la lib
    for (auto _ : state) {
        // Simulation d'une frame (dt = 0.016s pour viser 60fps)
        flock.updateAll(rules, settings, 0.016f);
    }

    // Nettoyage des pointeurs pour éviter les fuites de mémoire
    for (size_t i = 0; i < rules.size(); ++i) {
        delete rules[i];
    }
    
    // Ajout d'une métrique personnalisée pour l'affichage
    state.SetItemsProcessed(state.iterations() * nbBoids);
}
// Tests avec 100, ..., 1000 boids. Résultat en millisecondes.
BENCHMARK(BM_FlockUpdate)->Range(100, 1000)->Unit(benchmark::kMillisecond);


// ----------------------------------------------------------------------------
// Benchmark 2 : Coût de la règle de Cohésion seule
// ----------------------------------------------------------------------------
static void BM_CohesionRule(benchmark::State& state) {
    bd::Settings settings;
    bd::Flock flock;
    
    int nbBoids = state.range(0);
    flock.initRandom(nbBoids, 1920.0f, 1080.0f, settings);
    
    bd::CohesionRule rule;

    for (auto _ : state) {
        // On force le calcul sur le premier boid pour mesurer la recherche de voisins
        rule.computeForce(flock, 0, settings);
    }
}
// Ici on mesure en microsecondes car c'est un calcul unitaire
BENCHMARK(BM_CohesionRule)->Range(100, 2000)->Unit(benchmark::kMicrosecond);

// Point d'entrée généré par Google Benchmark
BENCHMARK_MAIN();