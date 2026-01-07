#ifndef PT_BOIDS_OBSTACLERULE_H
#define PT_BOIDS_OBSTACLERULE_H

#include "Rule.h"
#include "Settings.h"

namespace bd {

    class ObstacleRule : public Rule {
    public:
        Vec2<float> computeForce(const Flock& flock, int boidIndex, const Settings& settings) const override;
    };

} // namespace bd

#endif //PT_BOIDS_OBSTACLERULE_H