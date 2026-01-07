
#ifndef PT_BOIDS_COHESIONRULE_H
#define PT_BOIDS_COHESIONRULE_H

#include "Rule.h"

namespace bd {

    class CohesionRule : public Rule {
    public:
        Vec2<float> computeForce(const Flock& flock, int boidIndex, const Settings& settings) const override;
    };

} // namespace bd


#endif //PT_BOIDS_COHESIONRULE_H