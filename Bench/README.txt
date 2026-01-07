# BENCHMARK - TESTS DE PERFORMANCE (Projet Boids)

Ce dossier contient module de Benchmark dédié à l'analyse des performances du moteur physique des Boids.
Il utilise la bibliothèque "Google Benchmark" pour mesurer avec précision le temps d'exécution des algorithmes (Mise à jour, Cohésion, etc.) sans l'interférence de l'interface graphique.

But du module :
---------------
Prouver expérimentalement la complexité algorithmique du projet (O(N^2)) et mesurer la limite du nombre de boids pour maintenir 60 FPS.

Fichiers du Benchmark :
-----------------------
- main_bench.cpp : Le point d'entrée qui définit les scénarios de test (Update global, Règle de cohésion...).
- Makefile       : Script de compilation indépendant pour le benchmark.
- include/bd/    : Copie des en-têtes (.h) du projet principal.
- src/bd/        : Copie des sources (.cpp) du moteur physique (Maths & Règles).

Pré-requis :
------------
La bibliothèque Google Benchmark doit être installée sur le système.

Installation (WSL / Linux) :
    sudo apt update
    sudo apt install libbenchmark-dev

Compilation & Exécution :
-------------------------
Le dossier possède son propre Makefile indépendant.

1. Se placer dans le dossier Bench :
    cd Bench

2. Compiler et lancer le benchmark :
    make run

3. Nettoyer les fichiers générés :
    make clean

Comprendre les résultats :
--------------------------
Une fois lancé, le programme affiche un tableau dans le terminal. Voici comment le lire :

Exemple de sortie :
------------------------------------------------------------------
Benchmark            Time             CPU   Iterations
------------------------------------------------------------------
BM_FlockUpdate/100   0.08 ms         0.08 ms     9754
BM_FlockUpdate/500   1.74 ms         1.74 ms      401
BM_FlockUpdate/1000  6.79 ms         6.79 ms      103

Analyse :
- Time : Temps moyen pour calculer UNE image (frame) de simulation.
- 100/500/1000 : Le nombre de boids testés.

Interprétation (Preuve O(N^2)) :
Si on double le nombre de boids (ex: 500 -> 1000) et que le temps est multiplié par 4 (ex: 1.7ms -> 6.8ms), cela confirme que l'algorithme est quadratique (O(N^2)).

Notes techniques :
------------------
- Les tests sont compilés avec l'option -O3 (optimisation maximale) pour refléter les performances réelles en Release.
- L'affichage graphique (SFML) est désactivé pour ne tester que le calcul pur (CPU).

Auteurs :
---------
Université de Poitiers — Master Big data & Logiciel
Projet C++ Avancé