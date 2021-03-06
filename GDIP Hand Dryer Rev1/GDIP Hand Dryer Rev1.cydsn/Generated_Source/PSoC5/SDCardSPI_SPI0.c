/*******************************************************************************
* File Name: SDCardSPI_SPI0.c
* Version 2.40
*
* Description:
*  This file provides all API functionality of the SPI Master component.
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

#if(SDCardSPI_SPI0_TX_SOFTWARE_BUF_ENABLED)
    volatile uint8 SDCardSPI_SPI0_txBuffer[SDCardSPI_SPI0_TX_BUFFER_SIZE] = {0u};
    volatile uint8 SDCardSPI_SPI0_txBufferFull;
    volatile uint8 SDCardSPI_SPI0_txBufferRead;
    volatile uint8 SDCardSPI_SPI0_txBufferWrite;
#endif /* (SDCardSPI_SPI0_TX_SOFTWARE_BUF_ENABLED) */

#if(SDCardSPI_SPI0_RX_SOFTWARE_BUF_ENABLED)
    volatile uint8 SDCardSPI_SPI0_rxBuffer[SDCardSPI_SPI0_RX_BUFFER_SIZE] = {0u};
    volatile uint8 SDCardSPI_SPI0_rxBufferFull;
    volatile uint8 SDCardSPI_SPI0_rxBufferRead;
    volatile uint8 SDCardSPI_SPI0_rxBufferWrite;
#endif /* (SDCardSPI_SPI0_RX_SOFTWARE_BUF_ENABLED) */

uint8 SDCardSPI_SPI0_initVar = 0u;

volatile uint8 SDCardSPI_SPI0_swStatusTx;
volatile uint8 SDCardSPI_SPI0_swStatusRx;


/*******************************************************************************
* Function Name: SDCardSPI_SPI0_Init
********************************************************************************
*
* Summary:
*  Inits/Restores default SPIM configuration provided with customizer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  When this function is called it initializes all of the necessary parameters
*  for execution. i.e. setting the initial interrupt mask, configuring the
*  interrupt service routine, configuring the bit-counter parameters and
*  clearing the FIFO and Status Register.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SDCardSPI_SPI0_Init(void) 
{
    /* Initialize the Bit counter */
    SDCardSPI_SPI0_COUNTER_PERIOD_REG = SDCardSPI_SPI0_BITCTR_INIT;

    /* Init TX ISR  */
    #if(0u != SDCardSPI_SPI0_INTERNAL_TX_INT_ENABLED)
        CyIntDisable         (SDCardSPI_SPI0_TX_ISR_NUMBER);
        CyIntSetPriority     (SDCardSPI_SPI0_TX_ISR_NUMBER,  SDCardSPI_SPI0_TX_ISR_PRIORITY);
        (void) CyIntSetVector(SDCardSPI_SPI0_TX_ISR_NUMBER, &SDCardSPI_SPI0_TX_ISR);
    #endif /* (0u != SDCardSPI_SPI0_INTERNAL_TX_INT_ENABLED) */

    /* Init RX ISR  */
    #if(0u != SDCardSPI_SPI0_INTERNAL_RX_INT_ENABLED)
        CyIntDisable         (SDCardSPI_SPI0_RX_ISR_NUMBER);
        CyIntSetPriority     (SDCardSPI_SPI0_RX_ISR_NUMBER,  SDCardSPI_SPI0_RX_ISR_PRIORITY);
        (void) CyIntSetVector(SDCardSPI_SPI0_RX_ISR_NUMBER, &SDCardSPI_SPI0_RX_ISR);
    #endif /* (0u != SDCardSPI_SPI0_INTERNAL_RX_INT_ENABLED) */

    /* Clear any stray data from the RX and TX FIFO */
    SDCardSPI_SPI0_ClearFIFO();

    #if(SDCardSPI_SPI0_RX_SOFTWARE_BUF_ENABLED)
        SDCardSPI_SPI0_rxBufferFull  = 0u;
        SDCardSPI_SPI0_rxBufferRead  = 0u;
        SDCardSPI_SPI0_rxBufferWrite = 0u;
    #endif /* (SDCardSPI_SPI0_RX_SOFTWARE_BUF_ENABLED) */

    #if(SDCardSPI_SPI0_TX_SOFTWARE_BUF_ENABLED)
        SDCardSPI_SPI0_txBufferFull  = 0u;
        SDCardSPI_SPI0_txBufferRead  = 0u;
        SDCardSPI_SPI0_txBufferWrite = 0u;
    #endif /* (SDCardSPI_SPI0_TX_SOFTWARE_BUF_ENABLED) */

    (void) SDCardSPI_SPI0_ReadTxStatus(); /* Clear Tx status and swStatusTx */
    (void) SDCardSPI_SPI0_ReadRxStatus(); /* Clear Rx status and swStatusRx */

    /* Configure TX and RX interrupt mask */
    SDCardSPI_SPI0_TX_STATUS_MASK_REG = SDCardSPI_SPI0_TX_INIT_INTERRUPTS_MASK;
    SDCardSPI_SPI0_RX_STATUS_MASK_REG = SDCardSPI_SPI0_RX_INIT_INTERRUPTS_MASK;
}


