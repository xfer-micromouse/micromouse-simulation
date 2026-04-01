#ifndef MMS_CONTROLLER_ICOMPONENT_H
#define MMS_CONTROLLER_ICOMPONENT_H

#include <utility>

#include "webots/Robot.hpp"

extern "C" {
#include "simavr/sim_avr.h"
}

class IComponent {
protected:
    std::string name;

public:
    explicit IComponent(std::string  name) : name(std::move(name)) {}
    virtual ~IComponent() = default;

    virtual void init(webots::Robot *robot, avr_t *avr) = 0;

    virtual void update_avr() = 0;
    virtual void update_webots() = 0;

    [[nodiscard]] const std::string& get_name() const { return name; }
};

#endif //MMS_CONTROLLER_ICOMPONENT_H