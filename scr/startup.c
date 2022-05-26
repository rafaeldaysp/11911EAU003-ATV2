#include <stdint.h>

#define SRAM_START 0x20000000U
#define SRAM_SIZE (128U * 1024)
#define SRAM_END  SRAM_START + SRAM_SIZE

#define STACK_START SRAM_END

void main();
void reset_handler();
void nmi_handler();

uint32_t vectors[] __attribute__((section (".isr_section"))) =
{
    STACK_START,
    (uint32_t)reset_handler,
    (uint32_t)nmi_handler,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0

};

void reset_handler(void)
{
    main();
}

void nmi_handler(void)
{
    while(1);
}