/*******************************************************************************
* Function Name: SDCardSPI_SPI0_Enable
********************************************************************************
*
* Summary:
*  Enable SPIM component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void SDCardSPI_SPI0_Enable(void) 
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    SDCardSPI_SPI0_COUNTER_CONTROL_REG |= SDCardSPI_SPI0_CNTR_ENABLE;
    SDCardSPI_SPI0_TX_STATUS_ACTL_REG  |= SDCardSPI_SPI0_INT_ENABLE;
    SDCardSPI_SPI0_RX_STATUS_ACTL_REG  |= SDCardSPI_SPI0_INT_ENABLE;
    CyExitCriticalSection(enableInterrupts);

    #if(0u != SDCardSPI_SPI0_INTERNAL_CLOCK)
        SDCardSPI_SPI0_IntClock_Enable();
    #endif /* (0u != SDCardSPI_SPI0_INTERNAL_CLOCK) */

    SDCardSPI_SPI0_EnableTxInt();
    SDCardSPI_SPI0_EnableRxInt();
}


/*******************************************************************************
* Function Name: SDCardSPI_SPI0_Start
********************************************************************************
*
* Summary:
*  Initialize and Enable the SPI Master component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  SDCardSPI_SPI0_initVar - used to check initial configuration, modified on
*  first function call.
*
* Theory:
*  Enable the clock input to enable operation.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SDCardSPI_SPI0_Start(void) 
{
    if(0u == SDCardSPI_SPI0_initVar)
    {
        SDCardSPI_SPI0_Init();
        SDCardSPI_SPI0_initVar = 1u;
    }

    SDCardSPI_SPI0_Enable();
}


/*******************************************************************************
* Function Name: SDCardSPI_SPI0_Stop
********************************************************************************
*
* Summary:
*  Disable the SPI Master component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the clock input to enable operation.
*
*******************************************************************************/
void SDCardSPI_SPI0_Stop(void) 
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    SDCardSPI_SPI0_TX_STATUS_ACTL_REG &= ((uint8) ~SDCardSPI_SPI0_INT_ENABLE);
    SDCardSPI_SPI0_RX_STATUS_ACTL_REG &= ((uint8) ~SDCardSPI_SPI0_INT_ENABLE);
    CyExitCriticalSection(enableInterrupts);

    #if(0u != SDCardSPI_SPI0_INTERNAL_CLOCK)
        SDCardSPI_SPI0_IntClock_Disable();
    #endif /* (0u != SDCardSPI_SPI0_INTERNAL_CLOCK) */

    SDCardSPI_SPI0_DisableTxInt();
    SDCardSPI_SPI0_DisableRxInt();
}


