/*******************************************************************************
* File Name: SDCardSPI_PM.c
* Version 1.20
*
* Description:
*  This file provides the API source code for Power Management of the emFile
*  component.
*
* Note:
*
*******************************************************************************
* Copyright 2011-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include <project.h>
#include "SDCardSPI.h"


/*******************************************************************************
* Function Name: SDCardSPI_SaveConfig
********************************************************************************
*
* Summary:
*  Saves SPI Masters configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void SDCardSPI_SaveConfig(void) 
{
    #if(SDCardSPI_NUMBER_SD_CARDS == 4u)
        SDCardSPI_SPI0_SaveConfig();
        SDCardSPI_SPI1_SaveConfig();
        SDCardSPI_SPI2_SaveConfig();
        SDCardSPI_SPI3_SaveConfig();
    #elif(SDCardSPI_NUMBER_SD_CARDS == 3u)
        SDCardSPI_SPI0_SaveConfig();
        SDCardSPI_SPI1_SaveConfig();
        SDCardSPI_SPI2_SaveConfig();
    #elif(SDCardSPI_NUMBER_SD_CARDS == 2u)
        SDCardSPI_SPI0_SaveConfig();
        SDCardSPI_SPI1_SaveConfig();
    #else
        SDCardSPI_SPI0_SaveConfig();
    #endif /* (SDCardSPI_NUMBER_SD_CARDS == 4u) */
}


/*******************************************************************************
* Function Name: SDCardSPI_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores SPI Masters configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  If this API is called without first calling SaveConfig then in the following
*  registers will be default values from Customizer:
*
*******************************************************************************/
void SDCardSPI_RestoreConfig(void) 
{
    #if(SDCardSPI_NUMBER_SD_CARDS == 4u)
        SDCardSPI_SPI0_RestoreConfig();
        SDCardSPI_SPI1_RestoreConfig();
        SDCardSPI_SPI2_RestoreConfig();
        SDCardSPI_SPI3_RestoreConfig();
    #elif(SDCardSPI_NUMBER_SD_CARDS == 3u)
        SDCardSPI_SPI0_SaveConfig();
        SDCardSPI_SPI1_SaveConfig();
        SDCardSPI_SPI2_SaveConfig();
    #elif(SDCardSPI_NUMBER_SD_CARDS == 2u)
        SDCardSPI_SPI0_SaveConfig();
        SDCardSPI_SPI1_SaveConfig();
    #else
        SDCardSPI_SPI0_SaveConfig();
    #endif /* (SDCardSPI_NUMBER_SD_CARDS == 4u) */
}


/*******************************************************************************
* Function Name: SDCardSPI_Sleep
********************************************************************************
*
* Summary:
*  Prepare emFile to go to sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void SDCardSPI_Sleep(void) 
{
    #if(SDCardSPI_NUMBER_SD_CARDS == 4u)
        SDCardSPI_SPI0_Sleep();
        SDCardSPI_SPI1_Sleep();
        SDCardSPI_SPI2_Sleep();
        SDCardSPI_SPI3_Sleep();
    #elif(SDCardSPI_NUMBER_SD_CARDS == 3u)
        SDCardSPI_SPI0_Sleep();
        SDCardSPI_SPI1_Sleep();
        SDCardSPI_SPI2_Sleep();
    #elif(SDCardSPI_NUMBER_SD_CARDS == 2u)
        SDCardSPI_SPI0_Sleep();
        SDCardSPI_SPI1_Sleep();
    #else
        SDCardSPI_SPI0_Sleep();
    #endif /* (SDCardSPI_NUMBER_SD_CARDS == 4u) */
}


/*******************************************************************************
* Function Name: SDCardSPI_Wakeup
********************************************************************************
*
* Summary:
*  Prepare SPIM Components to wake up.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void SDCardSPI_Wakeup(void) 
{
    #if(SDCardSPI_NUMBER_SD_CARDS == 4u)
        SDCardSPI_SPI0_Wakeup();
        SDCardSPI_SPI1_Wakeup();
        SDCardSPI_SPI2_Wakeup();
        SDCardSPI_SPI3_Wakeup();
    #elif(SDCardSPI_NUMBER_SD_CARDS == 3u)
        SDCardSPI_SPI0_Wakeup();
        SDCardSPI_SPI1_Wakeup();
        SDCardSPI_SPI2_Wakeup();
    #elif(SDCardSPI_NUMBER_SD_CARDS == 2u)
        SDCardSPI_SPI0_Wakeup();
        SDCardSPI_SPI1_Wakeup();
    #else
        SDCardSPI_SPI0_Wakeup();
    #endif /* (SDCardSPI_NUMBER_SD_CARDS == 4u) */
}


/* [] END OF FILE */
