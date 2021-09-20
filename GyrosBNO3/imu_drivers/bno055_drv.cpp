#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "i2c_drv.h"
#include "bno055_drv.h"

BNO055Drv& BNO055Drv::instance(const uint8_t & t_unit) {

    //
    //
    //
    switch (t_unit) {
        case BNO055Drv::Unit::BNO_1:
            static BNO055Drv s_instance_1 = BNO055Drv(t_unit);
            return s_instance_1;
        case BNO055Drv::Unit::BNO_2:
            static BNO055Drv s_instance_2 = BNO055Drv(t_unit);
            return s_instance_2;
    }

}

BNO055Drv::BNO055Drv(const uint8_t & t_unit) {

    m_unit = t_unit;
    m_page = 0;
    m_operation_mode = CONFIGMODE;
    m_power_mode = NORMAL_MODE;
    m_accel_unit = MS2;
    m_gyro_unit = DPS;
    m_init = false;

}

void BNO055Drv::init() {


    //
    // Select BNO055 config mode
    //
    set_operation_mode(CONFIGMODE);

    //
    // Configure ACC
    //
    write(PAGE_1, ACC_CONFIG, 0x00 << 5 | 0x02 << 3 | 0x00);

    //
    // Configure GYR
    //
    write(PAGE_1, GYR_CONFIG_0, 0x04 << 3 | 0x03);
    write(PAGE_1, GYR_CONFIG_1, 0x00);

    //
    // Configure MAG
    //
    write(PAGE_1, MAG_CONFIG, 0x00 << 5 | 0x01 << 3 | 0x03);

    //
    // Select BNO055 gyro temperature source
    //
    write(PAGE_0, TEMP_SOURCE, 0x01);

    //
    // Select BNO055 system power mode
    //
    set_power_mode(NORMAL_MODE);

    //
    // Select BNO055 system operation mode
    //
    set_operation_mode(AMG);
    m_init = true;

}

uint8_t BNO055Drv::read_chip_id() {

    return read(PAGE_0, CHIP_ID);

}

uint8_t BNO055Drv::read_page_id() {

    return read(PAGE_0, PAGE_ID);

}

void BNO055Drv::write_page_id(uint8_t t_page_id) {

    write(PAGE_0, PAGE_ID, t_page_id);

}

int16_t BNO055Drv::read_accel_x_raw() {

    int16_t lsb, msb, ret_val;
    lsb = read(PAGE_0, ACC_DATA_X_LSB);
    msb = read(PAGE_0, ACC_DATA_X_MSB);
    ret_val = (msb << 8) | lsb;
    return ret_val;

}

int16_t BNO055Drv::read_accel_y_raw() {

    int16_t lsb, msb, ret_val;
    lsb = read(PAGE_0, ACC_DATA_Y_LSB);
    msb = read(PAGE_0, ACC_DATA_Y_MSB);
    ret_val = (msb << 8) | lsb;
    return ret_val;

}

int16_t BNO055Drv::read_accel_z_raw() {

    int16_t lsb, msb, ret_val;
    lsb = read(PAGE_0, ACC_DATA_Z_LSB);
    msb = read(PAGE_0, ACC_DATA_Z_MSB);
    ret_val = (msb << 8) | lsb;
    return ret_val;

}

BNO055Drv::accel_data_raw BNO055Drv::read_accel_xyz_raw() {

    accel_data_raw ret_val;
    ret_val.x = read_accel_x_raw();
    ret_val.y = read_accel_y_raw();
    ret_val.z = read_accel_z_raw();
    return ret_val;

}

int16_t BNO055Drv::read_mag_x_raw() {

    int16_t lsb, msb, ret_val;
    lsb = read(PAGE_0, MAG_DATA_X_LSB);
    msb = read(PAGE_0, MAG_DATA_X_MSB);
    ret_val = (msb << 8) | lsb;
    return ret_val;

}

