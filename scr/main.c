#include <stdlib.h>
#include <stdint.h>

#define STM32_GPIOA_BASE     0x48000000U // Localizaçao do GPIO A

#define STM32_RCC_BASE       0x40023800     /* 0x40023800-0x40023bff: Reset and Clock control RCC */

/* GPIOC Base Addresses ******************************************************/

#define STM32_GPIOC_BASE     0x40020800     /* 0x48000800-0x48000bff: GPIO Port C */

/* Register Offsets *********************************************************/

#define STM32_RCC_AHB1ENR_OFFSET  0x0030   /* AHB1 Peripheral Clock enable register */

#define STM32_GPIO_MODER_OFFSET   0x0000  /* GPIO port mode register */
#define STM32_GPIO_OTYPER_OFFSET  0x0004  /* GPIO port output type register */
#define STM32_GPIO_PUPDR_OFFSET   0x000c  /* GPIO port pull-up/pull-down register */
#define STM32_GPIO_ODR_OFFSET     0x0014  /* GPIO port output data register */
#define STM32_GPIO_BSRR_OFFSET    0x0018  /* GPIO port bit set/reset register */

#define STM32_RCC_AHB1ENR           (STM32_RCC_BASE+STM32_RCC_AHB1ENR_OFFSET)
#define STM32_GPIOC_MODER           (STM32_GPIOC_BASE+STM32_GPIO_MODER_OFFSET)
#define STM32_GPIOA_MODER           (STM32_GPIOA_BASE+STM32_GPIO_MODER_OFFSET)
#define STM32_GPIOC_OTYPER          (STM32_GPIOC_BASE+STM32_GPIO_OTYPER_OFFSET)

#define STM32_GPIOC_PUPDR           (STM32_GPIOC_BASE+STM32_GPIO_PUPDR_OFFSET)
#define STM32_GPIOC_BSRR            (STM32_GPIOC_BASE + STM32_GPIO_BSRR_OFFSET)

/* Registradores do GPIOA */

#define STM32_GPIO_INPDATA_OFFSET   0x0010  // Offset do registrador de input data
#define STM32_GPIOA_INPDATA         (STM32_GPIOA_BASE+STM32_GPIO_INPDATA_OFFSET) // Localização do registrador de input data
#define STM32_GPIOA_PUPDR           (STM32_GPIOA_BASE+STM32_GPIO_PUPDR_OFFSET) // Localização do registrador pull up/pull down de A
#define GPIO_INPDATA_MASK(n)       (1 << n) // Máscara para determinar o valor de uma porta input

#define RCC_AHB1ENR_GPIOCEN         (1 << 2)
#define RCC_AHB1ENR_GPIOAEN         (1 << 0)  // Atribuindo bit 1 para o periférico PA0 
#define GPIO_MODER_INPUT           (0) /* Input */
#define GPIO_MODER_OUTPUT          (1) /* General purpose output mode */
#define GPIO_MODER_ALT             (2) /* Alternate mode */
#define GPIO_MODER_ANALOG          (3) /* Analog mode */

#define GPIO_MODER_SHIFT(n)        (n << 1)
#define GPIO_MODER_MASK(n)         (3 << GPIO_MODER_SHIFT(n))

/* GPIO port output type register */

#define GPIO_OTYPER_PP             (0) /* 0=Output push-pull */
#define GPIO_OTYPER_OD             (1) /* 1=Output open-drain */

#define GPIO_OT_SHIFT(n)           (n)
#define GPIO_OT_MASK(n)            (1 << GPIO_OT_SHIFT(n))

/* GPIO port pull-up/pull-down register */

#define GPIO_PUPDR_NONE            (0) /* No pull-up, pull-down */
#define GPIO_PUPDR_PULLUP          (1) /* Pull-up */
#define GPIO_PUPDR_PULLDOWN        (2) /* Pull-down */

#define GPIO_PUPDR_SHIFT(n)        (n << 1)
#define GPIO_PUPDR_MASK(n)         (3 << (n << 1))

/* GPIO port bit set/reset register */

#define GPIO_BSRR_SET(n)           (1 << (n))
#define GPIO_BSRR_RESET(n)         (1 << ((n) + 16))


bool button_status(uint32_t *pGPIOA_INPDATA){
    uint32_t reg;
    reg = *pGPIOA_INPDATA;
    reg &= GPIO_INPDATA_MASK(0);
    return !reg;
}

int main(int argc, char *argv[])
{
    uint32_t reg;
    uint32_t *pRCC_AHB1ENR = (uint32_t *)STM32_RCC_AHB1ENR;
    uint32_t *pGPIOC_MODER = (uint32_t *)STM32_GPIOC_MODER;
    uint32_t *pGPIOA_MODER = (uint32_t *)STM32_GPIOA_MODER;   // Registrador do modo de operaçao de A
    uint32_t *pGPIOC_OTYPER = (uint32_t *)STM32_GPIOC_OTYPER;
    uint32_t *pGPIOC_PUPDR = (uint32_t *)STM32_GPIOC_PUPDR;
    uint32_t *pGPIOA_PUPDR = (uint32_t *)STM32_GPIOA_PUPDR; // Registrador para pull up/pull down de A
    uint32_t *pGPIOC_BSRR = (uint32_t *)STM32_GPIOC_BSRR;
    uint32_t *pGPIOA_INPDATA = (uint32_t *)STM32_GPIOA_INPDATA;  // Registrador de input da porta A
    uint32_t delay = 0;

    reg = *pRCC_AHB1ENR;
    reg |= RCC_AHB1ENR_GPIOCEN;
    *pRCC_AHB1ENR = reg;

    reg = *pRCC_AHB1ENR; // Ligando o clock no periférico PA0
    reg |= RCC_AHB1ENR_GPIOAEN;
    *pRCC_AHB1ENR = reg;
    
    reg = *pGPIOC_MODER;
    reg &= ~GPIO_MODER_MASK(13);
    reg |= (GPIO_MODER_OUTPUT << GPIO_MODER_SHIFT(13));
    *pGPIOC_MODER = reg;

    reg = *pGPIOA_MODER;
    reg &= ~GPIO_MODER_MASK(13);
    reg |= (GPIO_MODER_INPUT << GPIO_MODER_SHIFT(13));
    *pGPIOA_MODER = reg;

    reg = *pGPIOC_OTYPER;
    reg &= ~GPIO_OT_MASK(13);
    reg |= (GPIO_OTYPER_PP << GPIO_OT_SHIFT(13));
    *pGPIOC_OTYPER = reg;

    reg = *pGPIOC_PUPDR;
    reg &= ~GPIO_PUPDR_MASK(13);
    reg |= (GPIO_PUPDR_NONE << GPIO_PUPDR_SHIFT(13));
    *pGPIOC_PUPDR = reg;

    reg = *pGPIOA_PUPDR;
    reg &= ~GPIO_PUPDR_MASK(13);
    reg |= (GPIO_PUPDR_PULLUP << GPIO_PUPDR_SHIFT(13));
    *pGPIOA_PUPDR = reg;

    while(1)
    {
        if (button_status(pGPIOA_INPDATA))
            delay = 500000;
        else
            delay = 50000;
        *pGPIOC_BSRR = GPIO_BSRR_RESET(13);
        for(uint32_t i = 0; i < delay; i++);
        *pGPIOC_BSRR = GPIO_BSRR_SET(13);
        for(uint32_t i = 0; i < delay; i++);
    }

    return EXIT_SUCCESS;
}