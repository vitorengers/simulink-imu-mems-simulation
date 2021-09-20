/*! \file structcmd.h
    \brief A Documented file.

    Details.
*/

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "spi_drv.h"
#include "scc2130_drv.h"

static void cs_high(const uint8_t & t_unit) {

    //
    //
    //
    switch (t_unit) {
        case SCC2130Drv::Unit::SCC_1:
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0xFF);
            break;
        case SCC2130Drv::Unit::SCC_2:
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0xFF);
            break;
    }

}

static void cs_low(const uint8_t & t_unit) {

    //
    //
    //
    switch (t_unit) {
        case SCC2130Drv::Unit::SCC_1:
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0x00);
            break;
        case SCC2130Drv::Unit::SCC_2:
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0x00);
            break;
    }

}

SCC2130Drv::SCC2130Drv(const uint8_t & t_unit) {

    uint8_t i;

    //
    //
    //
    m_unit = t_unit;

    //
    //
    //
    m_gyro_fs_sel = 0;

    //
    //
    //
    m_accel_fs_sel = 0;

    //
    //
    //
    for (i = 0; i < 12; i++) {
        m_data_buffer[i] = 0;
    }

    //
    // Enabling necessary peripherals
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    //
    // CS pins configuration
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,GPIO_PIN_1|GPIO_PIN_4);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,GPIO_PIN_4|GPIO_PIN_5);

    //
    // Setting all CS pins to HIGH
    //
    GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_1|GPIO_PIN_4,0xFF);
    GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_4|GPIO_PIN_5,0xFF);

}

void SCC2130Drv::init() {

}

uint8_t SCC2130Drv::read(const uint8_t & t_register) {

    //
    //
    //
    cs_low(m_unit);

    //
    //
    //
    uint32_t ret_val = SPIDrv::instance().receive(t_register);

    //
    //
    //
    cs_high(m_unit);
    return ret_val;

}

void SCC2130Drv::write(const uint8_t & t_register, const uint8_t & t_data) {

    //
    //
    //
    cs_low(m_unit);

    //
    //
    //
    SPIDrv::instance().send(t_register, t_data);

    //
    //
    //
    cs_high(m_unit);

}

void SCC2130Drv::dataRead() {

    //
    //  Read accelerometer data
    //

    //
    // Read gyroscope data
    //

}

RawData SCC2130Drv::dataAccelGetRaw() {

    RawData ret_val;

    //
    //
    //
    ret_val.x = raw_valid((m_data_buffer[6] << 8) | m_data_buffer[7]);
    ret_val.y = raw_valid((m_data_buffer[8] << 8) | m_data_buffer[9]);
    ret_val.z = raw_valid((m_data_buffer[10] << 8) | m_data_buffer[11]);
    return ret_val;

}

FloatData SCC2130Drv::dataAccelGetFloat() {

    RawData tmp_val;
    FloatData ret_val;
    uint16_t tmp;

    //
    //
    //
    tmp_val = dataAccelGetRaw();
    ret_val.x = (tmp_val.x.right(tmp)) ? float_valid(tmp * 0.000061) : float_invalid(-1);  // TODO :: sensibility
    ret_val.y = (tmp_val.y.right(tmp)) ? float_valid(tmp * 0.000061) : float_invalid(-1);
    ret_val.z = (tmp_val.z.right(tmp)) ? float_valid(tmp * 0.000061) : float_invalid(-1);
    return ret_val;

}

RawData SCC2130Drv::dataGyroGetRaw() {

    RawData ret_val;

    //
    //
    //
    ret_val.x = raw_valid((m_data_buffer[6] << 8) | m_data_buffer[7]);
    ret_val.y = raw_valid((m_data_buffer[8] << 8) | m_data_buffer[9]);
    ret_val.z = raw_valid((m_data_buffer[10] << 8) | m_data_buffer[11]);
    return ret_val;

}

FloatData SCC2130Drv::dataGyroGetFloat() {

    RawData tmp_val;
    FloatData ret_val;
    uint16_t tmp;

    //
    //
    //
    tmp_val = dataAccelGetRaw();
    ret_val.x = (tmp_val.x.right(tmp)) ? float_valid(tmp * 0.000061) : float_invalid(-1);  // TODO :: sensibility
    ret_val.y = (tmp_val.y.right(tmp)) ? float_valid(tmp * 0.000061) : float_invalid(-1);
    ret_val.z = (tmp_val.z.right(tmp)) ? float_valid(tmp * 0.000061) : float_invalid(-1);
    return ret_val;

}
