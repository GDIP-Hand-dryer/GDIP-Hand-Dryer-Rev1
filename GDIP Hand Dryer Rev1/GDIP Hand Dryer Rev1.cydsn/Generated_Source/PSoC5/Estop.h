/*******************************************************************************
* File Name: Estop.h  
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

#if !defined(CY_PINS_Estop_H) /* Pins Estop_H */
#define CY_PINS_Estop_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Estop_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Estop__PORT == 15 && ((Estop__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Estop_Write(uint8 value);
void    Estop_SetDriveMode(uint8 mode);
uint8   Estop_ReadDataReg(void);
uint8   Estop_Read(void);
void    Estop_SetInterruptMode(uint16 position, uint16 mode);
uint8   Estop_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Estop_SetDriveMode() function.
     *  @{
     */
        #define Estop_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Estop_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Estop_DM_RES_UP          PIN_DM_RES_UP
        #define Estop_DM_RES_DWN         PIN_DM_RES_DWN
        #define Estop_DM_OD_LO           PIN_DM_OD_LO
        #define Estop_DM_OD_HI           PIN_DM_OD_HI
        #define Estop_DM_STRONG          PIN_DM_STRONG
        #define Estop_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Estop_MASK               Estop__MASK
#define Estop_SHIFT              Estop__SHIFT
#define Estop_WIDTH              1u

/* Interrupt constants */
#if defined(Estop__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Estop_SetInterruptMode() function.
     *  @{
     */
        #define Estop_INTR_NONE      (uint16)(0x0000u)
        #define Estop_INTR_RISING    (uint16)(0x0001u)
        #define Estop_INTR_FALLING   (uint16)(0x0002u)
        #define Estop_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Estop_INTR_MASK      (0x01u) 
#endif /* (Estop__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Estop_PS                     (* (reg8 *) Estop__PS)
/* Data Register */
#define Estop_DR                     (* (reg8 *) Estop__DR)
/* Port Number */
#define Estop_PRT_NUM                (* (reg8 *) Estop__PRT) 
/* Connect to Analog Globals */                                                  
#define Estop_AG                     (* (reg8 *) Estop__AG)                       
/* Analog MUX bux enable */
#define Estop_AMUX                   (* (reg8 *) Estop__AMUX) 
/* Bidirectional Enable */                                                        
#define Estop_BIE                    (* (reg8 *) Estop__BIE)
/* Bit-mask for Aliased Register Access */
#define Estop_BIT_MASK               (* (reg8 *) Estop__BIT_MASK)
/* Bypass Enable */
#define Estop_BYP                    (* (reg8 *) Estop__BYP)
/* Port wide control signals */                                                   
#define Estop_CTL                    (* (reg8 *) Estop__CTL)
/* Drive Modes */
#define Estop_DM0                    (* (reg8 *) Estop__DM0) 
#define Estop_DM1                    (* (reg8 *) Estop__DM1)
#define Estop_DM2                    (* (reg8 *) Estop__DM2) 
/* Input Buffer Disable Override */
#define Estop_INP_DIS                (* (reg8 *) Estop__INP_DIS)
/* LCD Common or Segment Drive */
#define Estop_LCD_COM_SEG            (* (reg8 *) Estop__LCD_COM_SEG)
/* Enable Segment LCD */
#define Estop_LCD_EN                 (* (reg8 *) Estop__LCD_EN)
/* Slew Rate Control */
#define Estop_SLW                    (* (reg8 *) Estop__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Estop_PRTDSI__CAPS_SEL       (* (reg8 *) Estop__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Estop_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Estop__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Estop_PRTDSI__OE_SEL0        (* (reg8 *) Estop__PRTDSI__OE_SEL0) 
#define Estop_PRTDSI__OE_SEL1        (* (reg8 *) Estop__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Estop_PRTDSI__OUT_SEL0       (* (reg8 *) Estop__PRTDSI__OUT_SEL0) 
#define Estop_PRTDSI__OUT_SEL1       (* (reg8 *) Estop__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Estop_PRTDSI__SYNC_OUT       (* (reg8 *) Estop__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Estop__SIO_CFG)
    #define Estop_SIO_HYST_EN        (* (reg8 *) Estop__SIO_HYST_EN)
    #define Estop_SIO_REG_HIFREQ     (* (reg8 *) Estop__SIO_REG_HIFREQ)
    #define Estop_SIO_CFG            (* (reg8 *) Estop__SIO_CFG)
    #define Estop_SIO_DIFF           (* (reg8 *) Estop__SIO_DIFF)
#endif /* (Estop__SIO_CFG) */

/* Interrupt Registers */
#if defined(Estop__INTSTAT)
    #define Estop_INTSTAT            (* (reg8 *) Estop__INTSTAT)
    #define Estop_SNAP               (* (reg8 *) Estop__SNAP)
    
	#define Estop_0_INTTYPE_REG 		(* (reg8 *) Estop__0__INTTYPE)
#endif /* (Estop__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Estop_H */


/* [] END OF FILE */
