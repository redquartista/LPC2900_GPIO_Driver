#include <stdio.h>
#include "gpio.h"

volatile gpio_reg *gpio_arr = (void *) GPIO_BASE_ADDR;

static status_e parameterValidWrite(gpio_lpc2901_e gpio, gpio_accessType_e ch, uint8_t pin, uint32_t value) {

    if(gpio >= gpio_all_e) {
        printf("Invalid GPIO choice.\n");
        return error_e;
    }

    if( ch != pin_e || ch != port_e ) {
        printf("Invalid choice.\n");
        return error_e;
    }

    if( (ch == pin_e) && (pin > GPIO_PINS_NUM - 1)) {
        printf("Invalid pin number.\n");
        return error_e;
    }

    if( (ch == pin_e) && (value != 0 || value != 1)) {
        printf("Invalid value to be written for a pin. Write 1 or 0.\n");
        return error_e;
    }  
}

static status_e parameterValidRead(gpio_lpc2901_e gpio, gpio_accessType_e ch, uint8_t pin) {

    if( gpio >= gpio_all_e) {
        printf("Invalid GPIO choice.\n");
        return error_e;
    }

    if( ch != pin_e || ch != port_e ) {
        printf("Invalid choice.\n");
        return error_e;
    }

    if( (ch == pin_e) && (pin > GPIO_PINS_NUM - 1)) {
        printf("Invalid pin number.\n");
        return error_e;
    }
}

status_e gpioWrite(gpio_lpc2901_e gpio, gpio_accessType_e ch, uint8_t pin, uint32_t value){

    /*Check if the input parameters are permissible as per LPC2901 specifications*/
   if(!parameterValidWrite(gpio, ch, pin, value))
       return error_e;

    switch (ch)
    {
    case port_e:
        //Set all pins on that gpio as output pins
        gpio_arr[gpio].dr_reg = 0xffff;
        //Write the value to the gpio port
        gpio_arr[gpio].or_reg = value;
        break;
    
    case pin_e:
        //Set the particular pin as an output pin
        gpio_arr[gpio].dr_reg |= (1<<pin);
        
        //Write the value to the pin
        if(value)
            gpio_arr[gpio].or_reg |= (1<<pin); //Set
        else
            gpio_arr[gpio].or_reg &= ~(1<<pin); //Clear
        break;
    
    default:
        return error_e;
        break;
    }

    return success_e;
}

status_e gpioRead(gpio_lpc2901_e gpio, gpio_accessType_e ch, uint8_t pin, uint32_t value){

    /*Check if the input parameters are valid  as per LPC2901 specifications*/
   if(!parameterValidRead(gpio, ch, pin))
       return error_e;

    switch (ch)
    {
    case port_e:
        //Set all pins on that GPIO as input pins
        gpio_arr[gpio].dr_reg = 0x0;
        //Read the input level on the GPIO port into the variable
        value = gpio_arr[gpio].pins_reg;
        break;
    
    case pin_e:
        //Set the particular pin as an input pin
        gpio_arr[gpio].dr_reg &= ~(1<<pin);
        //Read the input level on the pin into the variable
        value = ((gpio_arr[gpio].pins_reg >> pin) & 1);  

    default:
        return error_e;
        break;
    }

    return success_e;
}

void testGPIO(void)
{   
    /*Test writing to a pin
    **Write 1 to pin 10 of GPIO0
    */
    if(gpioWrite(gpio0_e, pin_e, 10ul, 1ul))
        printf("Wrote to  a pin successfully\n");
    else
        printf("Failure in writing to a pin\n");
    
    /*Test writing to an entire GPIO
    **Write 0x0123abcd to entire GPIO1 port
    */
    if(gpioWrite(gpio1_e, port_e, 0, 0x0123abcdul))
        printf("Wrote to an entire port successfully\n");
    else
        printf("Failure in writing to an entire port\n");


    uint32_t value = 0;
    /*Test reading from a GPIO pin
    **Read input level on pin 11 of GPIO2
    */
    if(gpioRead(gpio2_e, pin_e, 11ul, value))
        printf("Read %08x from a pin successfully\n", (unsigned int) value);
    else
        printf("Failure in reading from a pin\n");
    
    /*Test reading to an entire GPIO
    **Read the entire GPIO3 port
    */
    if(gpioRead(gpio3_e, port_e, 0ul, value))
        printf("Read %08x from an entire GPIO port successfully\n", (unsigned int) value);
    else
        printf("Failure in reading from an entire port\n");

}