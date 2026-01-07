#ifndef BD_VEC2_H
#define BD_VEC2_H

#include <cmath>
#include <ostream>

namespace bd {

/**
 * @brief Classe template représentant un vecteur mathématique 2D.
 *
 * Cette classe générique est utilisée pour représenter des positions, des vitesses,
 * des forces ou des dimensions. Elle fournit toutes les opérations vectorielles
 * usuelles (addition, soustraction, produit scalaire, normalisation, etc.).
 *
 * @tparam T Le type de données sous-jacent (ex: float, double, int).
 */
template<typename T>
class Vec2 {
public:
    T x; ///< Composante X du vecteur.
    T y; ///< Composante Y du vecteur.

    /**
     * @brief Constructeur par défaut.
     * Initialise le vecteur à l'origine (0, 0).
     */
    Vec2() : x(T(0)), y(T(0)) {}

    /**
     * @brief Constructeur avec paramètres.
     * @param x Valeur de la composante X.
     * @param y Valeur de la composante Y.
     */
    Vec2(T x, T y) : x(x), y(y) {}

    /**
     * @brief Calcule la longueur au carré du vecteur (Magnitude²).
     *
     * Cette méthode est plus performante que length() car elle évite le calcul
     * coûteux de la racine carrée (sqrt). À utiliser pour les comparaisons de distances.
     *
     * @return T La somme des carrés des composantes (x*x + y*y).
     */
    T lengthSquared() const {
        return x * x + y * y;
    }

    /**
     * @brief Calcule la longueur réelle du vecteur (Magnitude).
     * @return T La racine carrée de la longueur au carré.
     */
    T length() const {
        return std::sqrt(lengthSquared());
    }

    /**
     * @brief Retourne une copie normalisée du vecteur.
     *
     * Le vecteur retourné a une direction identique mais une longueur de 1 (vecteur unitaire).
     * Gère le cas du vecteur nul pour éviter la division par zéro.
     *
     * @return Vec2 Le vecteur normalisé.
     */
    Vec2 normalized() const {
        T len = length();
        if (len > T(0)) {
            return Vec2(x / len, y / len);
        }
        return Vec2(0, 0);
    }


    /**
     * @brief Opérateur d'addition vectorielle.
     * @param v Le vecteur à ajouter.
     * @return Vec2 Un nouveau vecteur résultant de (x1+x2, y1+y2).
     */
    Vec2 operator+(const Vec2& v) const {
        return Vec2(x + v.x, y + v.y);
    }

    /**
     * @brief Opérateur de soustraction vectorielle.
     * @param v Le vecteur à soustraire.
     * @return Vec2 Un nouveau vecteur résultant de (x1-x2, y1-y2).
     */
    Vec2 operator-(const Vec2& v) const {
        return Vec2(x - v.x, y - v.y);
    }

    /**
     * @brief Opérateur de multiplication par un scalaire.
     * @param scalar Le nombre par lequel multiplier le vecteur.
     * @return Vec2 Un nouveau vecteur mis à l'échelle.
     */
    Vec2 operator*(T scalar) const {
        return Vec2(x * scalar, y * scalar);
    }

    /**
     * @brief Opérateur de division par un scalaire.
     * @param scalar Le nombre par lequel diviser le vecteur.
     * @return Vec2 Un nouveau vecteur réduit.
     */
    Vec2 operator/(T scalar) const {
        return Vec2(x / scalar, y / scalar);
    }


    /**
     * @brief Opérateur d'addition composée (+=).
     * Ajoute un vecteur à l'instance courante.
     * @param v Le vecteur à ajouter.
     * @return Vec2& Référence vers l'objet courant modifié.
     */
    Vec2& operator+=(const Vec2& v) {
        x += v.x;
        y += v.y;
        return *this;
    }

    /**
     * @brief Opérateur de soustraction composée (-=).
     * Soustrait un vecteur à l'instance courante.
     * @param v Le vecteur à soustraire.
     * @return Vec2& Référence vers l'objet courant modifié.
     */
    Vec2& operator-=(const Vec2& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    /**
     * @brief Opérateur de multiplication composée (*=).
     * Multiplie l'instance courante par un scalaire.
     * @param scalar Le facteur de multiplication.
     * @return Vec2& Référence vers l'objet courant modifié.
     */
    Vec2& operator*=(T scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    /**
     * @brief Opérateur de division composée (/=).
     * Divise l'instance courante par un scalaire.
     * @param scalar Le diviseur.
     * @return Vec2& Référence vers l'objet courant modifié.
     */
    Vec2& operator/=(T scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }



    /**
     * @brief Opérateur d'égalité.
     * @param v Le vecteur à comparer.
     * @return true Si x et y sont identiques.
     */
    bool operator==(const Vec2& v) const {
        return (x == v.x) && (y == v.y);
    }

    /**
     * @brief Opérateur d'inégalité.
     * @param v Le vecteur à comparer.
     * @return true Si x ou y sont différents.
     */
    bool operator!=(const Vec2& v) const {
        return !(*this == v);
    }


    /**
     * @brief Calcule le produit scalaire (Dot Product) de deux vecteurs.
     *
     * Le produit scalaire est utile pour calculer des angles ou des projections.
     * Si le résultat est 0, les vecteurs sont perpendiculaires.
     *
     * @param a Premier vecteur.
     * @param b Second vecteur.
     * @return T Le résultat du produit scalaire.
     */
    static T dot(const Vec2& a, const Vec2& b) {
        return a.x * b.x + a.y * b.y;
    }

    /**
     * @brief Calcule la distance euclidienne entre deux points.
     * @param a Position du premier point.
     * @param b Position du second point.
     * @return T La distance entre a et b.
     */
    static T distance(const Vec2& a, const Vec2& b) {
        return (b - a).length();
    }
};

/**
 * @brief Opérateur de multiplication scalaire à gauche (ex: 2.0f * vec).
 * @tparam T Type numérique.
 * @param scalar Le scalaire.
 * @param v Le vecteur.
 * @return Vec2<T> Le nouveau vecteur résultant.
 */
template<typename T>
Vec2<T> operator*(T scalar, const Vec2<T>& v) {
    return v * scalar;
}

/**
 * @brief Surcharge de l'opérateur de flux pour l'affichage (ex: std::cout << vec).
 * Affiche le vecteur sous la forme "(x, y)".
 * @tparam T Type numérique.
 * @param os Le flux de sortie.
 * @param v Le vecteur à afficher.
 * @return std::ostream& Le flux modifié.
 */
template<typename T>
std::ostream& operator<<(std::ostream& os, const Vec2<T>& v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

} // namespace bd

#endif // BD_VEC2_H