/*******************************************************************************
* Function Name: SDCardSPI_SPI0_EnableTxInt
********************************************************************************
*
* Summary:
*  Enable internal Tx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Enable the internal Tx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void SDCardSPI_SPI0_EnableTxInt(void) 
{
    #if(0u != SDCardSPI_SPI0_INTERNAL_TX_INT_ENABLED)
        CyIntEnable(SDCardSPI_SPI0_TX_ISR_NUMBER);
    #endif /* (0u != SDCardSPI_SPI0_INTERNAL_TX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: SDCardSPI_SPI0_EnableRxInt
********************************************************************************
*
* Summary:
*  Enable internal Rx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Enable the internal Rx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void SDCardSPI_SPI0_EnableRxInt(void) 
{
    #if(0u != SDCardSPI_SPI0_INTERNAL_RX_INT_ENABLED)
        CyIntEnable(SDCardSPI_SPI0_RX_ISR_NUMBER);
    #endif /* (0u != SDCardSPI_SPI0_INTERNAL_RX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: SDCardSPI_SPI0_DisableTxInt
********************************************************************************
*
* Summary:
*  Disable internal Tx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the internal Tx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void SDCardSPI_SPI0_DisableTxInt(void) 
{
    #if(0u != SDCardSPI_SPI0_INTERNAL_TX_INT_ENABLED)
        CyIntDisable(SDCardSPI_SPI0_TX_ISR_NUMBER);
    #endif /* (0u != SDCardSPI_SPI0_INTERNAL_TX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: SDCardSPI_SPI0_DisableRxInt
********************************************************************************
*
* Summary:
*  Disable internal Rx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the internal Rx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void SDCardSPI_SPI0_DisableRxInt(void) 
{
    #if(0u != SDCardSPI_SPI0_INTERNAL_RX_INT_ENABLED)
        CyIntDisable(SDCardSPI_SPI0_RX_ISR_NUMBER);
    #endif /* (0u != SDCardSPI_SPI0_INTERNAL_RX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: SDCardSPI_SPI0_SetTxInterruptMode
********************************************************************************
*
* Summary:
*  Configure which status bits trigger an interrupt event.
*
* Parameters:
*  intSrc: An or'd combination of the desired status bit masks (defined in the
*  header file).
*
* Return:
*  None.
*
* Theory:
*  Enables the output of specific status bits to the interrupt controller.
*
*******************************************************************************/
void SDCardSPI_SPI0_SetTxInterruptMode(uint8 intSrc) 
{
    SDCardSPI_SPI0_TX_STATUS_MASK_REG = intSrc;
}


/*******************************************************************************
* Function Name: SDCardSPI_SPI0_SetRxInterruptMode
********************************************************************************
*
* Summary:
*  Configure which status bits trigger an interrupt event.
*
* Parameters:
*  intSrc: An or'd combination of the desired status bit masks (defined in the
*  header file).
*
* Return:
*  None.
*
* Theory:
*  Enables the output of specific status bits to the interrupt controller.
*
*******************************************************************************/
void SDCardSPI_SPI0_SetRxInterruptMode(uint8 intSrc) 
{
    SDCardSPI_SPI0_RX_STATUS_MASK_REG  = intSrc;
}


/*******************************************************************************
* Function Name: SDCardSPI_SPI0_ReadTxStatus
********************************************************************************
*
* Summary:
*  Read the Tx status register for the component.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the Tx status register.
*
* Global variables:
*  SDCardSPI_SPI0_swStatusTx - used to store in software status register,
*  modified every function call - resets to zero.
*
* Theory:
*  Allows the user and the API to read the Tx status register for error
*  detection and flow control.
*
* Side Effects:
*  Clear Tx status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 SDCardSPI_SPI0_ReadTxStatus(void) 
{
    uint8 tmpStatus;

    #if(SDCardSPI_SPI0_TX_SOFTWARE_BUF_ENABLED)
        /* Disable TX interrupt to protect global veriables */
        SDCardSPI_SPI0_DisableTxInt();

        tmpStatus = SDCardSPI_SPI0_GET_STATUS_TX(SDCardSPI_SPI0_swStatusTx);
        SDCardSPI_SPI0_swStatusTx = 0u;

        SDCardSPI_SPI0_EnableTxInt();

    #else

        tmpStatus = SDCardSPI_SPI0_TX_STATUS_REG;

    #endif /* (SDCardSPI_SPI0_TX_SOFTWARE_BUF_ENABLED) */

    return(tmpStatus);
}


/*******************************************************************************
* Function Name: SDCardSPI_SPI0_ReadRxStatus
********************************************************************************
*
* Summary:
*  Read the Rx status register for the component.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the Rx status register.
*
* Global variables:
*  SDCardSPI_SPI0_swStatusRx - used to store in software Rx status register,
*  modified every function call - resets to zero.
*
* Theory:
*  Allows the user and the API to read the Rx status register for error
*  detection and flow control.
*
* Side Effects:
*  Clear Rx status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 SDCardSPI_SPI0_ReadRxStatus(void) 
{
    uint8 tmpStatus;

    #if(SDCardSPI_SPI0_RX_SOFTWARE_BUF_ENABLED)
        /* Disable RX interrupt to protect global veriables */
        SDCardSPI_SPI0_DisableRxInt();

        tmpStatus = SDCardSPI_SPI0_GET_STATUS_RX(SDCardSPI_SPI0_swStatusRx);
        SDCardSPI_SPI0_swStatusRx = 0u;

        SDCardSPI_SPI0_EnableRxInt();

    #else

        tmpStatus = SDCardSPI_SPI0_RX_STATUS_REG;

    #endif /* (SDCardSPI_SPI0_RX_SOFTWARE_BUF_ENABLED) */

    return(tmpStatus);
}


