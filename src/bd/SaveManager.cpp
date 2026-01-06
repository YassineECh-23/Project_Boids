#include "../../include/bd/SaveManager.h"
#include <fstream>
#include <iostream>
#include <filesystem> // C++17 (Standard moderne)
#include <ctime>
#include <iomanip>
#include <sstream>

namespace fs = std::filesystem;

namespace bd {

    const std::string SaveManager::SAVE_FOLDER = "../assets/saves/";

    bool SaveManager::saveAuto(const Settings& settings, const Flock& flock) {
        // 1. Créer le dossier s'il n'existe pas
        if (!fs::exists(SAVE_FOLDER)) {
            fs::create_directories(SAVE_FOLDER);
        }

        // 2. Générer le nom : save_YYYY-MM-DD_HH-MM-SS.save
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << SAVE_FOLDER << "save_"
            << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S")
            << ".txt"; // Extension .txt pour lisibilité

        std::string filename = oss.str();

        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "[SaveManager] Erreur creation fichier : " << filename << std::endl;
            return false;
        }

        // 3. Écriture (Même format qu'avant)
        file << "SETTINGS\n";
        file << "nbBoids " << flock.size() << "\n";
        file << "width " << settings.windowWidth << "\n";
        file << "height " << settings.windowHeight << "\n";
        file << "r " << settings.r << "\n";
        file << "dmin " << settings.dmin << "\n";
        file << "wcoh " << settings.wcoh << "\n";
        file << "wsep " << settings.wsep << "\n";
        file << "wali " << settings.wali << "\n";
        file << "bounce " << (settings.enableBounce ? 1 : 0) << "\n";
        file << "obstacle " << (settings.enableObstacle ? 1 : 0) << "\n";
        file << "predator " << (settings.enablePredator ? 1 : 0) << "\n";

        file << "BOIDS\n";
        for (int i = 0; i < flock.size(); ++i) {
            const Boid& b = flock.getBoid(i);
            file << b.getPosition().x << " " << b.getPosition().y << " "
                 << b.getVelocity().x << " " << b.getVelocity().y << "\n";
        }

        std::cout << "[SaveManager] Sauvegarde reussie : " << filename << std::endl;
        return true;
    }

    bool SaveManager::load(const std::string& filepath, Settings& settings, Flock& flock) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "[SaveManager] Fichier introuvable : " << filepath << std::endl;
            return false;
        }

        std::string line, mode = "";
        flock.clear();

        while (file >> line) {
            if (line == "SETTINGS") { mode = "SETTINGS"; continue; }
            if (line == "BOIDS") { mode = "BOIDS"; continue; }

            if (mode == "SETTINGS") {
                if (line == "nbBoids") file >> settings.nbBoids;
                else if (line == "width") file >> settings.windowWidth;
                else if (line == "height") file >> settings.windowHeight;
                else if (line == "r") file >> settings.r;
                else if (line == "dmin") file >> settings.dmin;
                else if (line == "wcoh") file >> settings.wcoh;
                else if (line == "wsep") file >> settings.wsep;
                else if (line == "wali") file >> settings.wali;
                else if (line == "bounce") { int v; file >> v; settings.enableBounce = (v==1); }
                else if (line == "obstacle") { int v; file >> v; settings.enableObstacle = (v==1); }
                else if (line == "predator") { int v; file >> v; settings.enablePredator = (v==1); }
            }
            else if (mode == "BOIDS") {
                float px, py, vx, vy;
                try {
                    px = std::stof(line);
                    file >> py >> vx >> vy;
                    flock.addBoid(Boid(Vec2<float>(px, py), Vec2<float>(vx, vy)));
                } catch (...) { continue; }
            }
        }
        settings.nbBoids = flock.size();
        std::cout << "[SaveManager] Chargement OK : " << filepath << std::endl;
        return true;
    }

    std::vector<std::string> SaveManager::listSaves() {
        std::vector<std::string> files;
        if (!fs::exists(SAVE_FOLDER)) return files;

        for (const auto& entry : fs::directory_iterator(SAVE_FOLDER)) {
            if (entry.path().extension() == ".txt" || entry.path().extension() == ".save") {
                // On garde le chemin complet pour le chargement,
                // mais on pourrait stocker juste le nom pour l'affichage.
                // Ici je stocke le chemin relatif "saves/fichier.txt"
                files.push_back(entry.path().string());
            }
        }
        return files;
    }

} // namespace bd