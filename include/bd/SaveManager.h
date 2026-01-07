#ifndef BD_SAVEMANAGER_H
#define BD_SAVEMANAGER_H

#include <string>
#include <vector>
#include "Settings.h"
#include "Flock.h"

namespace bd {

    /**
     * @brief Gestionnaire de persistance (Sauvegarde et Chargement).
     *
     * Cette classe utilitaire (méthodes statiques) permet de :
     * - Sauvegarder l'état actuel de la simulation (paramètres + boids) dans un fichier.
     * - Charger une simulation depuis un fichier existant.
     * - Lister les sauvegardes disponibles sur le disque.
     */
    class SaveManager {
    public:
        /**
         * @brief Sauvegarde l'état actuel avec un nom généré automatiquement.
         *
         * Crée un fichier (généralement horodaté ou numéroté) dans le dossier de sauvegarde
         * contenant toutes les données nécessaires pour restaurer la simulation.
         *
         * @param settings Les paramètres de configuration actuels.
         * @param flock    L'état actuel du groupe de boids (positions, vitesses).
         * @return true Si la sauvegarde a réussi.
         * @return false En cas d'erreur d'écriture fichier.
         */
        static bool saveAuto(const Settings& settings, const Flock& flock);

        /**
         * @brief Charge une simulation depuis un fichier spécifique.
         *
         * Lit le fichier, parse les données et remplace l'état actuel des objets
         * settings et flock passés en référence.
         *
         * @param filepath Chemin complet du fichier à charger.
         * @param settings Objet Settings à mettre à jour avec les valeurs du fichier.
         * @param flock    Objet Flock à repeupler avec les boids du fichier.
         * @return true Si le chargement a réussi.
         * @return false Si le fichier n'existe pas ou est corrompu.
         */
        static bool load(const std::string& filepath, Settings& settings, Flock& flock);

        /**
         * @brief Récupère la liste des fichiers de sauvegarde disponibles.
         *
         * Scanne le répertoire défini par SAVE_FOLDER.
         *
         * @return DynamicArray<std::string> Une liste contenant les noms des fichiers trouvés.
         */
        static DynamicArray<std::string> listSaves();

        /**
         * @brief Chemin relatif vers le dossier de stockage des sauvegardes.
         * Par exemple "assets/saves/".
         */
        static const std::string SAVE_FOLDER;
    };

} // namespace bd

#endif // BD_SAVEMANAGER_H