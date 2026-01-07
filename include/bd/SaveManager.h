#ifndef BD_SAVEMANAGER_H
#define BD_SAVEMANAGER_H

#include <string>
#include <vector>
#include "Settings.h"
#include "Flock.h"

namespace bd {

    class SaveManager {
    public:
        // Sauvegarde avec un nom automatique
        static bool saveAuto(const Settings& settings, const Flock& flock);

        // Charge un fichier spécifique
        static bool load(const std::string& filepath, Settings& settings, Flock& flock);

        // Récupère la liste des fichiers de sauvegarde disponibles
        static DynamicArray<std::string> listSaves();

        // Dossier de sauvegarde
        static const std::string SAVE_FOLDER;
    };

} // namespace bd

#endif // BD_SAVEMANAGER_H