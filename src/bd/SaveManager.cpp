#include "../../include/bd/SaveManager.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace fs = std::filesystem;

namespace bd {

    const std::string SaveManager::SAVE_FOLDER = "assets/saves/";

    /**
     * Sauvegarde automatiquement la configuration (Settings) dans un fichier texte.
     * - Crée le dossier de sauvegarde s'il n'existe pas
     * - Génère un nom de fichier basé sur la date/heure actuelle
     * - Écrit uniquement les paramètres (pas les positions/vitesses des boids)
     *
     * @param settings Paramètres de la simulation à sauvegarder
     * @param flock    Flock courant (non utilisé ici car on ne sauvegarde pas les boids)
     * @return         true si la sauvegarde a réussi, sinon false
     */
    bool SaveManager::saveAuto(const Settings& settings, const Flock& flock) {
        // 1. Création du dossier
        if (!fs::exists(SAVE_FOLDER)) {
            fs::create_directories(SAVE_FOLDER);
        }

        // 2. Nom du fichier
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << SAVE_FOLDER << "Settings_"
            << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S")
            << ".txt";

        std::string filename = oss.str();
        std::ofstream file(filename);

        if (!file.is_open()) {
            std::cerr << "[SaveManager] Erreur creation : " << filename << std::endl;
            return false;
        }

        // 3. ÉCRITURE DES SETTINGS UNIQUEMENT (Pas de positions de boids)
        file << "SETTINGS\n";
        file << "nbBoids " << settings.nbBoids << "\n";
        file << "width " << settings.windowWidth << "\n";
        file << "height " << settings.windowHeight << "\n";
        file << "r " << settings.r << "\n";
        file << "dmin " << settings.dmin << "\n";
        file << "wcoh " << settings.wcoh << "\n";
        file << "wsep " << settings.wsep << "\n";
        file << "wali " << settings.wali << "\n";
        file << "bounce " << (settings.enableBounce ? 1 : 0) << "\n";

        // Extensions
        file << "obstacle " << (settings.enableObstacle ? 1 : 0) << "\n";
        file << "predator " << (settings.enablePredator ? 1 : 0) << "\n";

        std::cout << " Configuration sauvegardee : " << filename << std::endl;
        return true;
    }

    /**
     * Charge une configuration (Settings) depuis un fichier texte.
     * - Ouvre le fichier de sauvegarde
     * - Lit des couples clé/valeur et met à jour les champs correspondants
     * - Ne reconstruit pas les positions des boids (le flock n'est pas modifié ici)
     *
     * @param filepath Chemin du fichier à charger
     * @param settings Référence vers les paramètres à remplir
     * @param flock    Flock courant (non utilisé ici pour les mêmes raisons que saveAuto)
     * @return         true si le chargement a réussi, sinon false
     */
    bool SaveManager::load(const std::string& filepath, Settings& settings, Flock& flock) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << " Sauveqarde introuvable : " << filepath << std::endl;
            return false;
        }

        std::string line;

        while (file >> line) {
            // Lecture simple clé/valeur
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

        std::cout << "[SaveManager] Configuration chargee : " << filepath << std::endl;
        return true;
    }

    /**
     * Liste tous les fichiers de sauvegarde disponibles dans le dossier SAVE_FOLDER.
     * Retourne uniquement les fichiers avec l'extension ".txt".
     * @return DynamicArray<std::string> Liste des chemins de fichiers de sauvegarde
     */
    DynamicArray<std::string> SaveManager::listSaves() {
        DynamicArray<std::string> files;
        if (!fs::exists(SAVE_FOLDER)) return files;
        for (const auto& entry : fs::directory_iterator(SAVE_FOLDER)) {
            if (entry.path().extension() == ".txt") {
                files.push_back(entry.path().string());
            }
        }
        return files;
    }

} // namespace bd