int16_t BNO055Drv::read_mag_y_raw() {

    int16_t lsb, msb, ret_val;
    lsb = read(PAGE_0, MAG_DATA_Y_LSB);
    msb = read(PAGE_0, MAG_DATA_Y_MSB);
    ret_val = (msb << 8) | lsb;
    return ret_val;

}

int16_t BNO055Drv::read_mag_z_raw() {

    int16_t lsb, msb, ret_val;
    lsb = read(PAGE_0, MAG_DATA_Z_LSB);
    msb = read(PAGE_0, MAG_DATA_Z_MSB);
    ret_val = (msb << 8) | lsb;
    return ret_val;

}

BNO055Drv::mag_data_raw BNO055Drv::read_mag_xyz_raw() {

    mag_data_raw ret_val;
    ret_val.x = read_mag_x_raw();
    ret_val.y = read_mag_y_raw();
    ret_val.z = read_mag_z_raw();
    return ret_val;

}

int16_t BNO055Drv::read_gyro_x_raw() {

    int16_t lsb, msb, ret_val;
    lsb = read(PAGE_0, GYR_DATA_X_LSB);
    msb = read(PAGE_0, GYR_DATA_X_MSB);
    ret_val = (msb << 8) | lsb;
    return ret_val;

}

int16_t BNO055Drv::read_gyro_y_raw() {

    int16_t lsb, msb, ret_val;
    lsb = read(PAGE_0, GYR_DATA_X_LSB);
    msb = read(PAGE_0, GYR_DATA_X_MSB);
    ret_val = (msb << 8) | lsb;
    return ret_val;

}

int16_t BNO055Drv::read_gyro_z_raw() {

    int16_t lsb, msb, ret_val;
    lsb = read(PAGE_0, GYR_DATA_X_LSB);
    msb = read(PAGE_0, GYR_DATA_X_MSB);
    ret_val = (msb << 8) | lsb;
    return ret_val;

}

BNO055Drv::gyro_data_raw BNO055Drv::read_gyro_xyz_raw() {

    gyro_data_raw ret_val;
    ret_val.x = read_gyro_x_raw();
    ret_val.y = read_gyro_y_raw();
    ret_val.z = read_gyro_z_raw();
    return ret_val;

}

float BNO055Drv::read_accel_x_float() {

    float divisor = (m_accel_unit) ? 1.0 : 100.0;
    return read_accel_x_raw()/divisor;

}

float BNO055Drv::read_accel_y_float() {

    float divisor = (m_accel_unit) ? 1.0 : 100.0;
    return read_accel_y_raw()/divisor;

}

float BNO055Drv::read_accel_z_float() {

    float divisor = (m_accel_unit) ? 1.0 : 100.0;
    return read_accel_z_raw()/divisor;

}

BNO055Drv::accel_data_float BNO055Drv::read_accel_xyz_float() {

    accel_data_float ret_val;
    ret_val.x = read_accel_x_float();
    ret_val.y = read_accel_y_float();
    ret_val.z = read_accel_z_float();
     return ret_val;

}

float BNO055Drv::read_mag_x_float() {

    return read_mag_x_raw()/16.0;

}

float BNO055Drv::read_mag_y_float() {

    return read_mag_y_raw()/16.0;

}

float BNO055Drv::read_mag_z_float() {

    return read_mag_z_raw()/16.0;

}

BNO055Drv::mag_data_float BNO055Drv::read_mag_xyz_float() {

    mag_data_float ret_val;
    ret_val.x = read_mag_x_float();
    ret_val.y = read_mag_y_float();
    ret_val.z = read_mag_z_float();
    return ret_val;

}

float BNO055Drv::read_gyro_x_float() {

    float divisor = (m_gyro_unit) ? 900.0 : 16.0;
    return read_gyro_x_raw()/divisor;

}

float BNO055Drv::read_gyro_y_float() {

    float divisor = (m_gyro_unit) ? 900.0 : 16.0;
    return read_gyro_y_raw()/divisor;

}

float BNO055Drv::read_gyro_z_float() {

    float divisor = (m_gyro_unit) ? 900.0 : 16.0;
    return read_gyro_z_raw()/divisor;

}

