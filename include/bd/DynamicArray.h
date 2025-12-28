//
// DynamicArray.h - Tableau dynamique maison
// Contraintes du cahier des charges (page 2) :
// - Pas de std::vector, std::list, etc.
// - Structure générique (template)
// - Gestion mémoire manuelle
//

#ifndef BD_DYNAMICARRAY_H
#define BD_DYNAMICARRAY_H

#include <cstddef>      // pour size_t
#include <stdexcept>    // pour exceptions

namespace bd {

template<typename T>
class DynamicArray {
private:
    T* data_;           // Pointeur vers les données
    size_t size_;       // Nombre d'éléments actuels
    size_t capacity_;   // Capacité totale allouée

    // ============================================================
    // RÉALLOCATION INTERNE (double la capacité)
    // ============================================================
    void resize() {
        // Si capacité = 0, initialiser à 4, sinon doubler
        capacity_ = (capacity_ == 0) ? 4 : capacity_ * 2;

        // Allouer nouveau tableau
        T* newData = new T[capacity_];

        // Copier les anciennes données
        for (size_t i = 0; i < size_; ++i) {
            newData[i] = data_[i];
        }

        // Libérer l'ancienne mémoire
        delete[] data_;

        // Mettre à jour le pointeur
        data_ = newData;
    }

public:
    // ============================================================
    // CONSTRUCTEUR & DESTRUCTEUR
    // ============================================================

    DynamicArray()
        : data_(nullptr), size_(0), capacity_(0) {}

    ~DynamicArray() {
        delete[] data_;
    }

    // ============================================================
    // CONSTRUCTEUR DE COPIE (important !)
    // ============================================================

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

    // ============================================================
    // OPÉRATEUR D'AFFECTATION (important !)
    // ============================================================

    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            // Libérer l'ancienne mémoire
            delete[] data_;
            data_ = nullptr;
            size_ = 0;
            capacity_ = 0;

            // Copier les nouvelles données
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

    // ============================================================
    // MÉTHODES PRINCIPALES
    // ============================================================

    /// Ajoute un élément à la fin
    void push_back(const T& value) {
        if (size_ == capacity_) {
            resize();  // Réallouer si nécessaire
        }
        data_[size_++] = value;
    }

    /// Supprime l'élément à l'index donné
    void removeAt(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("DynamicArray::removeAt - index hors limites");
        }

        // Décaler tous les éléments après l'index
        for (size_t i = index; i < size_ - 1; ++i) {
            data_[i] = data_[i + 1];
        }

        --size_;
    }

    /// Vide le tableau
    void clear() {
        size_ = 0;
    }

    // ============================================================
    // ACCESSEURS
    // ============================================================

    /// Accès en lecture/écriture
    T& operator[](size_t index) {
        if (index >= size_) {
            throw std::out_of_range("DynamicArray::operator[] - index hors limites");
        }
        return data_[index];
    }

    /// Accès en lecture seule
    const T& operator[](size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("DynamicArray::operator[] const - index hors limites");
        }
        return data_[index];
    }

    /// Retourne la taille actuelle
    size_t size() const {
        return size_;
    }

    /// Retourne la capacité
    size_t capacity() const {
        return capacity_;
    }

    /// Vérifie si le tableau est vide
    bool empty() const {
        return size_ == 0;
    }
};

} // namespace bd

#endif // BD_DYNAMICARRAY_H