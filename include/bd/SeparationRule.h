//
// Created by Yassine on 29/12/2025.
//

#ifndef PT_BOIDS_SEPARATIONRULE_H
#define PT_BOIDS_SEPARATIONRULE_H

#include "Rule.h"

namespace bd {

    class SeparationRule : public Rule {
    public:
        Vec2<float> computeForce(const Flock& flock, int boidIndex, const Settings& settings) const override;
    };

} // namespace bd

#endif //PT_BOIDS_SEPARATIONRULE_H