BNO055Drv::gyro_data_float BNO055Drv::read_gyro_xyz_float() {

    gyro_data_float ret_val;
    ret_val.x = read_gyro_x_float();
    ret_val.y = read_gyro_y_float();
    ret_val.z = read_gyro_z_float();
    return ret_val;

}

uint8_t BNO055Drv::get_operation_mode() {

    return (read(PAGE_0, OPR_MODE) & 0x0F);

}

void BNO055Drv::set_operation_mode(uint8_t t_operation_mode) {

    uint8_t prev_operation_mode = m_operation_mode;
    if (prev_operation_mode == CONFIGMODE) {
        write(PAGE_0, OPR_MODE, t_operation_mode);
        SysCtlDelay(SysCtlClockGet()/426);
    } else {
        write(PAGE_0, OPR_MODE, CONFIGMODE);
        SysCtlDelay(SysCtlClockGet()/159);
        if (t_operation_mode != CONFIGMODE) {
            write(PAGE_0, OPR_MODE, t_operation_mode);
            SysCtlDelay(SysCtlClockGet()/426);
        }
    }
    m_operation_mode = t_operation_mode;

}

uint8_t BNO055Drv::get_power_mode() {

    return (read(PAGE_0, PWR_MODE) & 0x03);

}

void BNO055Drv::set_power_mode(uint8_t t_power_mode) {

    uint8_t prev_operation_mode = m_operation_mode;
    if (prev_operation_mode != CONFIGMODE) {
        set_operation_mode(CONFIGMODE);
    }
    m_power_mode = t_power_mode;
    write(PAGE_0, PWR_MODE, t_power_mode);
    if (prev_operation_mode != CONFIGMODE) {
        set_operation_mode(prev_operation_mode);
    }

}

uint8_t BNO055Drv::get_accel_unit() {

    return (read(PAGE_0, UNIT_SEL) & 0x01);

}

void BNO055Drv::set_accel_unit(uint8_t t_accel_unit) {

    uint8_t prev_operation_mode = m_operation_mode;
    if (prev_operation_mode != CONFIGMODE) {
        set_operation_mode(CONFIGMODE);
    }
    m_accel_unit = t_accel_unit;
    write(PAGE_0, PWR_MODE, (read(PAGE_0, UNIT_SEL) & ~(0x01)) | t_accel_unit);
    if (prev_operation_mode != CONFIGMODE) {
        set_operation_mode(prev_operation_mode);
    }

}

uint8_t BNO055Drv::get_gyro_unit() {

    return (read(PAGE_0, UNIT_SEL) & 0x02);

}

void BNO055Drv::set_gyro_unit(uint8_t t_gyro_unit) {

    uint8_t prev_operation_mode = m_operation_mode;
    if (prev_operation_mode != CONFIGMODE) {
        set_operation_mode(CONFIGMODE);
    }
    m_gyro_unit = t_gyro_unit;
    write(PAGE_0, PWR_MODE, (read(PAGE_0, UNIT_SEL) & ~(0x02)) | t_gyro_unit);
    if (prev_operation_mode != CONFIGMODE) {
        set_operation_mode(prev_operation_mode);
    }

}


uint8_t BNO055Drv::read(const uint8_t & t_page,const uint8_t & t_register) {

    //
    //
    //
    if (m_page != t_page) {
        I2CDrv::instance().send(m_unit, RegisterMapPage1::PAGE_ID, t_page);
        m_page = t_page;
    }

    //
    //
    //
    uint32_t ret_val = I2CDrv::instance().receive(m_unit, t_register);
    return ret_val;

}

void BNO055Drv::write(const uint8_t & t_page, const uint8_t & t_register, const uint8_t & t_data) {

    //
    //
    //
    if (m_page != t_page) {
        I2CDrv::instance().send(m_unit, RegisterMapPage1::PAGE_ID, t_page);
        m_page = t_page;
    }

    //
    //
    //
    I2CDrv::instance().send(m_unit, t_register, t_data);

}