/*******************************************************************************
* Function Name: SDCardSPI_SPI0_WriteTxData
********************************************************************************
*
* Summary:
*  Write a byte of data to be sent across the SPI.
*
* Parameters:
*  txDataByte: The data value to send across the SPI.
*
* Return:
*  None.
*
* Global variables:
*  SDCardSPI_SPI0_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer, modified every function
*  call if TX Software Buffer is used.
*  SDCardSPI_SPI0_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer.
*  SDCardSPI_SPI0_txBuffer[SDCardSPI_SPI0_TX_BUFFER_SIZE] - used to store
*  data to sending, modified every function call if TX Software Buffer is used.
*
* Theory:
*  Allows the user to transmit any byte of data in a single transfer.
*
* Side Effects:
*  If this function is called again before the previous byte is finished then
*  the next byte will be appended to the transfer with no time between
*  the byte transfers. Clear Tx status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SDCardSPI_SPI0_WriteTxData(uint8 txData) 
{
    #if(SDCardSPI_SPI0_TX_SOFTWARE_BUF_ENABLED)

        uint8 tempStatus;
        uint8 tmpTxBufferRead;

        /* Block if TX buffer is FULL: don't overwrite */
        do
        {
            tmpTxBufferRead = SDCardSPI_SPI0_txBufferRead;
            if(0u == tmpTxBufferRead)
            {
                tmpTxBufferRead = (SDCardSPI_SPI0_TX_BUFFER_SIZE - 1u);
            }
            else
            {
                tmpTxBufferRead--;
            }

        }while(tmpTxBufferRead == SDCardSPI_SPI0_txBufferWrite);

        /* Disable TX interrupt to protect global veriables */
        SDCardSPI_SPI0_DisableTxInt();

        tempStatus = SDCardSPI_SPI0_GET_STATUS_TX(SDCardSPI_SPI0_swStatusTx);
        SDCardSPI_SPI0_swStatusTx = tempStatus;


        if((SDCardSPI_SPI0_txBufferRead == SDCardSPI_SPI0_txBufferWrite) &&
           (0u != (SDCardSPI_SPI0_swStatusTx & SDCardSPI_SPI0_STS_TX_FIFO_NOT_FULL)))
        {
            /* Add directly to the TX FIFO */
            CY_SET_REG8(SDCardSPI_SPI0_TXDATA_PTR, txData);
        }
        else
        {
            /* Add to the TX software buffer */
            SDCardSPI_SPI0_txBufferWrite++;
            if(SDCardSPI_SPI0_txBufferWrite >= SDCardSPI_SPI0_TX_BUFFER_SIZE)
            {
                SDCardSPI_SPI0_txBufferWrite = 0u;
            }

            if(SDCardSPI_SPI0_txBufferWrite == SDCardSPI_SPI0_txBufferRead)
            {
                SDCardSPI_SPI0_txBufferRead++;
                if(SDCardSPI_SPI0_txBufferRead >= SDCardSPI_SPI0_TX_BUFFER_SIZE)
                {
                    SDCardSPI_SPI0_txBufferRead = 0u;
                }
                SDCardSPI_SPI0_txBufferFull = 1u;
            }

            SDCardSPI_SPI0_txBuffer[SDCardSPI_SPI0_txBufferWrite] = txData;

            SDCardSPI_SPI0_TX_STATUS_MASK_REG |= SDCardSPI_SPI0_STS_TX_FIFO_NOT_FULL;
        }

        SDCardSPI_SPI0_EnableTxInt();

    #else

        while(0u == (SDCardSPI_SPI0_TX_STATUS_REG & SDCardSPI_SPI0_STS_TX_FIFO_NOT_FULL))
        {
            ; /* Wait for room in FIFO */
        }

        /* Put byte in TX FIFO */
        CY_SET_REG8(SDCardSPI_SPI0_TXDATA_PTR, txData);

    #endif /* (SDCardSPI_SPI0_TX_SOFTWARE_BUF_ENABLED) */
}


