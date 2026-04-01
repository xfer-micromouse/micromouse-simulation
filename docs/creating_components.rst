Creating and Connecting Components
==================================

Purpose
-------

Components are the bridge between Webots devices and simavr peripherals.
Each component must implement the IComponent interface and define how data
flows in both directions.

IComponent Contract
-------------------

Every component implements:

- init(robot, avr): locate devices, cache pointers, and register IRQ hooks.
- update_avr(): push Webots-side values into simulated MCU peripherals.
- update_webots(): apply MCU outputs to Webots actuators.

Design Workflow
---------------

1. Decide direction of data flow:
   - Sensor-like component: mostly update_avr.
   - Actuator-like component: mostly update_webots.
2. Choose MCU endpoint:
   - ADC channel, GPIO pin, PWM pin, UART IRQ, etc.
3. Match names with the Webots world device names exactly.
4. Register the component in simulator initialization.

Example: Add a New Distance Sensor to ADC5
------------------------------------------

The current DistanceSensorComponent already handles ADC mapping. To add a
new physical reading path, instantiate it with the Webots device name and
ADC channel.

In controller initialization:

.. code-block:: cpp

   components.push_back(std::make_unique<DistanceSensorComponent>("ds_rear", 5));

This means:

- Webots device name: ds_rear
- simavr ADC channel: ADC5

Example: Implement a New Actuator Component
-------------------------------------------

Below is a minimal example component that listens to one AVR GPIO pin and
uses it as an on/off command for a Webots motor.

.. code-block:: cpp

   #include "IComponent.hpp"
   #include "webots/Motor.hpp"

   extern "C" {
   #include "simavr/avr_ioport.h"
   }

   class DigitalMotorEnableComponent : public IComponent {
   public:
       DigitalMotorEnableComponent(const std::string& name, char port, uint8_t pin)
           : IComponent(name), avr_(nullptr), motor_(nullptr), port_(port), pin_(pin) {}

       void init(webots::Robot* robot, avr_t* avr) override {
           avr_ = avr;
           motor_ = robot->getMotor(name);

           avr_irq_t* pin_irq = avr_io_getirq(avr_, AVR_IOCTL_IOPORT_GETIRQ(port_), pin_);
           avr_irq_register_notify(pin_irq, on_pin_change, this);

           if (motor_) {
               motor_->setPosition(INFINITY);
           }
       }

       void update_avr() override {
           // No sensor data pushed to AVR for this component.
       }

       void update_webots() override {
           if (!motor_) return;
           motor_->setVelocity(enabled_ ? motor_->getMaxVelocity() : 0.0);
       }

   private:
       static void on_pin_change(struct avr_irq_t*, uint32_t value, void* param) {
           auto* self = static_cast<DigitalMotorEnableComponent*>(param);
           self->enabled_ = (value > 0);
       }

       avr_t* avr_;
       webots::Motor* motor_;
       const char port_;
       const uint8_t pin_;
       bool enabled_ = false;
   };

Connect the New Component in Simulator Setup
--------------------------------------------

Register your component alongside the existing mappings:

.. code-block:: cpp

   components.push_back(std::make_unique<DigitalMotorEnableComponent>(
       "l_motor", 'D', 7
   ));

Then keep the existing initialization loop:

.. code-block:: cpp

   for (auto& component : components) {
       component->init(robot_, avr_);
   }

Validation Checklist
--------------------

- Confirm Webots device name exists in the world/proto.
- Confirm MCU pin/channel matches firmware configuration.
- Add UART debug prints in firmware to observe pin/channel activity.
- Run a short simulation and verify both expected and edge-case behavior.
