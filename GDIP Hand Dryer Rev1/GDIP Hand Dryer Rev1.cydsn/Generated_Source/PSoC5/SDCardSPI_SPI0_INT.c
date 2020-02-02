/*******************************************************************************
* File Name: SDCardSPI_SPI0_INT.c
* Version 2.40
*
* Description:
*  This file provides all Interrupt Service Routine (ISR) for the SPI Master
*  component.
*
* Note:
*  None.
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SDCardSPI_SPI0_PVT.h"

/* User code required at start of ISR */
/* `#START SDCardSPI_SPI0_ISR_START_DEF` */

/* `#END` */


/*******************************************************************************
* Function Name: SDCardSPI_SPI0_TX_ISR
********************************************************************************
*
* Summary:
*  Interrupt Service Routine for TX portion of the SPI Master.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  SDCardSPI_SPI0_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer.
*  SDCardSPI_SPI0_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer, modified when exist data to
*  sending and FIFO Not Full.
*  SDCardSPI_SPI0_txBuffer[SDCardSPI_SPI0_TX_BUFFER_SIZE] - used to store
*  data to sending.
*  All described above Global variables are used when Software Buffer is used.
*
*******************************************************************************/
CY_ISR(SDCardSPI_SPI0_TX_ISR)
{
    #if(SDCardSPI_SPI0_TX_SOFTWARE_BUF_ENABLED)
        uint8 tmpStatus;
    #endif /* (SDCardSPI_SPI0_TX_SOFTWARE_BUF_ENABLED) */

    /* User code required at start of ISR */
    /* `#START SDCardSPI_SPI0_TX_ISR_START` */

    /* `#END` */

    #if(SDCardSPI_SPI0_TX_SOFTWARE_BUF_ENABLED)
        /* Check if TX data buffer is not empty and there is space in TX FIFO */
        while(SDCardSPI_SPI0_txBufferRead != SDCardSPI_SPI0_txBufferWrite)
        {
            tmpStatus = SDCardSPI_SPI0_GET_STATUS_TX(SDCardSPI_SPI0_swStatusTx);
            SDCardSPI_SPI0_swStatusTx = tmpStatus;

            if(0u != (SDCardSPI_SPI0_swStatusTx & SDCardSPI_SPI0_STS_TX_FIFO_NOT_FULL))
            {
                if(0u == SDCardSPI_SPI0_txBufferFull)
                {
                   SDCardSPI_SPI0_txBufferRead++;

                    if(SDCardSPI_SPI0_txBufferRead >= SDCardSPI_SPI0_TX_BUFFER_SIZE)
                    {
                        SDCardSPI_SPI0_txBufferRead = 0u;
                    }
                }
                else
                {
                    SDCardSPI_SPI0_txBufferFull = 0u;
                }

                /* Move data from the Buffer to the FIFO */
                CY_SET_REG8(SDCardSPI_SPI0_TXDATA_PTR,
                    SDCardSPI_SPI0_txBuffer[SDCardSPI_SPI0_txBufferRead]);
            }
            else
            {
                break;
            }
        }

        if(SDCardSPI_SPI0_txBufferRead == SDCardSPI_SPI0_txBufferWrite)
        {
            /* TX Buffer is EMPTY: disable interrupt on TX NOT FULL */
            SDCardSPI_SPI0_TX_STATUS_MASK_REG &= ((uint8) ~SDCardSPI_SPI0_STS_TX_FIFO_NOT_FULL);
        }

    #endif /* (SDCardSPI_SPI0_TX_SOFTWARE_BUF_ENABLED) */

    /* User code required at end of ISR (Optional) */
    /* `#START SDCardSPI_SPI0_TX_ISR_END` */

    /* `#END` */
}


/*******************************************************************************
* Function Name: SDCardSPI_SPI0_RX_ISR
********************************************************************************
*
* Summary:
*  Interrupt Service Routine for RX portion of the SPI Master.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  SDCardSPI_SPI0_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer modified when FIFO contains
*  new data.
*  SDCardSPI_SPI0_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer, modified when overflow occurred.
*  SDCardSPI_SPI0_rxBuffer[SDCardSPI_SPI0_RX_BUFFER_SIZE] - used to store
*  received data, modified when FIFO contains new data.
*  All described above Global variables are used when Software Buffer is used.
*
*******************************************************************************/
CY_ISR(SDCardSPI_SPI0_RX_ISR)
{
    #if(SDCardSPI_SPI0_RX_SOFTWARE_BUF_ENABLED)
        uint8 tmpStatus;
        uint8 rxData;
    #endif /* (SDCardSPI_SPI0_RX_SOFTWARE_BUF_ENABLED) */

    /* User code required at start of ISR */
    /* `#START SDCardSPI_SPI0_RX_ISR_START` */

    /* `#END` */

    #if(SDCardSPI_SPI0_RX_SOFTWARE_BUF_ENABLED)

        tmpStatus = SDCardSPI_SPI0_GET_STATUS_RX(SDCardSPI_SPI0_swStatusRx);
        SDCardSPI_SPI0_swStatusRx = tmpStatus;

        /* Check if RX data FIFO has some data to be moved into the RX Buffer */
        while(0u != (SDCardSPI_SPI0_swStatusRx & SDCardSPI_SPI0_STS_RX_FIFO_NOT_EMPTY))
        {
            rxData = CY_GET_REG8(SDCardSPI_SPI0_RXDATA_PTR);

            /* Set next pointer. */
            SDCardSPI_SPI0_rxBufferWrite++;
            if(SDCardSPI_SPI0_rxBufferWrite >= SDCardSPI_SPI0_RX_BUFFER_SIZE)
            {
                SDCardSPI_SPI0_rxBufferWrite = 0u;
            }

            if(SDCardSPI_SPI0_rxBufferWrite == SDCardSPI_SPI0_rxBufferRead)
            {
                SDCardSPI_SPI0_rxBufferRead++;
                if(SDCardSPI_SPI0_rxBufferRead >= SDCardSPI_SPI0_RX_BUFFER_SIZE)
                {
                    SDCardSPI_SPI0_rxBufferRead = 0u;
                }

                SDCardSPI_SPI0_rxBufferFull = 1u;
            }

            /* Move data from the FIFO to the Buffer */
            SDCardSPI_SPI0_rxBuffer[SDCardSPI_SPI0_rxBufferWrite] = rxData;

            tmpStatus = SDCardSPI_SPI0_GET_STATUS_RX(SDCardSPI_SPI0_swStatusRx);
            SDCardSPI_SPI0_swStatusRx = tmpStatus;
        }

    #endif /* (SDCardSPI_SPI0_RX_SOFTWARE_BUF_ENABLED) */

    /* User code required at end of ISR (Optional) */
    /* `#START SDCardSPI_SPI0_RX_ISR_END` */

    /* `#END` */
}

/* [] END OF FILE */