/*******************************************************************************
* Function Name: SDCardSPI_SPI0_ReadRxData
********************************************************************************
*
* Summary:
*  Read the next byte of data received across the SPI.
*
* Parameters:
*  None.
*
* Return:
*  The next byte of data read from the FIFO.
*
* Global variables:
*  SDCardSPI_SPI0_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer.
*  SDCardSPI_SPI0_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer, modified every function
*  call if RX Software Buffer is used.
*  SDCardSPI_SPI0_rxBuffer[SDCardSPI_SPI0_RX_BUFFER_SIZE] - used to store
*  received data.
*
* Theory:
*  Allows the user to read a byte of data received.
*
* Side Effects:
*  Will return invalid data if the FIFO is empty. The user should Call
*  GetRxBufferSize() and if it returns a non-zero value then it is safe to call
*  ReadByte() function.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 SDCardSPI_SPI0_ReadRxData(void) 
{
    uint8 rxData;

    #if(SDCardSPI_SPI0_RX_SOFTWARE_BUF_ENABLED)

        /* Disable RX interrupt to protect global veriables */
        SDCardSPI_SPI0_DisableRxInt();

        if(SDCardSPI_SPI0_rxBufferRead != SDCardSPI_SPI0_rxBufferWrite)
        {
            if(0u == SDCardSPI_SPI0_rxBufferFull)
            {
                SDCardSPI_SPI0_rxBufferRead++;
                if(SDCardSPI_SPI0_rxBufferRead >= SDCardSPI_SPI0_RX_BUFFER_SIZE)
                {
                    SDCardSPI_SPI0_rxBufferRead = 0u;
                }
            }
            else
            {
                SDCardSPI_SPI0_rxBufferFull = 0u;
            }
        }

        rxData = SDCardSPI_SPI0_rxBuffer[SDCardSPI_SPI0_rxBufferRead];

        SDCardSPI_SPI0_EnableRxInt();

    #else

        rxData = CY_GET_REG8(SDCardSPI_SPI0_RXDATA_PTR);

    #endif /* (SDCardSPI_SPI0_RX_SOFTWARE_BUF_ENABLED) */

    return(rxData);
}


/*******************************************************************************
* Function Name: SDCardSPI_SPI0_GetRxBufferSize
********************************************************************************
*
* Summary:
*  Returns the number of bytes/words of data currently held in the RX buffer.
*  If RX Software Buffer not used then function return 0 if FIFO empty or 1 if
*  FIFO not empty. In another case function return size of RX Software Buffer.
*
* Parameters:
*  None.
*
* Return:
*  Integer count of the number of bytes/words in the RX buffer.
*
* Global variables:
*  SDCardSPI_SPI0_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer.
*  SDCardSPI_SPI0_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer.
*
* Side Effects:
*  Clear status register of the component.
*
*******************************************************************************/
uint8 SDCardSPI_SPI0_GetRxBufferSize(void) 
{
    uint8 size;

    #if(SDCardSPI_SPI0_RX_SOFTWARE_BUF_ENABLED)

        /* Disable RX interrupt to protect global veriables */
        SDCardSPI_SPI0_DisableRxInt();

        if(SDCardSPI_SPI0_rxBufferRead == SDCardSPI_SPI0_rxBufferWrite)
        {
            size = 0u;
        }
        else if(SDCardSPI_SPI0_rxBufferRead < SDCardSPI_SPI0_rxBufferWrite)
        {
            size = (SDCardSPI_SPI0_rxBufferWrite - SDCardSPI_SPI0_rxBufferRead);
        }
        else
        {
            size = (SDCardSPI_SPI0_RX_BUFFER_SIZE - SDCardSPI_SPI0_rxBufferRead) + SDCardSPI_SPI0_rxBufferWrite;
        }

        SDCardSPI_SPI0_EnableRxInt();

    #else

        /* We can only know if there is data in the RX FIFO */
        size = (0u != (SDCardSPI_SPI0_RX_STATUS_REG & SDCardSPI_SPI0_STS_RX_FIFO_NOT_EMPTY)) ? 1u : 0u;

    #endif /* (SDCardSPI_SPI0_TX_SOFTWARE_BUF_ENABLED) */

    return(size);
}


