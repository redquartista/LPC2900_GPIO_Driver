#include <stdio.h>
#include "gpio.h"

volatile gpio_reg *gpio_arr = (void *) GPIO_BASE_ADDR;

static uint32_t parameterValidWrite(uint8_t gpio, uint8_t ch, uint8_t pin, uint32_t value) {

    if( gpio > GPIO_NUM - 1) {
        printf("Invalid GPIO choice.\n");
        return FAILURE;
    }

    if( ch != PIN || ch != PORT ) {
        printf("Invalid choice.\n");
        return FAILURE;
    }

    if( (ch == PIN) && (pin > GPIO_PINS_NUM - 1)) {
        printf("Invalid pin number.\n");
        return FAILURE;
    }

    if( (ch == PIN) && (value != 0 || value != 1)) {
        printf("Invalid value to be written for a pin. Write 1 or 0.\n");
        return FAILURE;
    }     
}

static uint32_t parameterValidRead(uint8_t gpio, uint8_t ch, uint8_t pin) {

    if( gpio > GPIO_NUM - 1) {
        printf("Invalid GPIO choice.\n");
        return FAILURE;
    }

    if( ch != PIN || ch != PORT ) {
        printf("Invalid choice.\n");
        return FAILURE;
    }

    if( (ch == PIN) && (pin > GPIO_PINS_NUM - 1)) {
        printf("Invalid pin number.\n");
        return FAILURE;
    }
}

uint32_t gpioWrite(uint8_t gpio, uint8_t ch, uint8_t pin, uint32_t value){

    /*Check if the input parameters are permissible as per LPC2901 specifications*/
   if(!parameterValidWrite(gpio, ch, pin, value))
       return FAILURE;

    switch (ch)
    {
    case PORT:
        //Set all pins on that gpio as output pins
        gpio_arr[gpio].dr_reg = 0xffff;
        //Write the value to the gpio port
        gpio_arr[gpio].or_reg = value;
        break;
    
    case PIN:
        //Set the particular pin as an output pin
        gpio_arr[gpio].dr_reg |= (1<<pin);
        
        //Write the value to the pin
        if(value)
            gpio_arr[gpio].or_reg |= (1<<pin); //Set
        else
            gpio_arr[gpio].or_reg &= ~(1<<pin); //Clear
        break;
    
    default:
        return FAILURE;
        break;
    }

    return SUCCESS;
}

uint32_t gpioRead(uint8_t gpio, uint8_t ch, uint8_t pin, uint32_t value){

    /*Check if the input parameters are valid  as per LPC2901 specifications*/
   if(!parameterValidRead(gpio, ch, pin))
       return FAILURE;

    switch (ch)
    {
    case PORT:
        //Set all pins on that GPIO as input pins
        gpio_arr[gpio].dr_reg = 0x0;
        //Read the input level on the GPIO port into the variable
        value = gpio_arr[gpio].pins_reg;
        break;
    
    case PIN:
        //Set the particular pin as an input pin
        gpio_arr[gpio].dr_reg &= ~(1<<pin);
        //Read the input level on the pin into the variable
        value = ((gpio_arr[gpio].pins_reg >> pin) & 1);  

    default:
        return FAILURE;
        break;
    }

    return SUCCESS;
}

void testGPIO(void)
{   
    /*Test writing to a pin
    **Write 1 to pin 10 of GPIO0
    */
    if(gpioWrite(GPIO0, PIN, 10, 1))
        printf("Wrote to  a pin successfully\n");
    else
        printf("Failure in writing to a pin\n");
    
    /*Test writing to an entire GPIO
    **Write 0x0123abcd to entire GPIO1 port
    */
    if(gpioWrite(GPIO1, PORT, 0, 0x0123abcd))
        printf("Wrote to an entire port successfully\n");
    else
        printf("Failure in writing to an entire port\n");


    uint32_t value = 0;
    /*Test reading from a GPIO pin
    **Read input level on pin 11 of GPIO2
    */
    if(gpioRead(GPIO2, PIN, 11, value))
        printf("Read %08x from a pin successfully\n", (unsigned int) value);
    else
        printf("Failure in reading from a pin\n");
    
    /*Test reading to an entire GPIO
    **Read the entire GPIO3 port
    */
    if(gpioRead(GPIO3, PORT, 0, value))
        printf("Read %08x from an entire GPIO port successfully\n", (unsigned int)value);
    else
        printf("Failure in reading from an entire port\n");

}