#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "spi_drv.h"
#include "ak8963_drv.h"
#include "mpu9250_drv.h"

AK8963Drv & AK8963Drv::instance(const uint8_t & t_unit) {

    //
    //
    //
    switch (t_unit) {
        case AK8963Drv::Unit::AK8963_1:
            static AK8963Drv s_instance_1 = AK8963Drv(t_unit);
            return s_instance_1;
        case AK8963Drv::Unit::AK8963_2:
            static AK8963Drv s_instance_2 = AK8963Drv(t_unit);
            return s_instance_2;
        case AK8963Drv::Unit::AK8963_3:
            static AK8963Drv s_instance_3 = AK8963Drv(t_unit);
            return s_instance_3;
        case AK8963Drv::Unit::AK8963_4:
            static AK8963Drv s_instance_4 = AK8963Drv(t_unit);
            return s_instance_4;
    }
}

AK8963Drv::AK8963Drv(const uint8_t & t_unit) {

    /*
    writeByte(MPU9250_ADDRESS, USER_CTRL, 0x20);          // Enable I2C Master mode
    writeByte(MPU9250_ADDRESS, I2C_MST_CTRL, 0x1D);       // I2C configuration STOP after each transaction, master I2C bus at 400 KHz
    writeByte(MPU9250_ADDRESS, I2C_MST_DELAY_CTRL, 0x81); // Use blocking data retreival and enable delay for mag sample rate mismatch
    writeByte(MPU9250_ADDRESS, I2C_SLV4_CTRL, 0x01);      // Delay mag data retrieval to once every other accel/gyro data sample

    // First extract the factory calibration for each magnetometer axis
    uint8_t rawData[3];  // x/y/z gyro calibration data stored here

    writeByte(MPU9250_ADDRESS, I2C_SLV0_ADDR, AK8963_ADDRESS);           // Set the I2C slave address of AK8963 and set for write.
    writeByte(MPU9250_ADDRESS, I2C_SLV0_REG, AK8963_CNTL2);              // I2C slave 0 register address from where to begin data transfer
    writeByte(MPU9250_ADDRESS, I2C_SLV0_DO, 0x01);                       // Reset AK8963
    writeByte(MPU9250_ADDRESS, I2C_SLV0_CTRL, 0x81);                     // Enable I2C and write 1 byte
    delay(50);
    writeByte(MPU9250_ADDRESS, I2C_SLV0_ADDR, AK8963_ADDRESS);           // Set the I2C slave address of AK8963 and set for write.
    writeByte(MPU9250_ADDRESS, I2C_SLV0_REG, AK8963_CNTL);               // I2C slave 0 register address from where to begin data transfer
    writeByte(MPU9250_ADDRESS, I2C_SLV0_DO, 0x00);                       // Power down magnetometer
    writeByte(MPU9250_ADDRESS, I2C_SLV0_CTRL, 0x81);                     // Enable I2C and write 1 byte
    delay(50);
    writeByte(MPU9250_ADDRESS, I2C_SLV0_ADDR, AK8963_ADDRESS);           // Set the I2C slave address of AK8963 and set for write.
    writeByte(MPU9250_ADDRESS, I2C_SLV0_REG, AK8963_CNTL);               // I2C slave 0 register address from where to begin data transfer
    writeByte(MPU9250_ADDRESS, I2C_SLV0_DO, 0x0F);                       // Enter fuze mode
    writeByte(MPU9250_ADDRESS, I2C_SLV0_CTRL, 0x81);                     // Enable I2C and write 1 byte
    delay(50);

    writeByte(MPU9250_ADDRESS, I2C_SLV0_ADDR, AK8963_ADDRESS | 0x80);    // Set the I2C slave address of AK8963 and set for read.
    writeByte(MPU9250_ADDRESS, I2C_SLV0_REG, AK8963_ASAX);               // I2C slave 0 register address from where to begin data transfer
    writeByte(MPU9250_ADDRESS, I2C_SLV0_CTRL, 0x83);                     // Enable I2C and read 3 bytes
    delay(50);
    readBytes(MPU9250_ADDRESS, EXT_SENS_DATA_00, 3, &rawData[0]);        // Read the x-, y-, and z-axis calibration values
    magCalibration[0] =  (float)(rawData[0] - 128)/256.0f + 1.0f;        // Return x-axis sensitivity adjustment values, etc.
    magCalibration[1] =  (float)(rawData[1] - 128)/256.0f + 1.0f;
    magCalibration[2] =  (float)(rawData[2] - 128)/256.0f + 1.0f;
    _magCalibration[0] = magCalibration[0];
    _magCalibration[1] = magCalibration[1];
    _magCalibration[2] = magCalibration[2];
    _Mmode = Mmode;

    writeByte(MPU9250_ADDRESS, I2C_SLV0_ADDR, AK8963_ADDRESS);           // Set the I2C slave address of AK8963 and set for write.
    writeByte(MPU9250_ADDRESS, I2C_SLV0_REG, AK8963_CNTL);               // I2C slave 0 register address from where to begin data transfer
    writeByte(MPU9250_ADDRESS, I2C_SLV0_DO, 0x00);                       // Power down magnetometer
    writeByte(MPU9250_ADDRESS, I2C_SLV0_CTRL, 0x81);                     // Enable I2C and transfer 1 byte
    delay(50);

    writeByte(MPU9250_ADDRESS, I2C_SLV0_ADDR, AK8963_ADDRESS);           // Set the I2C slave address of AK8963 and set for write.
    writeByte(MPU9250_ADDRESS, I2C_SLV0_REG, AK8963_CNTL);               // I2C slave 0 register address from where to begin data transfer
    // Configure the magnetometer for continuous read and highest resolution
    // set Mscale bit 4 to 1 (0) to enable 16 (14) bit resolution in CNTL register,
    // and enable continuous mode data acquisition Mmode (bits [3:0]), 0010 for 8 Hz and 0110 for 100 Hz sample rates
    writeByte(MPU9250_ADDRESS, I2C_SLV0_DO, Mscale << 4 | Mmode);        // Set magnetometer data resolution and sample ODR
    writeByte(MPU9250_ADDRESS, I2C_SLV0_CTRL, 0x81);                     // Enable I2C and transfer 1 byte
    delay(50);
    writeByte(MPU9250_ADDRESS, I2C_SLV0_ADDR, AK8963_ADDRESS | 0x80);    // Set the I2C slave address of AK8963 and set for read.
    writeByte(MPU9250_ADDRESS, I2C_SLV0_REG, AK8963_CNTL);               // I2C slave 0 register address from where to begin data transfer
    writeByte(MPU9250_ADDRESS, I2C_SLV0_CTRL, 0x81);                     // Enable I2C and transfer 1 byte
    delay(50);
    */
}

void AK8963Drv::init() {

}

uint8_t AK8963Drv::read(const uint8_t & t_register) {

    /*
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
    */

}

void AK8963Drv::write(const uint8_t & t_register, const uint8_t & t_data) {

    /*
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
    */

}

void AK8963Drv::dataRead() {

}

RawData AK8963Drv::dataMagGetRaw() {

}

FloatData AK8963Drv::dataMagGetFloat() {

}
