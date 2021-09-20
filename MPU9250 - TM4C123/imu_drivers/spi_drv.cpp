// -*-c++-*-

/*!
  \file spi_drv.cpp
  \brief A Documented file.
*/

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "spi_drv.h"

SPIDrv::SPIDrv() {

    uint32_t u32DataRx;

    //
    // The SSI0 peripheral must be enabled for use.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);

    //
    // For this example SSI0 is used with PortA[5:2]. The actual port and pins
    // used may be different on your part, consult the data sheet for more
    // information.  GPIO port A needs to be enabled so these pins can be used.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Configure the pin muxing for SSI0 functions on port A2, A3, A4, and A5.
    // This step is not necessary if your part does not support pin muxing.
    //
    GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    GPIOPinConfigure(GPIO_PA4_SSI0RX);
    GPIOPinConfigure(GPIO_PA5_SSI0TX);

    //
    // Configure the GPIO settings for the SSI pins.  This function also gives
    // control of these pins to the SSI hardware.  Consult the data sheet to
    // see which functions are allocated per pin.
    // The pins are assigned as follows:
    //      PA5 - SSI0Tx
    //      PA4 - SSI0Rx
    //      PA2 - SSI0CLK
    //
    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_2);

    //
    // Configure and enable the SSI port for SPI master mode.  Use SSI0,
    // system clock supply, idle clock level low and active low clock in
    // freescale SPI mode, master mode, 1MHz SSI frequency, and 8-bit data.
    // For SPI mode, you can set the polarity of the SSI clock when the SSI
    // unit is idle.  You can also configure what clock edge you want to
    // capture data on.  Please reference the datasheet for more information on
    // the different SPI modes.
    //
    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0,
                       SSI_MODE_MASTER, 1000000, 8);
    m_bit_rate = 1000000;
    m_data_width = 8;

    //
    // Enable the SSI0 module.
    //
    SSIEnable(SSI0_BASE);

    //
    // Read any residual data from the SSI port. This makes sure the receive
    // FIFOs are empty, so we don't read any unwanted junk. This is done here
    // because the SPI SSI mode is full-duplex, which allows you to send and
    // receive at the same time. The SSIDataGetNonBlocking function returns
    // "true" when data was returned, and "false" when no data was returned.
    // The "non-blocking" function checks if there is any data in the receive
    // FIFO and does not "hang" if there isn't.
    //
    while (SSIDataGetNonBlocking(SSI0_BASE, &u32DataRx)) continue;

}

SPIDrv & SPIDrv::instance() {

    //
    //
    //
    static SPIDrv s_instance;
    return s_instance;
}

void SPIDrv::bitRate(const uint32_t & t_bit_rate) {

    //
    // Configure and enable the SSI port for SPI master mode.  Use SSI0,
    // system clock supply, idle clock level low and active low clock in
    // freescale SPI mode, master mode, 1MHz SSI frequency, and 8-bit data.
    // For SPI mode, you can set the polarity of the SSI clock when the SSI
    // unit is idle.  You can also configure what clock edge you want to
    // capture data on.  Please reference the datasheet for more information on
    // the different SPI modes.
    //
    SSIDisable(SSI0_BASE);
    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0,
                       SSI_MODE_MASTER, t_bit_rate, m_data_width);
    m_bit_rate = t_bit_rate;
    SSIEnable(SSI0_BASE);
}

void SPIDrv::dataWidth(const uint32_t & t_data_width) {

    //
    // Configure and enable the SSI port for SPI master mode.  Use SSI0,
    // system clock supply, idle clock level low and active low clock in
    // freescale SPI mode, master mode, 1MHz SSI frequency, and 8-bit data.
    // For SPI mode, you can set the polarity of the SSI clock when the SSI
    // unit is idle.  You can also configure what clock edge you want to
    // capture data on.  Please reference the datasheet for more information on
    // the different SPI modes.
    //
    SSIDisable(SSI0_BASE);
    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0,
                       SSI_MODE_MASTER, m_bit_rate, t_data_width);
    m_data_width = t_data_width;
    SSIEnable(SSI0_BASE);
}

void SPIDrv::send(const uint32_t & t_register, const uint32_t & t_data) {

    uint32_t u32DataRx;

    //
    //
    //
    SSIDataPut(SSI0_BASE, t_register);
    SSIDataPut(SSI0_BASE, t_data);
    while (SSIBusy(SSI0_BASE)) continue;

    //
    // Read any residual data from the SSI port. This makes sure the receive
    // FIFOs are empty, so we don't read any unwanted junk. This is done here
    // because the SPI SSI mode is full-duplex, which allows you to send and
    // receive at the same time. The SSIDataGetNonBlocking function returns
    // "true" when data was returned, and "false" when no data was returned.
    // The "non-blocking" function checks if there is any data in the receive
    // FIFO and does not "hang" if there isn't.
    //
    while (SSIDataGetNonBlocking(SSI0_BASE, &u32DataRx)) continue;
}

uint32_t SPIDrv::receive(const uint32_t & t_register) {

    uint32_t u32DataRx;

    //
    //
    //
    SSIDataPut(SSI0_BASE, t_register);
    SSIDataPut(SSI0_BASE, 0x00);
    while (SSIBusy(SSI0_BASE)) continue;
    SSIDataGet(SSI0_BASE, &u32DataRx);
    SSIDataGet(SSI0_BASE, &u32DataRx);
    return u32DataRx;
}
