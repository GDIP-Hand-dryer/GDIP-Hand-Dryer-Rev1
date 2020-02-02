/*******************************************************************************
* File Name: SDCardSPI_SPI0_CS.c  
* Version 2.0
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "SDCardSPI_SPI0_CS.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 SDCardSPI_SPI0_CS__PORT == 15 && ((SDCardSPI_SPI0_CS__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: SDCardSPI_SPI0_CS_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None
*  
*******************************************************************************/
void SDCardSPI_SPI0_CS_Write(uint8 value) 
{
    uint8 staticBits = (SDCardSPI_SPI0_CS_DR & (uint8)(~SDCardSPI_SPI0_CS_MASK));
    SDCardSPI_SPI0_CS_DR = staticBits | ((uint8)(value << SDCardSPI_SPI0_CS_SHIFT) & SDCardSPI_SPI0_CS_MASK);
}


/*******************************************************************************
* Function Name: SDCardSPI_SPI0_CS_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  SDCardSPI_SPI0_CS_DM_STRONG     Strong Drive 
*  SDCardSPI_SPI0_CS_DM_OD_HI      Open Drain, Drives High 
*  SDCardSPI_SPI0_CS_DM_OD_LO      Open Drain, Drives Low 
*  SDCardSPI_SPI0_CS_DM_RES_UP     Resistive Pull Up 
*  SDCardSPI_SPI0_CS_DM_RES_DWN    Resistive Pull Down 
*  SDCardSPI_SPI0_CS_DM_RES_UPDWN  Resistive Pull Up/Down 
*  SDCardSPI_SPI0_CS_DM_DIG_HIZ    High Impedance Digital 
*  SDCardSPI_SPI0_CS_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void SDCardSPI_SPI0_CS_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(SDCardSPI_SPI0_CS_0, mode);
}


/*******************************************************************************
* Function Name: SDCardSPI_SPI0_CS_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro SDCardSPI_SPI0_CS_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 SDCardSPI_SPI0_CS_Read(void) 
{
    return (SDCardSPI_SPI0_CS_PS & SDCardSPI_SPI0_CS_MASK) >> SDCardSPI_SPI0_CS_SHIFT;
}


/*******************************************************************************
* Function Name: SDCardSPI_SPI0_CS_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 SDCardSPI_SPI0_CS_ReadDataReg(void) 
{
    return (SDCardSPI_SPI0_CS_DR & SDCardSPI_SPI0_CS_MASK) >> SDCardSPI_SPI0_CS_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(SDCardSPI_SPI0_CS_INTSTAT) 

    /*******************************************************************************
    * Function Name: SDCardSPI_SPI0_CS_ClearInterrupt
    ********************************************************************************
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 SDCardSPI_SPI0_CS_ClearInterrupt(void) 
    {
        return (SDCardSPI_SPI0_CS_INTSTAT & SDCardSPI_SPI0_CS_MASK) >> SDCardSPI_SPI0_CS_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
