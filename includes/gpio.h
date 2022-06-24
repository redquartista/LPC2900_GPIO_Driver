#include <stdio.h>

#define GPIO_BASE_ADDR  0xE004A000 

/*Macros for access type*/
#define PIN     0
#define PORT    1

/*Pins per port*/
#define GPIO_PINS_NUM   32

/**
*@brief Enumerating available GPIOs
*
*/
enum gpio_lpc2901{
    GPIO0,
    GPIO1,
    GPIO2,
    GPIO3,

    GPIO_NUM
};

/*Macros to denote success or failure*/
#define SUCCESS     1
#define FAILURE     0

typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

/**
 * @brief A structure access GPIOs as array members
 * 
 */
typedef struct {
    uint32_t pins_reg;
    uint32_t or_reg;
    uint32_t dr_reg;
    char doNotAccess[0x1000 - 0xc]; //Distance between two GPIOs. Not to be accessed.
}gpio_reg;

/**
 * @brief Write to a GPIO port or a particular pin on that port
 *
 * @param gpio A valid GPIO [GPI0-GPIO3]
 * @param ch A valid choice of access. [PORT / PIN]. ch=PORT will write to the entire port, ch=PIN will write only to a 'pin' on the gpio port.
 * @param pin A valid choice of pin number [0-31]. It is ignored if 'PORT' is passed at 'ch'
 * @param value A valid value. [0x0 to 0xffffffff for ch=PORT, 0 or 1 for ch=PIN]
 * 
 * @return A return value indicating the success or failure of the operation [SUCCSSS / FAILURE]
 */
uint32_t gpioWrite(uint8_t gpio, uint8_t ch, uint8_t pin, uint32_t value);

/**
 * @brief  Read from a GPIO port or input level on a particular pin on that port
 *
 * @param gpio A valid GPIO [GPIO0-GPIO3]
 * @param ch A valid choice of access [PORT / PIN]. ch=PORT will write to the entire port, ch=PIN will write only to a 'pin' on the gpio port
 * @param pin A valid choice of pin number [0-31]. It is ignored if ch = PORT.
 * @param value A variable which the GPIO / Pin input level is to be read into
 *  
 * @return A return value indicating the success or failure of the operation [SUCCSSS / FAILURE]
 */
uint32_t gpioRead(uint8_t gpio, uint8_t ch, uint8_t pin, uint32_t value);


/**
 * @brief Test the GPIO device driver
 *
 */
 void testGPIO();