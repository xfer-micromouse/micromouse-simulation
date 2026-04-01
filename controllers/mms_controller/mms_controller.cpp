#include <webots/Robot.hpp>
#include "DistanceSensorComponent.hpp"
#include "MotorComponent.hpp"
#include <cmath> // Required for INFINITY
#include <functional>
#include <memory>

extern "C" {
#include "simavr/sim_avr.h"
#include "simavr/sim_elf.h"
#include "simavr/avr_uart.h"
}


class Simulator {
public:
    Simulator(const std::string& firmware_path) {
        if (!init_avr(firmware_path, "atmega328p", 16000000)) {
            throw std::runtime_error("Failed to initialize AVR simulator");
        }
        init_webots_components();

    }

    ~Simulator() {
        if (avr_) {
            avr_terminate(avr_);
            avr_ = nullptr;
        }
        delete robot_;
    }

        void run() const {
            const double step_seconds = time_step_ / 1000.0;
            auto cycles_per_step = static_cast<uint64_t>(step_seconds * avr_->frequency);

            while (robot_->step(time_step_) != -1) {
                for (auto& component : components) {
                    component->update_avr();
                }

                uint64_t target_cycle = avr_->cycle + cycles_per_step;
                while (avr_->cycle < target_cycle) {
                    avr_run(avr_);
                }

                for (auto& component : components) {
                    component->update_webots();
                }
            }
        }

private:
    avr_t *avr_ = nullptr;

    webots::Robot * robot_ = nullptr;
    int time_step_;

    std::vector<std::unique_ptr<IComponent>> components;

    bool init_avr(const std::string &firmware_path, const std::string &chip_name, uint32_t frequency) {
        elf_firmware_t elf = {0};
        if (elf_read_firmware(firmware_path.c_str(), &elf) != 0) {
            printf("Failed to read ELF firmware");
            return false;
        }

        avr_ = avr_make_mcu_by_name(chip_name.c_str());
        if (!avr_) {
            printf("Failed to create AVR instance");
            return false;
        }

        avr_init(avr_);
        avr_load_firmware(avr_, &elf);
        avr_->frequency = frequency;
        avr_->vcc = 3300; // 3.3V in millivolts

        // register serial callback
        if (avr_irq_t *uart_tx_irq = avr_io_getirq(avr_, AVR_IOCTL_UART_GETIRQ('0'), UART_IRQ_OUTPUT)) {
            avr_irq_register_notify(uart_tx_irq, uart_output_hook, nullptr);
            std::cout << "Successfully hooked UART0 output." << std::endl;
        } else {
            std::cerr << "Warning: Could not find UART0 on this virtual MCU." << std::endl;
        }

        return true;
    }

    bool init_webots_components() {
        robot_ = new webots::Robot();

        time_step_ = static_cast<int>(robot_->getBasicTimeStep());

        components.push_back(std::make_unique<DistanceSensorComponent>("ds_left", 0));
        components.push_back(std::make_unique<DistanceSensorComponent>("ds_left_front", 1));
        components.push_back(std::make_unique<DistanceSensorComponent>("ds_front", 2));
        components.push_back(std::make_unique<DistanceSensorComponent>("ds_right_front", 3));
        components.push_back(std::make_unique<DistanceSensorComponent>("ds_right", 4));

        components.push_back(std::make_unique<MotorComponent>("l_motor", 'B', 2, true));
        components.push_back(std::make_unique<MotorComponent>("r_motor", 'B', 1, true));

        for (auto& component : components) {
            component->init(robot_, avr_);
        }

        return true;
    }


    static void uart_output_hook(struct avr_irq_t *irq, uint32_t value, void *param) {
        putchar((char) value);
        fflush(stdout);
    }
};

int main(int argc, char **argv) {
    try {
        Simulator simulator("/home/ian/dev/micromouse/simulator/controllers/mms_controller/arduino_src/arduino_mms_sim/.pio/build/nanoatmega328/firmware.elf");
        simulator.run();
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
