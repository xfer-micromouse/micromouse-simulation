#ifndef MMS_CONTROLLER_DISTANCESENSORCOMPONENT_H
#define MMS_CONTROLLER_DISTANCESENSORCOMPONENT_H

#include "IComponent.hpp"
#include "webots/DistanceSensor.hpp"

extern "C" {
#include "simavr/avr_adc.h"
}

class DistanceSensorComponent : public IComponent {
public:
    DistanceSensorComponent(const std::string& name, int adc_channel)
        : IComponent(name), avr_(nullptr), sensor_(nullptr), adc_channel_(adc_channel) {}

    void init(webots::Robot* r, avr_t* a) override {
        avr_ = a;
        sensor_ = r->getDistanceSensor(name);
        if (sensor_) {
            int time_step = static_cast<int>(r->getBasicTimeStep());
            sensor_->enable(time_step);
            printf("Init Distance Sensor: %s on ADC %u\n", name.c_str(), adc_channel_);
        } else {
            fprintf(stderr, "Warning: Webots device '%s' not found!\n", name.c_str());
        }
    }

    void update_webots() override { }

    void update_avr() override {
        double dist_val = sensor_->getValue();

        auto mv_value = static_cast<uint32_t>((dist_val / sensor_->getMaxValue()) * avr_->vcc);
        if (mv_value > avr_->vcc) {
            mv_value = avr_->vcc;
        }

        avr_irq_t* adc_irq = avr_io_getirq(avr_, AVR_IOCTL_ADC_GETIRQ, adc_channel_);
        avr_raise_irq(adc_irq, mv_value);
    }

private:
    avr_t* avr_;
    webots::DistanceSensor* sensor_;
    int adc_channel_;

};


#endif //MMS_CONTROLLER_DISTANCESENSORCOMPONENT_H