#ifndef MMS_CONTROLLER_MOTORCOMPONENT_HPP
#define MMS_CONTROLLER_MOTORCOMPONENT_HPP

#include "IComponent.hpp"
#include "webots/Motor.hpp"

extern "C" {
#include "simavr/avr_ioport.h"
}

class MotorComponent : public IComponent {
public:
    MotorComponent(const std::string &name, char port, uint8_t pin, bool velocity_control = false)
        : IComponent(name), avr_(nullptr), motor_(nullptr), port_(port), pin_(pin), velocity_control_(velocity_control) {
    }

    void init(webots::Robot *r, avr_t *a) override {
        avr_ = a;
        motor_ = r->getMotor(name);
        if (motor_) {
            printf("Init Motor: %s on pin %u\n", name.c_str(), pin_);
        } else {
            fprintf(stderr, "Warning: Webots device '%s' not found!\n", name.c_str());
        }

        avr_irq_t *avr_pin = avr_io_getirq(avr_, AVR_IOCTL_IOPORT_GETIRQ(port_), pin_);
        avr_irq_register_notify(avr_pin, avr_pin_change_callback, this);

        if (velocity_control_) {
            motor_->setPosition(INFINITY); // Set to velocity control mode
        }
    }

    void update_webots() override {
        if (!motor_) return;

        if (velocity_control_) {
                // In velocity control mode, we interpret the duty cycle as a percentage of max speed
                double duty_cycle = getDutyCycle();
                double max_speed = motor_->getMaxVelocity();
                motor_->setVelocity(duty_cycle * max_speed);
                return;
        }

        double duty_cycle = getDutyCycle();
        double max_torque = motor_->getMaxTorque();
        motor_->setTorque(duty_cycle * max_torque);
    }

    void update_avr() override {
    }

private:
    avr_t *avr_;
    webots::Motor *motor_;
    const char port_;
    const uint8_t pin_;

    bool velocity_control_;

    uint64_t last_transition_time = 0;
    uint64_t last_rising_edge_time = 0;

    uint64_t last_high_duration = 0;
    uint64_t current_period = 0;

    uint32_t current_state = 0;
    bool has_synced = false;

    static constexpr uint64_t TIMEOUT_THRESHOLD = 50000;

    static void avr_pin_change_callback(struct avr_irq_t *irq, uint32_t value, void *param) {
        auto *self = static_cast<MotorComponent *>(param);
        self->pin_change_callback(value);
    }

    void pin_change_callback(const uint32_t value) {
        std::cout << "Pin change detected on " << name << ": new value = " << value << std::endl;
        // simavr IRQ values can sometimes be flags/masks, so clamp to 0 or 1
        uint32_t new_state = (value > 0) ? 1 : 0;

        // Ignore redundant triggers (defensive programming)
        if (new_state == current_state) return;

        uint64_t now = avr_->cycle;

        if (new_state == 1) {
            // --- Rising Edge ---
            // A full period has completed (Rising edge to Rising edge)
            if (last_rising_edge_time != 0) {
                current_period = now - last_rising_edge_time;
                has_synced = true; // We now have a valid period and high duration
            }
            last_rising_edge_time = now;
        } else {
            // --- Falling Edge ---
            // The high portion of the PWM cycle has completed
            if (last_rising_edge_time != 0) {
                last_high_duration = now - last_rising_edge_time;
            }
        }

        current_state = new_state;
        last_transition_time = now;
    }

    double getDutyCycle() {
        uint64_t now = avr_->cycle;

        // 1. Check for flatline (0% or 100% duty cycle)
        if (now - last_transition_time > TIMEOUT_THRESHOLD) {
            has_synced = false; // Reset sync because the PWM clock stopped
            return (current_state == 1) ? 1.0 : 0.0;
        }

        // 2. Fallback if we haven't witnessed a full PWM cycle yet
        if (!has_synced || current_period == 0) {
            return (current_state == 1) ? 1.0 : 0.0;
        }

        // 3. Calculate duty cycle from the last completed period
        double duty_cycle = static_cast<double>(last_high_duration) / static_cast<double>(current_period);

        // Clamp between 0.0 and 1.0 to handle minor floating point/timing jitter
        if (duty_cycle > 1.0) return 1.0;
        if (duty_cycle < 0.0) return 0.0;

        return duty_cycle;
    }
};


#endif //MMS_CONTROLLER_MOTORCOMPONENT_HPP
