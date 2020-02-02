/*******************************************************************************
* File Name: Detect_5V.h  
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

#if !defined(CY_PINS_Detect_5V_H) /* Pins Detect_5V_H */
#define CY_PINS_Detect_5V_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Detect_5V_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Detect_5V__PORT == 15 && ((Detect_5V__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Detect_5V_Write(uint8 value);
void    Detect_5V_SetDriveMode(uint8 mode);
uint8   Detect_5V_ReadDataReg(void);
uint8   Detect_5V_Read(void);
void    Detect_5V_SetInterruptMode(uint16 position, uint16 mode);
uint8   Detect_5V_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Detect_5V_SetDriveMode() function.
     *  @{
     */
        #define Detect_5V_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Detect_5V_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Detect_5V_DM_RES_UP          PIN_DM_RES_UP
        #define Detect_5V_DM_RES_DWN         PIN_DM_RES_DWN
        #define Detect_5V_DM_OD_LO           PIN_DM_OD_LO
        #define Detect_5V_DM_OD_HI           PIN_DM_OD_HI
        #define Detect_5V_DM_STRONG          PIN_DM_STRONG
        #define Detect_5V_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Detect_5V_MASK               Detect_5V__MASK
#define Detect_5V_SHIFT              Detect_5V__SHIFT
#define Detect_5V_WIDTH              1u

/* Interrupt constants */
#if defined(Detect_5V__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Detect_5V_SetInterruptMode() function.
     *  @{
     */
        #define Detect_5V_INTR_NONE      (uint16)(0x0000u)
        #define Detect_5V_INTR_RISING    (uint16)(0x0001u)
        #define Detect_5V_INTR_FALLING   (uint16)(0x0002u)
        #define Detect_5V_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Detect_5V_INTR_MASK      (0x01u) 
#endif /* (Detect_5V__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Detect_5V_PS                     (* (reg8 *) Detect_5V__PS)
/* Data Register */
#define Detect_5V_DR                     (* (reg8 *) Detect_5V__DR)
/* Port Number */
#define Detect_5V_PRT_NUM                (* (reg8 *) Detect_5V__PRT) 
/* Connect to Analog Globals */                                                  
#define Detect_5V_AG                     (* (reg8 *) Detect_5V__AG)                       
/* Analog MUX bux enable */
#define Detect_5V_AMUX                   (* (reg8 *) Detect_5V__AMUX) 
/* Bidirectional Enable */                                                        
#define Detect_5V_BIE                    (* (reg8 *) Detect_5V__BIE)
/* Bit-mask for Aliased Register Access */
#define Detect_5V_BIT_MASK               (* (reg8 *) Detect_5V__BIT_MASK)
/* Bypass Enable */
#define Detect_5V_BYP                    (* (reg8 *) Detect_5V__BYP)
/* Port wide control signals */                                                   
#define Detect_5V_CTL                    (* (reg8 *) Detect_5V__CTL)
/* Drive Modes */
#define Detect_5V_DM0                    (* (reg8 *) Detect_5V__DM0) 
#define Detect_5V_DM1                    (* (reg8 *) Detect_5V__DM1)
#define Detect_5V_DM2                    (* (reg8 *) Detect_5V__DM2) 
/* Input Buffer Disable Override */
#define Detect_5V_INP_DIS                (* (reg8 *) Detect_5V__INP_DIS)
/* LCD Common or Segment Drive */
#define Detect_5V_LCD_COM_SEG            (* (reg8 *) Detect_5V__LCD_COM_SEG)
/* Enable Segment LCD */
#define Detect_5V_LCD_EN                 (* (reg8 *) Detect_5V__LCD_EN)
/* Slew Rate Control */
#define Detect_5V_SLW                    (* (reg8 *) Detect_5V__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Detect_5V_PRTDSI__CAPS_SEL       (* (reg8 *) Detect_5V__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Detect_5V_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Detect_5V__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Detect_5V_PRTDSI__OE_SEL0        (* (reg8 *) Detect_5V__PRTDSI__OE_SEL0) 
#define Detect_5V_PRTDSI__OE_SEL1        (* (reg8 *) Detect_5V__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Detect_5V_PRTDSI__OUT_SEL0       (* (reg8 *) Detect_5V__PRTDSI__OUT_SEL0) 
#define Detect_5V_PRTDSI__OUT_SEL1       (* (reg8 *) Detect_5V__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Detect_5V_PRTDSI__SYNC_OUT       (* (reg8 *) Detect_5V__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Detect_5V__SIO_CFG)
    #define Detect_5V_SIO_HYST_EN        (* (reg8 *) Detect_5V__SIO_HYST_EN)
    #define Detect_5V_SIO_REG_HIFREQ     (* (reg8 *) Detect_5V__SIO_REG_HIFREQ)
    #define Detect_5V_SIO_CFG            (* (reg8 *) Detect_5V__SIO_CFG)
    #define Detect_5V_SIO_DIFF           (* (reg8 *) Detect_5V__SIO_DIFF)
#endif /* (Detect_5V__SIO_CFG) */

/* Interrupt Registers */
#if defined(Detect_5V__INTSTAT)
    #define Detect_5V_INTSTAT            (* (reg8 *) Detect_5V__INTSTAT)
    #define Detect_5V_SNAP               (* (reg8 *) Detect_5V__SNAP)
    
	#define Detect_5V_0_INTTYPE_REG 		(* (reg8 *) Detect_5V__0__INTTYPE)
#endif /* (Detect_5V__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Detect_5V_H */


/* [] END OF FILE */