/*******************************************************************************
* Function Name: SDCardSPI_SPI0_GetTxBufferSize
********************************************************************************
*
* Summary:
*  Returns the number of bytes/words of data currently held in the TX buffer.
*  If TX Software Buffer not used then function return 0 - if FIFO empty, 1 - if
*  FIFO not full, 4 - if FIFO full. In another case function return size of TX
*  Software Buffer.
*
* Parameters:
*  None.
*
* Return:
*  Integer count of the number of bytes/words in the TX buffer.
*
* Global variables:
*  SDCardSPI_SPI0_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer.
*  SDCardSPI_SPI0_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer.
*
* Side Effects:
*  Clear status register of the component.
*
*******************************************************************************/
uint8  SDCardSPI_SPI0_GetTxBufferSize(void) 
{
    uint8 size;

    #if(SDCardSPI_SPI0_TX_SOFTWARE_BUF_ENABLED)
        /* Disable TX interrupt to protect global veriables */
        SDCardSPI_SPI0_DisableTxInt();

        if(SDCardSPI_SPI0_txBufferRead == SDCardSPI_SPI0_txBufferWrite)
        {
            size = 0u;
        }
        else if(SDCardSPI_SPI0_txBufferRead < SDCardSPI_SPI0_txBufferWrite)
        {
            size = (SDCardSPI_SPI0_txBufferWrite - SDCardSPI_SPI0_txBufferRead);
        }
        else
        {
            size = (SDCardSPI_SPI0_TX_BUFFER_SIZE - SDCardSPI_SPI0_txBufferRead) + SDCardSPI_SPI0_txBufferWrite;
        }

        SDCardSPI_SPI0_EnableTxInt();

    #else

        size = SDCardSPI_SPI0_TX_STATUS_REG;

        if(0u != (size & SDCardSPI_SPI0_STS_TX_FIFO_EMPTY))
        {
            size = 0u;
        }
        else if(0u != (size & SDCardSPI_SPI0_STS_TX_FIFO_NOT_FULL))
        {
            size = 1u;
        }
        else
        {
            size = SDCardSPI_SPI0_FIFO_SIZE;
        }

    #endif /* (SDCardSPI_SPI0_TX_SOFTWARE_BUF_ENABLED) */

    return(size);
}


/*******************************************************************************
* Function Name: SDCardSPI_SPI0_ClearRxBuffer
********************************************************************************
*
* Summary:
*  Clear the RX RAM buffer by setting the read and write pointers both to zero.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  SDCardSPI_SPI0_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer, modified every function
*  call - resets to zero.
*  SDCardSPI_SPI0_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer, modified every function call -
*  resets to zero.
*
* Theory:
*  Setting the pointers to zero makes the system believe there is no data to
*  read and writing will resume at address 0 overwriting any data that may have
*  remained in the RAM.
*
* Side Effects:
*  Any received data not read from the RAM buffer will be lost when overwritten.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SDCardSPI_SPI0_ClearRxBuffer(void) 
{
    /* Clear Hardware RX FIFO */
    while(0u !=(SDCardSPI_SPI0_RX_STATUS_REG & SDCardSPI_SPI0_STS_RX_FIFO_NOT_EMPTY))
    {
        (void) CY_GET_REG8(SDCardSPI_SPI0_RXDATA_PTR);
    }

    #if(SDCardSPI_SPI0_RX_SOFTWARE_BUF_ENABLED)
        /* Disable RX interrupt to protect global veriables */
        SDCardSPI_SPI0_DisableRxInt();

        SDCardSPI_SPI0_rxBufferFull  = 0u;
        SDCardSPI_SPI0_rxBufferRead  = 0u;
        SDCardSPI_SPI0_rxBufferWrite = 0u;

        SDCardSPI_SPI0_EnableRxInt();
    #endif /* (SDCardSPI_SPI0_RX_SOFTWARE_BUF_ENABLED) */
}


