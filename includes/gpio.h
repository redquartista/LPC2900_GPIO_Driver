#include <stdio.h>

#define GPIO_BASE_ADDR  (uint32_t) 0xE004A000ul 

/**
 * @brief enum to specify the access-type for the GPIO
 * 
 */
typedef enum{
    pin_e,
    port_e
} gpio_accessType_e;


/*Pins per port*/
#define GPIO_PINS_NUM   (uint32_t) 32ul

/**
*@brief Enumerating the available GPIOs
*
*/
typedef enum{
    gpio0_e,
    gpio1_e,
    gpio2_e,
    gpio3_e,
    gpio_all_e
} gpio_lpc2901_e;

/**
 * @brief  An enum to codify error and success states 
 * 
 */
typedef enum {
    error_e = 0,
    success_e
} status_e;

typedef enum{
    gpio0_e,
    gpio1_e,
    gpio2_e,
    gpio3_e,
    gpio_all_e
} gpio_lpc2901_e;

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
    //Distance between two GPIOs. Not to be accessed.
    char doNotAccess[0x1000ul - 0xcul];
}gpio_reg;

/**
 * @brief Write to a GPIO port or a particular pin on that port
 *
 * @param gpio A valid GPIO [gpio0_e - gpio3_e]
 * @param ch A valid choice of access. [pin_e / port_e]. ch=port_e will write to the entire port, ch=pin_e will write only to a 'pin' on the gpio port.
 * @param pin A valid choice of pin number [0-31]. It is ignored if 'port_e' is passed at 'ch'
 * @param value A valid value. [0x0 to 0xffffffff for ch = port_e; 0 or 1 for ch = pin_e]
 * 
 * @return A return value indicating the success or failure of the operation [success_e / error_e]
 */
status_e gpioWrite(gpio_lpc2901_e gpio, gpio_accessType_e ch, uint8_t pin, uint32_t value);

/**
 * @brief  Read from a GPIO port or input level on a particular pin on that port
 *
 * @param gpio A valid GPIO [gpio0_e - gpio3_e]
 * @param ch A valid choice of access [pin_e / port_e]. ch = port_e will write to the entire port, ch = pin_e will write only to a 'pin' on the gpio port
 * @param pin A valid choice of pin number [0-31]. It is ignored if ch = port_e.
 * @param value Address of the variable into which the GPIO / Pin input level is to be read
 *  
 * @return A return value indicating the success or failure of the operation [success_e / error_e]
 */
status_e gpioRead(gpio_lpc2901_e gpio, gpio_accessType_e ch, uint8_t pin, uint32_t* value);


/**
 * @brief Test the GPIO device driver
 *
 */
 void testGPIO();