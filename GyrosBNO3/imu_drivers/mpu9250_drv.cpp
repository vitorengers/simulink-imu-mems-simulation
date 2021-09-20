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
#include "mpu9250_drv.h"
#include "utils/uartstdio.h"

static void cs_high(const uint8_t & t_unit) {
//    UARTprintf("cs high");
    //
    //
    //
    switch (t_unit) {
        case MPU9250Drv::Unit::MPU_1:
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0xFF);
            break;
        case MPU9250Drv::Unit::MPU_2:
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0xFF);
            break;
        case MPU9250Drv::Unit::MPU_3:
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0xFF);
            break;
        case MPU9250Drv::Unit::MPU_4:
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0xFF);
            break;
    }

}

static void cs_low(const uint8_t & t_unit) {
//    UARTprintf("cs low");
    //
    //
    //
    switch (t_unit) {
        case MPU9250Drv::Unit::MPU_1:
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0x00);
            break;
        case MPU9250Drv::Unit::MPU_2:
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0x00);
            break;
        case MPU9250Drv::Unit::MPU_3:
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0x00);
            break;
        case MPU9250Drv::Unit::MPU_4:
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0x00);
            break;
    }

}

static void self_test() {

    //
    //
    //

}

static void calibrate() {

    //
    //
    //

}

MPU9250Drv & MPU9250Drv::instance(const uint8_t & t_unit) {
//    UARTprintf("instance");
    //
    //
    //

    switch (t_unit) {
        case MPU9250Drv::Unit::MPU_1:
            static MPU9250Drv s_instance_1 = MPU9250Drv(t_unit);
            return s_instance_1;
        case MPU9250Drv::Unit::MPU_2:
            static MPU9250Drv s_instance_2 = MPU9250Drv(t_unit);
            return s_instance_2;
        case MPU9250Drv::Unit::MPU_3:
            static MPU9250Drv s_instance_3 = MPU9250Drv(t_unit);
            return s_instance_3;
        case MPU9250Drv::Unit::MPU_4:
            static MPU9250Drv s_instance_4 = MPU9250Drv(t_unit);
            return s_instance_4;
    }

}

MPU9250Drv::MPU9250Drv(const uint8_t & t_unit) {
//    UARTprintf("constructor");
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

    //
    // Start by performing self test and reporting values
    //
    self_test();

    //
    // Calibrate gyroscope and accelerometer, load biases in bias registers
    //
    calibrate();

}

void MPU9250Drv::init() {
//    UARTprintf("init");
    uint32_t u32_data;

    //
    // Wake up the device
    //
    write(RegisterMap::PWR_MGMT_1,0x00);

    //
    // Get stable time source
    //
    write(RegisterMap::PWR_MGMT_1, 0x01);

    //
    // Configure gyroscope and thermometer
    //
    write(RegisterMap::CONFIG, 0x03);

    //
    // Set sample rate = gyroscope output rate/(1 + SMPLRT_DIV)
    //
    write(RegisterMap::SMPLRT_DIV, 0x04);

    //
    // Set gyroscope full-scale range configuration
    //
    u32_data = read(RegisterMap::GYRO_CONFIG);
    // u32_data = u32_data & ~0xE0;
    u32_data = u32_data & ~0x03;
    u32_data = u32_data & ~0x18;
    u32_data = u32_data | m_gyro_fs_sel << 3;
    // u32_data =| 0x00;
    write(RegisterMap::GYRO_CONFIG, u32_data);

    //
    // Set accelerometer full-scale range configuration
    //
    u32_data = read(RegisterMap::ACCEL_CONFIG);
    // u32_data = u32_data & ~0xE0;
    u32_data = u32_data & ~0x18;
    u32_data = u32_data | m_accel_fs_sel << 3;
    write(RegisterMap::ACCEL_CONFIG, u32_data);

    //
    // Set accelerometer sample rate configuration
    //
    u32_data = read(RegisterMap::ACCEL_CONFIG_2);
    u32_data = u32_data & ~0x0F;
    u32_data = u32_data | 0x03;
    write(RegisterMap::ACCEL_CONFIG_2, u32_data);

    //
    // Configure Interrupts and Bypass Enable setting interrupt pin active high,
    // push-pull, hold interrupt pin level HIGH until interrupt cleared, clear on
    // read of INT_STATUS, and enable I2C_BYPASS_EN so additional chips can join
    // the I2C bus and all can be controlled by the master
    //
    write(RegisterMap::INT_PIN_CFG, 0x22);
    write(RegisterMap::INT_ENABLE, 0x01);

}