/*******************************************************************************
* Function Name: SDCardSPI_SPI0_ClearTxBuffer
********************************************************************************
*
* Summary:
*  Clear the TX RAM buffer by setting the read and write pointers both to zero.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  SDCardSPI_SPI0_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer, modified every function
*  call - resets to zero.
*  SDCardSPI_SPI0_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer, modified every function call -
*  resets to zero.
*
* Theory:
*  Setting the pointers to zero makes the system believe there is no data to
*  read and writing will resume at address 0 overwriting any data that may have
*  remained in the RAM.
*
* Side Effects:
*  Any data not yet transmitted from the RAM buffer will be lost when
*  overwritten.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SDCardSPI_SPI0_ClearTxBuffer(void) 
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    /* Clear TX FIFO */
    SDCardSPI_SPI0_AUX_CONTROL_DP0_REG |= ((uint8)  SDCardSPI_SPI0_TX_FIFO_CLR);
    SDCardSPI_SPI0_AUX_CONTROL_DP0_REG &= ((uint8) ~SDCardSPI_SPI0_TX_FIFO_CLR);

    #if(SDCardSPI_SPI0_USE_SECOND_DATAPATH)
        /* Clear TX FIFO for 2nd Datapath */
        SDCardSPI_SPI0_AUX_CONTROL_DP1_REG |= ((uint8)  SDCardSPI_SPI0_TX_FIFO_CLR);
        SDCardSPI_SPI0_AUX_CONTROL_DP1_REG &= ((uint8) ~SDCardSPI_SPI0_TX_FIFO_CLR);
    #endif /* (SDCardSPI_SPI0_USE_SECOND_DATAPATH) */
    CyExitCriticalSection(enableInterrupts);

    #if(SDCardSPI_SPI0_TX_SOFTWARE_BUF_ENABLED)
        /* Disable TX interrupt to protect global veriables */
        SDCardSPI_SPI0_DisableTxInt();

        SDCardSPI_SPI0_txBufferFull  = 0u;
        SDCardSPI_SPI0_txBufferRead  = 0u;
        SDCardSPI_SPI0_txBufferWrite = 0u;

        /* Buffer is EMPTY: disable TX FIFO NOT FULL interrupt */
        SDCardSPI_SPI0_TX_STATUS_MASK_REG &= ((uint8) ~SDCardSPI_SPI0_STS_TX_FIFO_NOT_FULL);

        SDCardSPI_SPI0_EnableTxInt();
    #endif /* (SDCardSPI_SPI0_TX_SOFTWARE_BUF_ENABLED) */
}


#if(0u != SDCardSPI_SPI0_BIDIRECTIONAL_MODE)
    /*******************************************************************************
    * Function Name: SDCardSPI_SPI0_TxEnable
    ********************************************************************************
    *
    * Summary:
    *  If the SPI master is configured to use a single bi-directional pin then this
    *  will set the bi-directional pin to transmit.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void SDCardSPI_SPI0_TxEnable(void) 
    {
        SDCardSPI_SPI0_CONTROL_REG |= SDCardSPI_SPI0_CTRL_TX_SIGNAL_EN;
    }


    /*******************************************************************************
    * Function Name: SDCardSPI_SPI0_TxDisable
    ********************************************************************************
    *
    * Summary:
    *  If the SPI master is configured to use a single bi-directional pin then this
    *  will set the bi-directional pin to receive.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void SDCardSPI_SPI0_TxDisable(void) 
    {
        SDCardSPI_SPI0_CONTROL_REG &= ((uint8) ~SDCardSPI_SPI0_CTRL_TX_SIGNAL_EN);
    }

#endif /* (0u != SDCardSPI_SPI0_BIDIRECTIONAL_MODE) */


/*******************************************************************************
* Function Name: SDCardSPI_SPI0_PutArray
********************************************************************************
*
* Summary:
*  Write available data from ROM/RAM to the TX buffer while space is available
*  in the TX buffer. Keep trying until all data is passed to the TX buffer.
*
* Parameters:
*  *buffer: Pointer to the location in RAM containing the data to send
*  byteCount: The number of bytes to move to the transmit buffer.
*
* Return:
*  None.
*
* Side Effects:
*  Will stay in this routine until all data has been sent.  May get locked in
*  this loop if data is not being initiated by the master if there is not
*  enough room in the TX FIFO.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SDCardSPI_SPI0_PutArray(const uint8 buffer[], uint8 byteCount)
                                                                          
{
    uint8 bufIndex;

    bufIndex = 0u;

    while(byteCount > 0u)
    {
        SDCardSPI_SPI0_WriteTxData(buffer[bufIndex]);
        bufIndex++;
        byteCount--;
    }
}


/*******************************************************************************
* Function Name: SDCardSPI_SPI0_ClearFIFO
********************************************************************************
*
* Summary:
*  Clear the RX and TX FIFO's of all data for a fresh start.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  Clear status register of the component.
*
*******************************************************************************/
void SDCardSPI_SPI0_ClearFIFO(void) 
{
    uint8 enableInterrupts;

    /* Clear Hardware RX FIFO */
    while(0u !=(SDCardSPI_SPI0_RX_STATUS_REG & SDCardSPI_SPI0_STS_RX_FIFO_NOT_EMPTY))
    {
        (void) CY_GET_REG8(SDCardSPI_SPI0_RXDATA_PTR);
    }

    enableInterrupts = CyEnterCriticalSection();
    /* Clear TX FIFO */
    SDCardSPI_SPI0_AUX_CONTROL_DP0_REG |= ((uint8)  SDCardSPI_SPI0_TX_FIFO_CLR);
    SDCardSPI_SPI0_AUX_CONTROL_DP0_REG &= ((uint8) ~SDCardSPI_SPI0_TX_FIFO_CLR);

    #if(SDCardSPI_SPI0_USE_SECOND_DATAPATH)
        /* Clear TX FIFO for 2nd Datapath */
        SDCardSPI_SPI0_AUX_CONTROL_DP1_REG |= ((uint8)  SDCardSPI_SPI0_TX_FIFO_CLR);
        SDCardSPI_SPI0_AUX_CONTROL_DP1_REG &= ((uint8) ~SDCardSPI_SPI0_TX_FIFO_CLR);
    #endif /* (SDCardSPI_SPI0_USE_SECOND_DATAPATH) */
    CyExitCriticalSection(enableInterrupts);
}


