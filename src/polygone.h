#ifndef POLYGONE_H
#define POLYGONE_H

#include <vector>
#include <iostream>
#include "point2d.h"
#include <stdexcept> // Pour lever des exceptions

using namespace std;

template <typename T>
class Polygone {
private:
    std::vector<Point2D<T>> sommets;

public:
    // Constructeurs
    Polygone() = default;

    Polygone(const vector<Point2D<T>>& listeSommets) : sommets(listeSommets) {}

    Polygone(const Polygone<T>& poly) : sommets(poly.sommets) {}

    // Accesseurs
    std::vector<Point2D<T>> getSommets() const {
        return sommets;
    }

    // Mutateurs
    void setSommets(const vector<Point2D<T>>& listeSommets) {
        sommets = listeSommets;
    }

    void addPoint(const Point2D<T>& point) {
        sommets.push_back(point);
    }

    // Translater tous les sommets du polygone
    void translate(T dx, T dy) {
        for (auto& sommet : sommets) {
            sommet.translate(dx, dy);
        }
    }

    // Vérifier le sens trigonométrique du polygone
    bool isCounterClockwise() const {
        if (sommets.size() < 3) {
            throw std::invalid_argument("Un polygone doit avoir au moins 3 sommets.");
        }

        // Trouver le point le plus bas à gauche
        size_t indexO = 0;
        for (size_t i = 1; i < sommets.size(); ++i) {
            if (sommets[i].getY() < sommets[indexO].getY() ||
                (sommets[i].getY() == sommets[indexO].getY() && sommets[i].getX() < sommets[indexO].getX())) {
                indexO = i;
            }
        }

        // Vérifier le sens trigonométrique
        size_t n = sommets.size();
        for (size_t i = 0; i < n; ++i) {
            size_t current = (indexO + i) % n;
            size_t next = (current + 1) % n;
            size_t nextNext = (next + 1) % n;

            T x1 = sommets[next].getX() - sommets[current].getX();
            T y1 = sommets[next].getY() - sommets[current].getY();

            T x2 = sommets[nextNext].getX() - sommets[current].getX();
            T y2 = sommets[nextNext].getY() - sommets[current].getY();

            // Calcul du produit scalaire
            T crossProduct = x1 * y2 - y1 * x2;

            // Si le produit est négatif, les points ne sont pas dans le sens trigonométrique
            if (crossProduct < 0) {
                return false;
            }
        }
        return true;
    }

    // Lever une exception si le polygone n'est pas dans le sens trigonométrique
    void checkCounterClockwise() const {
        if (!isCounterClockwise()) {
            throw std::logic_error("Le polygone n'est pas dans le sens trigonométrique.");
        }
    }

    // Vérifier si deux segments s'intersectent
    bool segmentsIntersect(const Point2D<T>& A, const Point2D<T>& B, const Point2D<T>& C, const Point2D<T>& D) const {
        auto crossProduct = [](const Point2D<T>& P, const Point2D<T>& Q, const Point2D<T>& R) {
            return (Q.getX() - P.getX()) * (R.getY() - P.getY()) -
                   (Q.getY() - P.getY()) * (R.getX() - P.getX());
        };

        T d1 = crossProduct(A, B, C);
        T d2 = crossProduct(A, B, D);
        T d3 = crossProduct(C, D, A);
        T d4 = crossProduct(C, D, B);

        // Vérifier les changements de signe
        return (d1 * d2 < 0) && (d3 * d4 < 0);
    }

    // Vérifier si le polygone est croisé
    bool isSelfIntersecting() const {
        size_t n = sommets.size();
        if (n < 4) return false; // Un polygone avec moins de 4 côtés ne peut pas être croisé

        for (size_t i = 0; i < n; ++i) {
            for (size_t j = i + 2; j < n; ++j) {
                // Éviter les côtés adjacents
                if ((i == 0 && j == n - 1) || (j == i + 1)) continue;

                if (segmentsIntersect(sommets[i], sommets[(i + 1) % n],
                                      sommets[j], sommets[(j + 1) % n])) {
                    return true;
                }
            }
        }
        return false;
    }

    // Lever une exception si le polygone est croisé
    void checkSelfIntersection() const {
        if (isSelfIntersecting()) {
            throw std::logic_error("Le polygone est croisé.");
        }
    }

    // Affichage du polygone
    friend std::ostream& operator<<(std::ostream& os, const Polygone<T>& poly) {
        os << "Polygone : ";
        for (const auto& sommet : poly.sommets) {
            os << sommet << " ";
        }
        return os;
    }
};

#endif // POLYGONE_H