uint8_t MPU9250Drv::read(const uint8_t & t_register) {

    //
    //
    //
    cs_low(m_unit);

    //
    //
    //
    if (SPIDrv::instance().dataWidth() != 8) {
        SPIDrv::instance().dataWidth(8);
    }

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

void MPU9250Drv::write(const uint8_t & t_register, const uint8_t & t_data) {

    //
    //
    //
    cs_low(m_unit);

    //
    //
    //
    if (SPIDrv::instance().dataWidth() != 8) {
        SPIDrv::instance().dataWidth(8);
    }

    //
    //
    //
    SPIDrv::instance().send(t_register, t_data);

    //
    //
    //
    cs_high(m_unit);

}

void MPU9250Drv::dataRead() {

    //
    //  Read accelerometer data
    //
    m_data_buffer[0]  = read(RegisterMap::ACCEL_XOUT_H);
    m_data_buffer[1]  = read(RegisterMap::ACCEL_XOUT_L);
    m_data_buffer[2]  = read(RegisterMap::ACCEL_YOUT_H);
    m_data_buffer[3]  = read(RegisterMap::ACCEL_YOUT_L);
    m_data_buffer[4]  = read(RegisterMap::ACCEL_ZOUT_H);
    m_data_buffer[5]  = read(RegisterMap::ACCEL_ZOUT_L);

    //
    // Read gyroscope data
    //
    m_data_buffer[6]  = read(RegisterMap::GYRO_XOUT_H);
    m_data_buffer[7]  = read(RegisterMap::GYRO_XOUT_L);
    m_data_buffer[8]  = read(RegisterMap::GYRO_YOUT_H);
    m_data_buffer[9]  = read(RegisterMap::GYRO_YOUT_L);
    m_data_buffer[10] = read(RegisterMap::GYRO_ZOUT_H);
    m_data_buffer[11] = read(RegisterMap::GYRO_ZOUT_L);

}

RawData MPU9250Drv::dataAccelGetRaw() {

    RawData ret_val;

    //
    //
    //
//    UARTprintf("passou 6");
    ret_val.x = raw_valid((m_data_buffer[0] << 8) | m_data_buffer[1]);
    ret_val.y = raw_valid((m_data_buffer[2] << 8) | m_data_buffer[3]);
    ret_val.z = raw_valid((m_data_buffer[4] << 8) | m_data_buffer[5]);
    return ret_val;

}

FloatData MPU9250Drv::dataAccelGetFloat() {

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

RawData MPU9250Drv::dataGyroGetRaw() {

    RawData ret_val;

    //
    //
    //
    ret_val.x = raw_valid((m_data_buffer[6] << 8) | m_data_buffer[7]);
    ret_val.y = raw_valid((m_data_buffer[8] << 8) | m_data_buffer[9]);
    ret_val.z = raw_valid((m_data_buffer[10] << 8) | m_data_buffer[11]);
    return ret_val;

}

FloatData MPU9250Drv::dataGyroGetFloat() {

    RawData tmp_val;
    FloatData ret_val;
    uint16_t tmp;

    //
    //
    //
    tmp_val = dataAccelGetRaw();
    ret_val.x = (tmp_val.x.right(tmp)) ? float_valid(tmp * 0.00763) : float_invalid(-1);  // TODO :: sensibility
    ret_val.y = (tmp_val.y.right(tmp)) ? float_valid(tmp * 0.00763) : float_invalid(-1);
    ret_val.z = (tmp_val.z.right(tmp)) ? float_valid(tmp * 0.00763) : float_invalid(-1);
    return ret_val;

}
