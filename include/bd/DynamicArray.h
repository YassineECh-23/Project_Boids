#ifndef BD_DYNAMICARRAY_H
#define BD_DYNAMICARRAY_H

#include <stdexcept>

namespace bd {

template<typename T>
class DynamicArray {
private:
    T* data_;
    size_t size_;
    size_t capacity_;

    /**
     *  Redimensionne le tableau dynamique lorsque la capacité est atteinte.
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
     * Constructeur par défaut.
     * Initialise un tableau vide sans allocation mémoire.
     */
    DynamicArray()
        : data_(nullptr), size_(0), capacity_(0) {}

    /**
     * Destructeur.
     * Libère la mémoire allouée pour le tableau dynamique.
     */
    ~DynamicArray() {
        delete[] data_;
    }

    /**
     * Constructeur de copie.
     * Crée une copie profonde d'un autre DynamicArray.
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
     * Opérateur d'affectation.
     * Remplace le contenu courant par une copie profonde d'un autre tableau.
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
     * Ajoute un élément à la fin du tableau.
     * Redimensionne le tableau si la capacité est atteinte.
     */
    void push_back(const T& value) {
        if (size_ == capacity_) {
            resize();
        }
        data_[size_++] = value;
    }

    /**
     * Supprime l'élément situé à l'index donné.
     * Décale les éléments suivants vers la gauche.
     * Lève une exception si l'index est invalide.
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
     * Vide le tableau sans libérer la mémoire allouée.
     */
    void clear() {
        size_ = 0;
    }

    /**
     * Accès à un élément du tableau (modifiable).
     * Lève une exception si l'index est hors limites.
     */
    T& operator[](size_t index) {
        if (index >= size_) {
            throw std::out_of_range("DynamicArray::operator[] - index hors limites");
        }
        return data_[index];
    }

    /**
     * Accès à un élément du tableau (lecture seule).
     * Lève une exception si l'index est hors limites.
     */
    const T& operator[](size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("DynamicArray::operator[] const - index hors limites");
        }
        return data_[index];
    }

    /**
     * Retourne le nombre d'éléments présents dans le tableau.
     */
    size_t size() const {
        return size_;
    }

    /**
     * Retourne la capacité actuelle du tableau.
     */
    size_t capacity() const {
        return capacity_;
    }

    /**
     * Indique si le tableau est vide.
     */
    bool empty() const {
        return size_ == 0;
    }
};

} // namespace bd

#endif // BD_DYNAMICARRAY_H