#ifndef BD_DYNAMICARRAY_H
#define BD_DYNAMICARRAY_H

#include <stdexcept>

namespace bd {

/**
 * @brief Classe template gérant un tableau dynamique générique.
 *
 * Cette classe imite le comportement de std::vector. Elle gère automatiquement
 * l'allocation et la réallocation de mémoire lorsque des éléments sont ajoutés.
 * Elle fournit un accès sécurisé aux éléments avec vérification des bornes.
 *
 * @tparam T Le type de données stockées dans le tableau.
 */
template<typename T>
class DynamicArray {
private:
    T* data_;
    size_t size_;
    size_t capacity_;

    /**
     * @brief Redimensionne le tableau dynamique lorsque la capacité est atteinte.
     *
     * Double la capacité actuelle (ou passe à 4 si vide), alloue un nouveau
     * bloc mémoire, copie les éléments existants et libère l'ancien bloc.
     */
    void resize() {
        capacity_ = (capacity_ == 0) ? 4 : capacity_ * 2;
        T* newData = new T[capacity_];
        for (size_t i = 0; i < size_; ++i) {
            newData[i] = data_[i];
        }
        delete[] data_;
        data_ = newData;
    }

public:

    public:

    /**
     * @brief Constructeur par défaut.
     *
     * Initialise un tableau vide (taille 0, capacité 0, pointeur nul).
     * Aucune mémoire n'est allouée à la construction.
     */
    DynamicArray()
        : data_(nullptr), size_(0), capacity_(0) {}

    /**
     * @brief Destructeur.
     *
     * Libère la mémoire allouée pour le tableau dynamique.
     */
    ~DynamicArray() {
        delete[] data_;
    }

    /**
     * @brief Constructeur de copie.
     *
     * Crée une copie profonde (deep copy) d'un autre DynamicArray.
     * Une nouvelle zone mémoire est allouée et les éléments sont copiés un par un.
     *
     * @param other Le tableau à copier.
     */
    DynamicArray(const DynamicArray& other)
        : data_(nullptr), size_(0), capacity_(0) {

        if (other.size_ > 0) {
            capacity_ = other.capacity_;
            size_ = other.size_;
            data_ = new T[capacity_];

            for (size_t i = 0; i < size_; ++i) {
                data_[i] = other.data_[i];
            }
        }
    }

    /**
     * @brief Opérateur d'affectation.
     *
     * Remplace le contenu courant par une copie profonde d'un autre tableau.
     * Gère correctement l'auto-affectation (this == &other).
     * Libère la mémoire existante avant d'allouer la nouvelle.
     *
     * @param other Le tableau source à copier.
     * @return DynamicArray& Une référence vers l'objet courant modifié.
     */
    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] data_;
            data_ = nullptr;
            size_ = 0;
            capacity_ = 0;

            if (other.size_ > 0) {
                capacity_ = other.capacity_;
                size_ = other.size_;
                data_ = new T[capacity_];

                for (size_t i = 0; i < size_; ++i) {
                    data_[i] = other.data_[i];
                }
            }
        }
        return *this;
    }

    /**
     * @brief Ajoute un élément à la fin du tableau.
     *
     * Si la capacité actuelle est insuffisante, une réallocation (resize) est déclenchée.
     *
     * @param value L'élément à ajouter.
     */
    void push_back(const T& value) {
        if (size_ == capacity_) {
            resize();
        }
        data_[size_++] = value;
    }

    /**
     * @brief Supprime l'élément situé à l'index donné.
     *
     * Décale tous les éléments suivants vers la gauche pour combler le vide.
     * La taille (size) est décrémentée de 1.
     *
     * @param index La position de l'élément à supprimer.
     * @throws std::out_of_range Si l'index est invalide (>= size).
     */
    void removeAt(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("DynamicArray::removeAt - index hors limites");
        }
        for (size_t i = index; i < size_ - 1; ++i) {
            data_[i] = data_[i + 1];
        }
        --size_;
    }

    /**
     * @brief Vide le tableau logiquement.
     *
     * La taille est remise à 0, mais la mémoire allouée (capacité) est conservée
     * pour éviter des réallocations futures inutiles.
     */
    void clear() {
        size_ = 0;
    }

    /**
     * @brief Accès à un élément du tableau (lecture/écriture).
     *
     * Permet de modifier l'élément à l'index spécifié.
     *
     * @param index L'indice de l'élément.
     * @return T& Une référence vers l'élément.
     * @throws std::out_of_range Si l'index est invalide (>= size).
     */
    T& operator[](size_t index) {
        if (index >= size_) {
            throw std::out_of_range("DynamicArray::operator[] - index hors limites");
        }
        return data_[index];
    }

    /**
     * @brief Accès à un élément du tableau (lecture seule).
     *
     * @param index L'indice de l'élément.
     * @return const T& Une référence constante vers l'élément.
     * @throws std::out_of_range Si l'index est invalide (>= size).
     */
    const T& operator[](size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("DynamicArray::operator[] const - index hors limites");
        }
        return data_[index];
    }

    /**
     * @brief Retourne le nombre d'éléments présents dans le tableau.
     * @return size_t Le nombre d'éléments.
     */
    size_t size() const {
        return size_;
    }

    /**
     * @brief Retourne la capacité actuelle du tableau (mémoire allouée).
     * @return size_t Le nombre d'éléments que le tableau peut contenir sans réallocation.
     */
    size_t capacity() const {
        return capacity_;
    }

    /**
     * @brief Vérifie si le tableau est vide.
     * @return true Si le tableau ne contient aucun élément (size == 0).
     * @return false Sinon.
     */
    bool empty() const {
        return size_ == 0;
    }
};

} // namespace bd

#endif // BD_DYNAMICARRAY_H