/* 
 * File:   Ultrasonic.c
 * Author: ALMASRIA 4 COMP
 *
 * Created on October 31, 2024, 12:19 AM
 */

#include "HC_SR04.h"
#include "../../MCAL_Layer/Timers/TIMER1_ICU/ICU.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
volatile uint8 g_flag = 0;      /* This flag is set when the high time is calculated */
volatile uint8 g_edgeCount = 0; /* Variable to count the edges */
volatile uint16 g_timeHigh = 0; /* This is the time in which the signal travels from HC-SR04 to object and returns to the module HC-SR-04 again */

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description :
 * Function responsible for initialize Ultrasonic Sensor
 */
void Ultrasonic_init(void) {
	/* Configure the ICU Driver with F_CPU/8 and wait for the RAISING edge at the beginning */
	ICU_ConfigType ICU_Configurations = { F_CPU_8, RAISING };
	ICU_init(&ICU_Configurations);

	/* Ultrasonic_edgeProcessing() is the CallBack Function called by the ICU Driver */
	ICU_setCallBack(Ultrasonic_edgeProcessing);

	/* The TRIGGER_PIN is the pin responsible for sending the Trigger pulse to the Ultrasonic Sensor */
    pin_Config_t ICP1 = {
      .port = TRIGGER_PORT,
      .pin  =  TRIGGER_PIN,
      .direction = GPIO_DIRECTION_OUTPUT,
      .logic = GPIO_LOW
    };
    gpio_pin_initialize(&ICP1);
}
/*
 * Description :
 * Function responsible for sending the Trigger pulse to the Ultrasonic Sensor.
 */
void Ultrasonic_Trigger(void) {
	/* Send a 20 microseconds pulse to the Ultrasonic Sensor through the TRIGGER_PIN */
	pin_Config_t ICP1 = {
      .port = TRIGGER_PORT,
      .pin  =  TRIGGER_PIN,
      .direction = GPIO_DIRECTION_OUTPUT,
      .logic = GPIO_LOW
    };
    gpio_pin_write_logic(&ICP1, GPIO_HIGH);
	_delay_us(20);
	gpio_pin_write_logic(&ICP1, GPIO_LOW);
}
/*
 * Description :
 * Function responsible for sending the trigger pulse by using Ultrasonic_Trigger() function
 * and start the measurements by the ICU from this moment.
 */
uint16 Ultrasonic_readDistance(void) {
	uint16 distnace;
	Ultrasonic_Trigger(); /* Send Rrigger pulse */
	while (g_flag != 1); /* Wait till calculate the high time */

	/* we have selected an internal 8MHz oscillator frequency for ATmega32
	 * with Prescaler F_CPU/8 for timer frequency
	 * then time to execute 1 instruction is 1 us*/
	distnace = (g_timeHigh / 57.5);
	/* (TIMER value) / 58.8 ( 57.5 give me best results) cm */

	return distnace;
}
/*
 * Description :
 * This is the call back function called by the ICU driver, It is used to calculate
 * the high time (pulse time) generated by the Ultrasonic Sensor.
 */
void Ultrasonic_edgeProcessing(void) {
	g_edgeCount++;
	g_flag = 0; /* Clear the g_flag */
	if (g_edgeCount == 1) /* Fisrt Edge (RISING) */
	{
		ICU_clearTimerValue(); /* Reset the timer counter */
		ICU_setEdgeDetectionType(FALLING);
	} else if (g_edgeCount == 2) /* Second Edge (FALLING) */
	{
		g_timeHigh = ICU_getInputCaptureValue(); /* Store the High time value */
		ICU_setEdgeDetectionType(RAISING);
		g_edgeCount = 0;
		g_flag = 1; /* Set g_flag to execute the distance measurement in the Ultrasonic_readDistance Function */
	}
}