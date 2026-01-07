#ifndef PT_BOIDS_PREDATORRULE_H
#define PT_BOIDS_PREDATORRULE_H


#include "Rule.h"
#include "Settings.h"

namespace bd {

    class PredatorRule : public Rule {
    public:
        Vec2<float> computeForce(const Flock& flock, int boidIndex, const Settings& settings) const override;
    };

} // namespace bd


#endif //PT_BOIDS_PREDATORRULE_H