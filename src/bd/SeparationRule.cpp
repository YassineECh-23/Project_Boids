#include "../../include/bd/SeparationRule.h"
#include "../../include/bd/Flock.h"

namespace bd {

    Vec2<float> SeparationRule::computeForce(const Flock& flock, int boidIndex, const Settings& settings) const {
        Vec2<float> force(0.0f, 0.0f);
        int neighborCount = 0;

        const Boid& me = flock.getBoid(boidIndex);

        for (int i = 0; i < flock.size(); ++i) {
            if (i == boidIndex) continue;

            const Boid& other = flock.getBoid(i);
            float dist = Vec2<float>::distance(me.getPosition(), other.getPosition());

            if (dist > 0 && dist < settings.dmin) {
                Vec2<float> diff = me.getPosition() - other.getPosition();
                diff /= (dist * dist); 
                force += diff;
                neighborCount++;
            }
        }

        return force;
    }

} // namespace bd