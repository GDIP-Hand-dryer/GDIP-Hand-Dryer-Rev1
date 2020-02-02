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
#include <PCA9685_Custom.h>
#include <math.h>
#define I2C_SLAVE_ADDR      (0x40u)

//float freq = 1600.0;
float oscillator_frequency = 27000000;

void PCA9685_Custom_Begin(){
    I2C_1_Start();
    PCA9685_Custom_Reset();
    PCA9685_Custom_setPWMFreq(60);
}

void PCA9685_Custom_Flash_All(){
        I2C_1_MasterSendStart(0x40, 0);     //Start Transmission to Servo controller
        I2C_1_MasterWriteByte(0xFA);        //Start write to channel 1 on/off times
        I2C_1_MasterWriteByte(0x00);        //Ch1 On time, Low byte
        I2C_1_MasterWriteByte(0xFB);        //Start write to channel 1 on/off times
        I2C_1_MasterWriteByte(0x00);        //ch1 On time, High byte
        I2C_1_MasterWriteByte(0xFC);        //Start write to channel 1 on/off times
        I2C_1_MasterWriteByte(0x01);        //Ch1 Off time, Low byte
        I2C_1_MasterWriteByte(0xFD);        //Start write to channel 1 on/off times
        I2C_1_MasterWriteByte(0x00);        //Ch1 Off time, High byte
        I2C_1_MasterSendStop();
        CyDelay(1000);
        I2C_1_MasterSendStart(0x40, 0);
        I2C_1_MasterWriteByte(0xFA);
        I2C_1_MasterWriteByte(0x00);
        I2C_1_MasterWriteByte(0xFB);        //Start write to channel 1 on/off times
        I2C_1_MasterWriteByte(0x00);
        I2C_1_MasterWriteByte(0xFC);        //Start write to channel 1 on/off times
        I2C_1_MasterWriteByte(0x00);
        I2C_1_MasterWriteByte(0xFD);        //Start write to channel 1 on/off times
        I2C_1_MasterWriteByte(0xFF);
        I2C_1_MasterSendStop();
        CyDelay(1000);
}

void PCA9685_Custom_Reset(void) {
 PCA9685_Custom_Write8(PCA9685_MODE1, 0x80);
}

void PCA9685_Custom_setPWM(uint8 num, uint16 on, uint16 off){
  I2C_1_MasterSendStart(I2C_SLAVE_ADDR, I2C_WRITE_XFER_MODE);
  I2C_1_MasterWriteByte(CH0_ON_L+(0x04*num));
  I2C_1_MasterWriteByte(on);
  I2C_1_MasterWriteByte(on>>8);
  I2C_1_MasterWriteByte(off);
  I2C_1_MasterWriteByte(off>>8);
  I2C_1_MasterSendStop();
}

void PCA9685_Custom_Set_All_PWM(uint16 on, uint16 off){
  I2C_1_MasterSendStart(I2C_SLAVE_ADDR, I2C_WRITE_XFER_MODE);
  I2C_1_MasterWriteByte(ALLLED_ON_L);
  I2C_1_MasterWriteByte(on);
  I2C_1_MasterWriteByte(ALLLED_ON_H);
  I2C_1_MasterWriteByte(on>>8);
  I2C_1_MasterWriteByte(ALLLED_OFF_L);
  I2C_1_MasterWriteByte(off);
  I2C_1_MasterWriteByte(ALLLED_ON_H);
  I2C_1_MasterWriteByte(off>>8);
  I2C_1_MasterSendStop();
}

void PCA9685_Custom_setPWMFreq(float freq) {
  //Serial.print("Attempting to set freq ");
  //Serial.println(freq);
  freq *= 0.9;  // Correct for overshoot in the frequency setting (see issue #11).
  float prescaleval = 25000000;
  prescaleval /= 4096;
  prescaleval /= freq;
  prescaleval -= 1;
 
  uint8_t prescale = floor(prescaleval + 0.5);
   
  uint8_t oldmode = PCA9685_Custom_Read8(PCA9685_MODE1);
  uint8_t newmode = (oldmode&0x7F) | 0x10; // sleep
  PCA9685_Custom_Write8(PCA9685_MODE1, newmode); // go to sleep
  PCA9685_Custom_Write8(PCA9685_PRESCALE, prescale); // set the prescaler
  PCA9685_Custom_Write8(PCA9685_MODE1, oldmode);
  CyDelayUs(5);  
  PCA9685_Custom_Write8(PCA9685_MODE1, oldmode | 0xa1);  //  This sets the MODE1 register to turn on auto increment.
}

uint8_t PCA9685_Custom_Write8(uint8_t reg, uint8_t data){
    uint8_t Result;
    if((Result = I2C_1_MasterSendStart(I2C_SLAVE_ADDR, I2C_WRITE_XFER_MODE))) return Result;
    if((Result = I2C_1_MasterWriteByte(reg))) return Result;
    if((Result = I2C_1_MasterWriteByte(data))) return Result;
    if((Result = I2C_1_MasterSendStop())) return Result;
    return Result;
}

uint8_t PCA9685_Custom_Read8(uint8_t reg){
  uint8_t response = 0x00;
  I2C_1_MasterSendStart(I2C_SLAVE_ADDR, I2C_WRITE_XFER_MODE);
  I2C_1_MasterWriteByte(reg);
  I2C_1_MasterSendRestart(I2C_SLAVE_ADDR, I2C_READ_XFER_MODE);
  response =  I2C_1_MasterReadByte(I2C_NAK_DATA);
  I2C_1_MasterSendStop();

  return response; 
}
/* [] END OF FILE */
