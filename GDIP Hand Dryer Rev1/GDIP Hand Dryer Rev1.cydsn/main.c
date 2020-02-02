/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

//SD Card Libraries
#include <FS.h>
#include <string.h>
#include <Global.h>

#include <PCA9685_Custom.h>

///////////////////Interrupt Handlers////////////////////
    CY_ISR( Estop_ISR_Handler ) {
        //estop routine here
        Servo_En_1_Write(1);//Enable servo 1
        Estop_ClearInterrupt();
    }
    
    CY_ISR( Detect_5V_Handler ){
        //Detect 5V Routine here
        Servo_En_1_Write(0);//Disable servo 1
        Detect_5V_ClearInterrupt();   
    }
    
    CY_ISR( UPS_LBO_Handler ){
        //UPS LBO Routine here
        UPS_LBO_ClearInterrupt();
    }

int main(void)
{
    ////////////////////Start Interrputs//////////////////////
    CyGlobalIntEnable; /* Enable global interrupts. */
    Estop_int_StartEx( Estop_ISR_Handler );
    Detect_5V_int_StartEx( Detect_5V_Handler );
    UPS_LBO_int_StartEx( UPS_LBO_Handler);
        
    ///////////////Setup the Servo Controller//////////////////
    PCA9685_Custom_Begin();
    
    ///////////Setup the SD Card//////////////////
    //Call new function 'SD Card Setup()' here
    
    
    //Some code to make the servos do something
    for(;;)
    {
        /* Place your application code here. */
        uint16_t lowLimit = 0;
        uint16_t highLimit = 600;
        
        for(uint16_t i =lowLimit ; i<highLimit;i++){
        PCA9685_Custom_setPWM(1,0,i);
        PCA9685_Custom_setPWM(5,0,i);
        CyDelay(10);
        }
        for(uint16_t i =highLimit ; i>lowLimit;i--){
        PCA9685_Custom_setPWM(1,0,i);
        PCA9685_Custom_setPWM(5,0,i);
        CyDelay(10);
        }
    }
}

/* [] END OF FILE */
