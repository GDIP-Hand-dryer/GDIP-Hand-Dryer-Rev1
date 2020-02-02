/*******************************************************************************
* File Name: UPS_LBO.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_UPS_LBO_H) /* Pins UPS_LBO_H */
#define CY_PINS_UPS_LBO_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "UPS_LBO_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 UPS_LBO__PORT == 15 && ((UPS_LBO__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    UPS_LBO_Write(uint8 value);
void    UPS_LBO_SetDriveMode(uint8 mode);
uint8   UPS_LBO_ReadDataReg(void);
uint8   UPS_LBO_Read(void);
void    UPS_LBO_SetInterruptMode(uint16 position, uint16 mode);
uint8   UPS_LBO_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the UPS_LBO_SetDriveMode() function.
     *  @{
     */
        #define UPS_LBO_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define UPS_LBO_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define UPS_LBO_DM_RES_UP          PIN_DM_RES_UP
        #define UPS_LBO_DM_RES_DWN         PIN_DM_RES_DWN
        #define UPS_LBO_DM_OD_LO           PIN_DM_OD_LO
        #define UPS_LBO_DM_OD_HI           PIN_DM_OD_HI
        #define UPS_LBO_DM_STRONG          PIN_DM_STRONG
        #define UPS_LBO_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define UPS_LBO_MASK               UPS_LBO__MASK
#define UPS_LBO_SHIFT              UPS_LBO__SHIFT
#define UPS_LBO_WIDTH              1u

/* Interrupt constants */
#if defined(UPS_LBO__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in UPS_LBO_SetInterruptMode() function.
     *  @{
     */
        #define UPS_LBO_INTR_NONE      (uint16)(0x0000u)
        #define UPS_LBO_INTR_RISING    (uint16)(0x0001u)
        #define UPS_LBO_INTR_FALLING   (uint16)(0x0002u)
        #define UPS_LBO_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define UPS_LBO_INTR_MASK      (0x01u) 
#endif /* (UPS_LBO__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define UPS_LBO_PS                     (* (reg8 *) UPS_LBO__PS)
/* Data Register */
#define UPS_LBO_DR                     (* (reg8 *) UPS_LBO__DR)
/* Port Number */
#define UPS_LBO_PRT_NUM                (* (reg8 *) UPS_LBO__PRT) 
/* Connect to Analog Globals */                                                  
#define UPS_LBO_AG                     (* (reg8 *) UPS_LBO__AG)                       
/* Analog MUX bux enable */
#define UPS_LBO_AMUX                   (* (reg8 *) UPS_LBO__AMUX) 
/* Bidirectional Enable */                                                        
#define UPS_LBO_BIE                    (* (reg8 *) UPS_LBO__BIE)
/* Bit-mask for Aliased Register Access */
#define UPS_LBO_BIT_MASK               (* (reg8 *) UPS_LBO__BIT_MASK)
/* Bypass Enable */
#define UPS_LBO_BYP                    (* (reg8 *) UPS_LBO__BYP)
/* Port wide control signals */                                                   
#define UPS_LBO_CTL                    (* (reg8 *) UPS_LBO__CTL)
/* Drive Modes */
#define UPS_LBO_DM0                    (* (reg8 *) UPS_LBO__DM0) 
#define UPS_LBO_DM1                    (* (reg8 *) UPS_LBO__DM1)
#define UPS_LBO_DM2                    (* (reg8 *) UPS_LBO__DM2) 
/* Input Buffer Disable Override */
#define UPS_LBO_INP_DIS                (* (reg8 *) UPS_LBO__INP_DIS)
/* LCD Common or Segment Drive */
#define UPS_LBO_LCD_COM_SEG            (* (reg8 *) UPS_LBO__LCD_COM_SEG)
/* Enable Segment LCD */
#define UPS_LBO_LCD_EN                 (* (reg8 *) UPS_LBO__LCD_EN)
/* Slew Rate Control */
#define UPS_LBO_SLW                    (* (reg8 *) UPS_LBO__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define UPS_LBO_PRTDSI__CAPS_SEL       (* (reg8 *) UPS_LBO__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define UPS_LBO_PRTDSI__DBL_SYNC_IN    (* (reg8 *) UPS_LBO__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define UPS_LBO_PRTDSI__OE_SEL0        (* (reg8 *) UPS_LBO__PRTDSI__OE_SEL0) 
#define UPS_LBO_PRTDSI__OE_SEL1        (* (reg8 *) UPS_LBO__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define UPS_LBO_PRTDSI__OUT_SEL0       (* (reg8 *) UPS_LBO__PRTDSI__OUT_SEL0) 
#define UPS_LBO_PRTDSI__OUT_SEL1       (* (reg8 *) UPS_LBO__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define UPS_LBO_PRTDSI__SYNC_OUT       (* (reg8 *) UPS_LBO__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(UPS_LBO__SIO_CFG)
    #define UPS_LBO_SIO_HYST_EN        (* (reg8 *) UPS_LBO__SIO_HYST_EN)
    #define UPS_LBO_SIO_REG_HIFREQ     (* (reg8 *) UPS_LBO__SIO_REG_HIFREQ)
    #define UPS_LBO_SIO_CFG            (* (reg8 *) UPS_LBO__SIO_CFG)
    #define UPS_LBO_SIO_DIFF           (* (reg8 *) UPS_LBO__SIO_DIFF)
#endif /* (UPS_LBO__SIO_CFG) */

/* Interrupt Registers */
#if defined(UPS_LBO__INTSTAT)
    #define UPS_LBO_INTSTAT            (* (reg8 *) UPS_LBO__INTSTAT)
    #define UPS_LBO_SNAP               (* (reg8 *) UPS_LBO__SNAP)
    
	#define UPS_LBO_0_INTTYPE_REG 		(* (reg8 *) UPS_LBO__0__INTTYPE)
#endif /* (UPS_LBO__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_UPS_LBO_H */


/* [] END OF FILE */
