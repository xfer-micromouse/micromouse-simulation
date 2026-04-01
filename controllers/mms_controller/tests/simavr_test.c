#include "simavr/sim_avr.h"
#include "simavr/sim_elf.h"
#include "simavr/avr_ioport.h"

#include <stdio.h>

int main() {
    elf_firmware_t elf = {0};
    if (elf_read_firmware(
        "/home/ian/dev/micromouse/simulator/controllers/mms_controller/arduino_src/arduino_mms_sim/.pio/build/nanoatmega328/firmware.elf",
        &elf) != 0) {
        printf("Failed to read ELF firmware");
        return 1;
    }

    avr_t *avr = avr_make_mcu_by_name("atmega328p");
    if (!avr) {
        printf("Failed to create AVR instance");
        return 1;
    }

    avr_init(avr);
    avr_load_firmware(avr, &elf);

    avr_irq_t *pd7_irq = avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('D'), IOPORT_IRQ_PIN7);

    int state = cpu_Running;
    while ((state != cpu_Done) && (state != cpu_Crashed)) {

        state = avr_run(avr);

        uint32_t current_pd7_state = pd7_irq->value;

        if (current_pd7_state == 1) {
            printf("pd7_irq->value = %d\n", pd7_irq->value);
        } else {
            printf("pd7_irq->value = %d\n", pd7_irq->value);
        }
    }

    return 0;
}
