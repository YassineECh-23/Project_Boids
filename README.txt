# PROJET BOIDS - Simulation de vol d'oiseaux (C++ / SFML)

Ce projet est une simulation comportementale de "Boids" (oiseaux artificiels) basée sur l'algorithme de Craig Reynolds.
Il implémente les trois règles fondamentales (Cohésion, Séparation, Alignement) ainsi que des extensions (Prédateurs, Obstacles, Sauvegarde).
Le projet inclut également une suite de tests unitaires (GoogleTest), des benchmarks de performance et une documentation technique.

Fichiers principaux :
---------------------
- src/bd/*.cpp       : Code source de la simulation et du moteur
- include/bd/*.h     : En-têtes (Classes Boid, Flock, Rules, SaveManager...)
- tests/*.cpp        : Tests unitaires (GoogleTest)
- Bench/             : Tests de performance (Google Benchmark)
- assets/            : Contient les ressources (polices .ttf) et les sauvegardes
- Documentation/     : Documentation technique générée (HTML)
- Document_et_Annexe/: Rapport de projet (PDF) et Diagrammes UML
- Makefile           : Script de compilation automatisé (Jeu + Tests + Doc)

Pré-requis & Installation (WSL / Linux) :
-----------------------------------------
Le projet nécessite un compilateur C++ (C++17), la bibliothèque graphique SFML, ainsi que Doxygen pour la documentation.

1. Mettre à jour les paquets :
    sudo apt update

2. Installer les dépendances (g++, make, SFML, Doxygen, Graphviz) :
    sudo apt install build-essential g++ make libsfml-dev doxygen graphviz

*Note : GoogleTest est géré localement par le Makefile, aucune installation système requise.*

Compilation :
-------------
Le projet utilise un Makefile complet.

1. Compiler le jeu :
    make

2. Compiler les tests unitaires :
    make tests

3. Générer la documentation technique (Doxygen) :
    doxygen Doxyfile

Utilisation :
-------------
Pour lancer la simulation :
    make run
    # ou : ./bin/boids

Pour lancer les tests unitaires :
    make run_tests
    # ou : ./bin/run_tests

Rapport, UML et Documentation :
-------------------------------
Les documents relatifs au projet sont organisés comme suit :

1. **Rapport et UML** :
   Le rapport complet du projet ainsi que les diagrammes UML (Conception initiale et finale) se trouvent dans le dossier :
   `Document_et_Annexe/`

2. **Documentation Technique (Code)** :
   La documentation du code générée par Doxygen se trouve dans le dossier :
   `Documentation/`

   Pour consulter le site web de la documentation, ouvrez le fichier :
   `Documentation/html/index.html`

   Commandes pour l'ouvrir depuis WSL :
   # Ouvre avec l'explorateur Windows par défaut
   explorer.exe Documentation/html/index.html
    
   # OU pour ouvrir spécifiquement avec Google Chrome :
   cmd.exe /C start chrome Documentation/html/index.html

Commandes en jeu :
------------------
Une fois la simulation lancée, voici les contrôles disponibles :

[ ESPACE ]      : Réinitialiser les positions (Reset)
[ S ]           : Sauvegarde rapide (assets/saves/savegame.txt)
[ B ]           : Activer / Désactiver le rebond (Bounce)
[ P ]           : Activer / Désactiver le Prédateur
[ Clic Droit ]  : Placer un Obstacle (Cercle bleu)
[ ECHAP ]       : Quitter la simulation

Menu Principal :
----------------
- RUN      : Lancer la simulation
- LOAD     : Charger la dernière sauvegarde
- SETTINGS : Modifier les paramètres (Nombre, Vitesse, Poids...)
- QUIT     : Quitter

Structure du projet :
---------------------
.
├── Makefile           # Script de build
├── Doxyfile           # Config Doxygen
├── README.md          # Ce fichier
├── bin/               # Exécutables // fait avec le Maakefile 
├── build/             # Fichiers temporaires (.o) // fait avec le Maakefile 
├── lib/               # Librairies (GoogleTest)
├── assets/            # Ressources (Font, Saves)
├── include/           # Fichiers .h (Headers)
├── src/               # Fichiers .cpp (Sources)
├── tests/             # Tests unitaires
├── Bench/             # Tests de performance
├── Document_et_Annexe/# Rapport PDF et UML
└── Documentation/     # Documentation générée (HTML)

Auteurs :
---------
Université de Poitiers — Master Big data & Logiciel
Projet C++ Avancé