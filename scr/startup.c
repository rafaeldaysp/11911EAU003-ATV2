#include <stdint.h>

#define SRAM_START 0x20000000U
#define SRAM_SIZE (128U * 1024U)
#define SRAM_END  SRAM_START + SRAM_SIZE

#define STACK_START SRAM_END

void main();
void reset_handler();
void nmi_handler        (void) __attribute__ ((weak, alias("default_handler")));
void hardfault_handler  (void) __attribute__ ((weak, alias("default_handler")));
void memmanage_handler  (void) __attribute__ ((weak, alias("default_handler")));
void busfault_handler   (void) __attribute__ ((weak, alias("default_handler")));
void usagefault_handler (void) __attribute__ ((weak, alias("default_handler")));
void svc_handler        (void) __attribute__ ((weak, alias("default_handler")));
void debugmon_handler   (void) __attribute__ ((weak, alias("default_handler")));
void pendsv_handler     (void) __attribute__ ((weak, alias("default_handler")));
void systick_handler    (void) __attribute__ ((weak, alias("default_handler")));

extern uint32_t _sdata;    /* Inicio da secao .data */
extern uint32_t _edata;    /* Fim da secao .data */
extern uint32_t _la_data;  /* Endereco de carga na RAM da secao .data */
extern uint32_t _etext;
extern uint32_t _sbss;     /* Inicio da secao .bss */
extern uint32_t _ebss;     /* Fim da secao .bss */

uint32_t vectors[] __attribute__((section (".isr_section"))) =
{
    STACK_START,
    (uint32_t)reset_handler, 
    (uint32_t)nmi_handler, 
    (uint32_t)hardfault_handler, 
    (uint32_t)memmanage_handler, 
    (uint32_t)busfault_handler,
    (uint32_t)usagefault_handler,
    0,
    0,
    0,
    0,
    (uint32_t)svc_handler,
    (uint32_t)debugmon_handler,
    0,
    (uint32_t)pendsv_handler,
    (uint32_t)systick_handler,

};

void reset_handler(void)
{

    /* Copia a secao .data para a RAM */
    uint32_t i;
    uint32_t size = (uint32_t)&_edata - (uint32_t)&_sdata;
    uint8_t *pDst = (uint8_t*)&_sdata; 
    uint8_t *pSrc = (uint8_t*)&_etext;

    for(i = 0; i < size; i++)
    {
        *pDst++ = *pSrc++;
    }

    /* Preenche a secao .bss com zero */
    size = (uint32_t)&_ebss - (uint32_t)&_sbss;
    pDst = (uint8_t*)&_sbss;
    for(i = 0 ; i < size; i++)
    {
        *pDst++ = 0;
    }

    main();
}

void default_handler(void)
{
    while(1);
}