/* Following functions are for version Compatibility, they are obsolete.
*  Please do not use it in new projects.
*/


/*******************************************************************************
* Function Name: SDCardSPI_SPI0_EnableInt
********************************************************************************
*
* Summary:
*  Enable internal interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Enable the internal interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void SDCardSPI_SPI0_EnableInt(void) 
{
    SDCardSPI_SPI0_EnableRxInt();
    SDCardSPI_SPI0_EnableTxInt();
}


/*******************************************************************************
* Function Name: SDCardSPI_SPI0_DisableInt
********************************************************************************
*
* Summary:
*  Disable internal interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the internal interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void SDCardSPI_SPI0_DisableInt(void) 
{
    SDCardSPI_SPI0_DisableTxInt();
    SDCardSPI_SPI0_DisableRxInt();
}


/*******************************************************************************
* Function Name: SDCardSPI_SPI0_SetInterruptMode
********************************************************************************
*
* Summary:
*  Configure which status bits trigger an interrupt event.
*
* Parameters:
*  intSrc: An or'd combination of the desired status bit masks (defined in the
*  header file).
*
* Return:
*  None.
*
* Theory:
*  Enables the output of specific status bits to the interrupt controller.
*
*******************************************************************************/
void SDCardSPI_SPI0_SetInterruptMode(uint8 intSrc) 
{
    SDCardSPI_SPI0_TX_STATUS_MASK_REG  = (intSrc & ((uint8) ~SDCardSPI_SPI0_STS_SPI_IDLE));
    SDCardSPI_SPI0_RX_STATUS_MASK_REG  =  intSrc;
}


/*******************************************************************************
* Function Name: SDCardSPI_SPI0_ReadStatus
********************************************************************************
*
* Summary:
*  Read the status register for the component.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the status register.
*
* Global variables:
*  SDCardSPI_SPI0_swStatus - used to store in software status register,
*  modified every function call - resets to zero.
*
* Theory:
*  Allows the user and the API to read the status register for error detection
*  and flow control.
*
* Side Effects:
*  Clear status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 SDCardSPI_SPI0_ReadStatus(void) 
{
    uint8 tmpStatus;

    #if(SDCardSPI_SPI0_TX_SOFTWARE_BUF_ENABLED || SDCardSPI_SPI0_RX_SOFTWARE_BUF_ENABLED)

        SDCardSPI_SPI0_DisableInt();

        tmpStatus  = SDCardSPI_SPI0_GET_STATUS_RX(SDCardSPI_SPI0_swStatusRx);
        tmpStatus |= SDCardSPI_SPI0_GET_STATUS_TX(SDCardSPI_SPI0_swStatusTx);
        tmpStatus &= ((uint8) ~SDCardSPI_SPI0_STS_SPI_IDLE);

        SDCardSPI_SPI0_swStatusTx = 0u;
        SDCardSPI_SPI0_swStatusRx = 0u;

        SDCardSPI_SPI0_EnableInt();

    #else

        tmpStatus  = SDCardSPI_SPI0_RX_STATUS_REG;
        tmpStatus |= SDCardSPI_SPI0_TX_STATUS_REG;
        tmpStatus &= ((uint8) ~SDCardSPI_SPI0_STS_SPI_IDLE);

    #endif /* (SDCardSPI_SPI0_TX_SOFTWARE_BUF_ENABLED || SDCardSPI_SPI0_RX_SOFTWARE_BUF_ENABLED) */

    return(tmpStatus);
}


/* [] END OF FILE */
