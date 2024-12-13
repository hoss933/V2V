/* 
 * File:   Ultrasonic.h
 * Author: Hossam Reda
 *
 * Created on October 31, 2024, 12:17 AM
 */

#ifndef ULTRASONIC_H
#define	ULTRASONIC_H

/*Section : Includes */
#include "../../MCAL_Layer/GPIO/hal_gpio.h"

/*Section : Macro Declaration*/

#define TRIGGER_PORT PORTB_INDEX
#define TRIGGER_PIN  GPIO_PIN5

/*Section : Macro Function*/

/*Section : Data type Declaration*/

/*Section : Function Declaration*/

/*
 * Description :
 * Function responsible for initialize Ultrasonic Sensor
 */
void Ultrasonic_init(void);

/*
 * Description :
 * Function responsible for sending the Trigger pulse to the Ultrasonic Sensor.
 */
void Ultrasonic_Trigger(void);

/*
 * Description :
 * Function responsible for sending the trigger pulse by using Ultrasonic_Trigger() function
 * and start the measurements by the ICU from this moment.
 */
uint16 Ultrasonic_readDistance(void);

/*
 * Description :
 * This is the call back function called by the ICU driver, It is used to calculate
 * the high time (pulse time) generated by the Ultrasonic Sensor.
 *
 */
void Ultrasonic_edgeProcessing(void);

#endif	/* ULTRASONIC_H */



