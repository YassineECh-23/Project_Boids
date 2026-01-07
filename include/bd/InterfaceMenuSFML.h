#ifndef BD_INTERFACE_MENU_SFML_H
#define BD_INTERFACE_MENU_SFML_H

#include <SFML/Graphics.hpp>
#include <string>
#include "DynamicArray.h"
#include "Settings.h"

namespace bd {

    /**
     * @brief Classe gérant l’interface graphique du menu principal avec SFML.
     *
     * Cette classe orchestre la navigation entre les différents écrans de l'application :
     * - Le menu principal (Main)
     * - L’écran de configuration des paramètres (Settings)
     * - L’écran de chargement des sauvegardes (Load)
     *
     * Elle gère les événements (souris, clavier), l'affichage et la logique
     * de transition vers la simulation.
     */
    class InterfaceMenuSFML {
    public:
        /**
         * @brief Constructeur par défaut.
         *
         * Crée la fenêtre SFML, initialise les paramètres par défaut
         * et configure l'interface utilisateur.
         */
        InterfaceMenuSFML();

        /**
         * @brief Lance la boucle principale du menu.
         *
         * Maintient la fenêtre ouverte, traite les événements et rafraîchit
         * l'affichage tant que l'utilisateur ne quitte pas ou ne lance pas la simulation.
         */
        void run();

    private:

        /**
         * @brief Énumération des différents écrans disponibles dans le menu.
         */
        enum class Screen { Main, Settings, Load };

        sf::RenderWindow window;   ///< Fenêtre principale SFML utilisée pour le menu.
        sf::Font font;             ///< Police d'écriture chargée pour les textes.
        bool fontLoaded = false;   ///< Indique si le fichier de police a été chargé avec succès.

        Screen screen = Screen::Main; ///< L'écran actuellement affiché à l'utilisateur.
        Settings settings;            ///< Structure stockant les paramètres de simulation à configurer.

        // --- Boutons du menu principal ---
        sf::RectangleShape btnRun;       ///< Bouton pour lancer la simulation directement.
        sf::RectangleShape btnLoad;      ///< Bouton pour accéder au menu de chargement.
        sf::RectangleShape btnSettings;  ///< Bouton pour accéder aux réglages.
        sf::RectangleShape btnQuit;      ///< Bouton pour quitter l'application.

        sf::Text txtTitle;      ///< Texte du titre principal "BOIDS".
        sf::Text txtRun;        ///< Label du bouton Run.
        sf::Text txtLoad;       ///< Label du bouton Load.
        sf::Text txtSettings;   ///< Label du bouton Settings.
        sf::Text txtQuit;       ///< Label du bouton Quit.

        // --- Boutons de l'écran Settings ---
        sf::RectangleShape btnBack;   ///< Bouton pour retourner au menu principal.
        sf::RectangleShape btnStart;  ///< Bouton pour valider les réglages et lancer la simulation.
        sf::Text txtBack;             ///< Label du bouton Back.
        sf::Text txtStart;            ///< Label du bouton Start.

        // --- Gestion de l’édition des paramètres ---
        int selectedIndex = -1;     ///< Indice du champ de paramètre actuellement en cours d'édition (-1 si aucun).
        std::string inputBuffer;    ///< Tampon stockant la saisie clavier de l'utilisateur pour un champ numérique.
        sf::Text txtHint;           ///< Texte d'aide affiché pour guider l'utilisateur.
        sf::Text txtError;          ///< Texte affichant les erreurs de saisie éventuelles.

        // --- Écran LOAD ---
        DynamicArray<std::string> saveFiles; ///< Liste dynamique des chemins des fichiers de sauvegarde détectés.
        sf::RectangleShape btnRefresh;       ///< Bouton pour rafraîchir la liste des fichiers.
        sf::Text txtRefresh;                 ///< Label du bouton Refresh.

        /**
         * @brief Initialise tous les éléments graphiques.
         *
         * Configure la taille, la couleur, la position et le style des boutons
         * et des textes. Charge également la police d'écriture.
         */
        void initUI();

        /**
         * @brief Met à jour la disposition des éléments graphiques.
         *
         * Recalcule les positions des boutons et des textes en fonction
         * de la taille actuelle de la fenêtre (gestion du redimensionnement).
         */
        void updateLayout();

        /**
         * @brief Gère les événements SFML.
         *
         * Traite les entrées utilisateur : clics souris, frappes clavier,
         * fermeture de fenêtre et redimensionnement.
         */
        void handleEvents();

        /**
         * @brief Met à jour la logique interne de l'interface.
         *
         * Gère le survol des boutons (hover effect) et la logique de sélection
         * des champs de texte.
         */
        void update();

        /**
         * @brief Dessine l’interface complète à l’écran.
         *
         * Efface l'écran et appelle la méthode de rendu spécifique
         * à l'écran actif (Main, Settings ou Load).
         */
        void render();

        /**
         * @brief Affiche les éléments du menu principal.
         */
        void renderMain();

        /**
         * @brief Affiche les éléments de l’écran de configuration.
         */
        void renderSettings();

        /**
         * @brief Affiche les éléments de l’écran de chargement.
         */
        void renderLoad();

        /**
         * @brief Vérifie si un rectangle a été cliqué.
         *
         * @param r     Le rectangle SFML (bouton) à tester.
         * @param mouse La position de la souris (coordonnées monde).
         * @return true Si la souris est à l'intérieur du rectangle, false sinon.
         */
        bool isClicked(const sf::RectangleShape& r, sf::Vector2f mouse) const;

        /**
         * @brief Lance la simulation.
         *
         * Ferme temporairement la fenêtre du menu, instancie la classe Simulation
         * avec les paramètres actuels (et le fichier de sauvegarde si fourni),
         * puis relance l'affichage du menu à la fin de la simulation.
         *
         * @param saveFile Chemin optionnel vers un fichier de sauvegarde à charger.
         */
        void launchSimulation(const std::string& saveFile = "");

        // --- Aides pour l’édition des paramètres ---

        /**
         * @brief Active l'édition d'un champ de paramètre spécifique.
         *
         * Prépare le buffer de saisie avec la valeur actuelle du paramètre.
         *
         * @param index L'indice du champ à sélectionner.
         */
        void selectField(int index);

        /**
         * @brief Valide la saisie utilisateur.
         *
         * Convertit le texte du buffer en nombre (int ou float) et met à jour
         * le paramètre correspondant dans la structure Settings.
         */
        void commitInput();

        /**
         * @brief Gère la touche Retour Arrière (Backspace).
         *
         * Supprime le dernier caractère du buffer de saisie.
         */
        void backspaceInput();

        /**
         * @brief Ajoute un caractère à la saisie en cours.
         *
         * Filtre les caractères pour n'accepter que les chiffres et symboles valides.
         *
         * @param c Le caractère saisi par l'utilisateur.
         */
        void appendChar(char c);

        /**
         * @brief Rafraîchit la liste des sauvegardes.
         *
         * Scanne le dossier des sauvegardes via le SaveManager et met à jour
         * la liste `saveFiles`.
         */
        void refreshSaves();
    };

} // namespace bd

#endif // BD_INTERFACE_MENU_SFML_H