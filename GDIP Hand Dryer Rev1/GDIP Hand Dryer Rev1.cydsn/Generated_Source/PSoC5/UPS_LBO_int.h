/*******************************************************************************
* File Name: UPS_LBO_int.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_UPS_LBO_int_H)
#define CY_ISR_UPS_LBO_int_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void UPS_LBO_int_Start(void);
void UPS_LBO_int_StartEx(cyisraddress address);
void UPS_LBO_int_Stop(void);

CY_ISR_PROTO(UPS_LBO_int_Interrupt);

void UPS_LBO_int_SetVector(cyisraddress address);
cyisraddress UPS_LBO_int_GetVector(void);

void UPS_LBO_int_SetPriority(uint8 priority);
uint8 UPS_LBO_int_GetPriority(void);

void UPS_LBO_int_Enable(void);
uint8 UPS_LBO_int_GetState(void);
void UPS_LBO_int_Disable(void);

void UPS_LBO_int_SetPending(void);
void UPS_LBO_int_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the UPS_LBO_int ISR. */
#define UPS_LBO_int_INTC_VECTOR            ((reg32 *) UPS_LBO_int__INTC_VECT)

/* Address of the UPS_LBO_int ISR priority. */
#define UPS_LBO_int_INTC_PRIOR             ((reg8 *) UPS_LBO_int__INTC_PRIOR_REG)

/* Priority of the UPS_LBO_int interrupt. */
#define UPS_LBO_int_INTC_PRIOR_NUMBER      UPS_LBO_int__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable UPS_LBO_int interrupt. */
#define UPS_LBO_int_INTC_SET_EN            ((reg32 *) UPS_LBO_int__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the UPS_LBO_int interrupt. */
#define UPS_LBO_int_INTC_CLR_EN            ((reg32 *) UPS_LBO_int__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the UPS_LBO_int interrupt state to pending. */
#define UPS_LBO_int_INTC_SET_PD            ((reg32 *) UPS_LBO_int__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the UPS_LBO_int interrupt. */
#define UPS_LBO_int_INTC_CLR_PD            ((reg32 *) UPS_LBO_int__INTC_CLR_PD_REG)


#endif /* CY_ISR_UPS_LBO_int_H */


/* [] END OF FILE */
