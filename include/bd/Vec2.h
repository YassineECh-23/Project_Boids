

#ifndef BD_VEC2_H
#define BD_VEC2_H

#include <cmath>
#include <ostream>

namespace bd {

template<typename T>
class Vec2 {
public:
    T x, y;

    // ============================================================
    // CONSTRUCTEURS
    // ============================================================
    Vec2() : x(T(0)), y(T(0)) {}
    Vec2(T x, T y) : x(x), y(y) {}

    // ============================================================
    // MÉTHODES MATHÉMATIQUES
    // ============================================================

    /// Longueur au carré (plus rapide car pas de sqrt)
    T lengthSquared() const {
        return x * x + y * y;
    }

    /// Longueur du vecteur
    T length() const {
        return std::sqrt(lengthSquared());
    }

    /// Retourne un vecteur normalisé (longueur = 1)
    Vec2 normalized() const {
        T len = length();
        if (len > T(0)) {
            return Vec2(x / len, y / len);
        }
        return Vec2(0, 0);
    }

    // ============================================================
    // OPÉRATEURS ARITHMÉTIQUES (obligatoires)
    // ============================================================

    Vec2 operator+(const Vec2& v) const {
        return Vec2(x + v.x, y + v.y);
    }

    Vec2 operator-(const Vec2& v) const {
        return Vec2(x - v.x, y - v.y);
    }

    Vec2 operator*(T scalar) const {
        return Vec2(x * scalar, y * scalar);
    }

    Vec2 operator/(T scalar) const {
        return Vec2(x / scalar, y / scalar);
    }

    // ============================================================
    // OPÉRATEURS COMPOSÉS (obligatoires)
    // ============================================================

    Vec2& operator+=(const Vec2& v) {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vec2& operator-=(const Vec2& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    Vec2& operator*=(T scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vec2& operator/=(T scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    // ============================================================
    // COMPARAISONS (obligatoires)
    // ============================================================

    bool operator==(const Vec2& v) const {
        return (x == v.x) && (y == v.y);
    }

    bool operator!=(const Vec2& v) const {
        return !(*this == v);
    }

    // ============================================================
    // FONCTIONS STATIQUES (utiles pour les boids)
    // ============================================================

    /// Produit scalaire
    static T dot(const Vec2& a, const Vec2& b) {
        return a.x * b.x + a.y * b.y;
    }

    /// Distance entre deux points
    static T distance(const Vec2& a, const Vec2& b) {
        return (b - a).length();
    }
};

// ============================================================
// OPÉRATEURS EXTERNES (obligatoires)
// ============================================================

/// Multiplication scalaire à gauche : 2.0f * v
template<typename T>
Vec2<T> operator*(T scalar, const Vec2<T>& v) {
    return v * scalar;
}

/// Opérateur de flux pour affichage : std::cout << v
template<typename T>
std::ostream& operator<<(std::ostream& os, const Vec2<T>& v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

} // namespace bd

#endif // BD_VEC2_H