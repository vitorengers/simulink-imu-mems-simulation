// -*-c++-*-

/*!
  \file bno055_drv.h
  \brief
 */

#ifndef __IMU_BOARD_BNO055_DRV_H__
#define __IMU_BOARD_BNO055_DRV_H__

#include <stdint.h>

/*!

*/

class BNO055Drv
{
public:

    /*!

    */
    enum Unit {
        BNO_1 = 0x28,
        BNO_2 = 0x29,
    };

    /*!

    */
    enum PageId {
        PAGE_0 = 0x00,
        PAGE_1 = 0x01,
    };

    /*!

    */
    enum OperationMode {
        CONFIGMODE = 0x00,
        ACCONLY = 0x01,
        MAGONLY = 0x02,
        GYROONLY = 0x03,
        ACCMAG = 0x04,
        ACCGYRO = 0x05,
        MAGGYRO = 0x06,
        AMG = 0x07,
        IMU = 0x08,
        COMPASS = 0x09,
        M4G = 0x0A,
        NDOF_FMC_OFF = 0x0B,
        NDOF = 0x0C,
    };

    /*!

    */
    enum PowerMode {
        NORMAL_MODE = 0x00,
        LOW_POWER_MODE = 0x01,
        SUSPEND_MODE = 0x02,
    };

    /*!

    */
    enum AccelerationUnit {
        MS2 = 0x00,
        MG = 0x01,
    };

    /*!

    */
    enum AngularRateUnit {
        DPS = 0x00,
        RPS = 0x02,
    };

    /*!

    */
    struct accel_data_raw {
        int16_t x;
        int16_t y;
        int16_t z;
    };

    /*!

    */
    struct mag_data_raw {
        int16_t x;
        int16_t y;
        int16_t z;
    };

    /*!

    */
    struct gyro_data_raw {
        int16_t x;
        int16_t y;
        int16_t z;
    };

    /*!

    */
    struct accel_data_float {
        float x;
        float y;
        float z;
    };

    /*!

    */
    struct mag_data_float {
        float x;
        float y;
        float z;
    };

    /*!

    */
    struct gyro_data_float {
        float x;
        float y;
        float z;
    };

    /*!

    */
    static BNO055Drv& instance(const uint8_t& t_unit);

    /*!

    */
    static const BNO055Drv& i(const uint8_t& t_unit) {
        return instance(t_unit);
    }

    /*!

    */
    void init();

    /*!

    */
    uint8_t read_chip_id();

    /*!

    */
    uint8_t read_page_id();

    /*!

    */
    void write_page_id(uint8_t t_page_id);

    /*!

    */
    int16_t read_accel_x_raw();

    /*!

    */
    int16_t read_accel_y_raw();

    /*!

    */
    int16_t read_accel_z_raw();

    /*!

    */
    accel_data_raw read_accel_xyz_raw();

    /*!

    */
    int16_t read_mag_x_raw();

    /*!

    */
    int16_t read_mag_y_raw();

    /*!

    */
    int16_t read_mag_z_raw();

    /*!

    */
    mag_data_raw read_mag_xyz_raw();

    /*!

    */
    int16_t read_gyro_x_raw();

    /*!

    */
    int16_t read_gyro_y_raw();

    /*!

    */
    int16_t read_gyro_z_raw();

    /*!

    */
    gyro_data_raw read_gyro_xyz_raw();

    /*!

    */
    float read_accel_x_float();

    /*!

    */
    float read_accel_y_float();

    /*!

    */
    float read_accel_z_float();

    /*!

    */
    accel_data_float read_accel_xyz_float();

    /*!

    */
    float read_mag_x_float();

    /*!

    */
    float read_mag_y_float();

    /*!

    */
    float read_mag_z_float();

    /*!

    */
    mag_data_float read_mag_xyz_float();

    /*!

    */
    float read_gyro_x_float();

    /*!

    */
    float read_gyro_y_float();

    /*!

    */
    float read_gyro_z_float();

    /*!

    */
    gyro_data_float read_gyro_xyz_float();


    /*!

    */
    uint8_t get_operation_mode();

    /*!

    */
    void set_operation_mode(uint8_t t_operation_mode);

    /*!

    */
    uint8_t get_power_mode();

    /*!

    */
    void set_power_mode(uint8_t t_power_mode);

    /*!

    */
    uint8_t get_accel_unit();

    /*!

    */
    void set_accel_unit(uint8_t t_accel_unit);

    /*!

    */
    uint8_t get_gyro_unit();

    /*!

    */
    void set_gyro_unit(uint8_t t_gyro_unit);

private:

    /*!

    */
    enum RegisterMapPage1 {
        CHIP_ID          = 0x00,
        ACC_ID           = 0x01,
        MAG_ID           = 0x02,
        GYR_ID           = 0x03,
        SW_REV_ID_LSB    = 0x04,
        SW_REV_ID_MSB    = 0x05,
        BL_REV_ID        = 0x06,
        PAGE_ID          = 0x07,
        ACC_DATA_X_LSB   = 0x08,
        ACC_DATA_X_MSB   = 0x09,
        ACC_DATA_Y_LSB   = 0x0A,
        ACC_DATA_Y_MSB   = 0x0B,
        ACC_DATA_Z_LSB   = 0x0C,
        ACC_DATA_Z_MSB   = 0x0D,
        MAG_DATA_X_LSB   = 0x0E,
        MAG_DATA_X_MSB   = 0x0F,
        MAG_DATA_Y_LSB   = 0x10,
        MAG_DATA_Y_MSB   = 0x11,
        MAG_DATA_Z_LSB   = 0x12,
        MAG_DATA_Z_MSB   = 0x13,
        GYR_DATA_X_LSB   = 0x14,
        GYR_DATA_X_MSB   = 0x15,
        GYR_DATA_Y_LSB   = 0x16,
        GYR_DATA_Y_MSB   = 0x17,
        GYR_DATA_Z_LSB   = 0x18,
        GYR_DATA_Z_MSB   = 0x19,
        EUL_HEADING_LSB  = 0x1A,
        EUL_HEADING_MSB  = 0x1B,
        EUL_ROLL_LSB     = 0x1C,
        EUL_ROLL_MSB     = 0x1D,
        EUL_PITCH_LSB    = 0x1E,
        EUL_PITCH_MSB    = 0x1F,
        QUA_DATA_W_LSB   = 0x20,
        QUA_DATA_W_MSB   = 0x21,
        QUA_DATA_X_LSB   = 0x22,
        QUA_DATA_X_MSB   = 0x23,
        QUA_DATA_Y_LSB   = 0x24,
        QUA_DATA_Y_MSB   = 0x25,
        QUA_DATA_Z_LSB   = 0x26,
        QUA_DATA_Z_MSB   = 0x27,
        LIA_DATA_X_LSB   = 0x28,
        LIA_DATA_X_MSB   = 0x29,
        LIA_DATA_Y_LSB   = 0x2A,
        LIA_DATA_Y_MSB   = 0x2B,
        LIA_DATA_Z_LSB   = 0x2C,
        LIA_DATA_Z_MSB   = 0x2D,
        GRV_DATA_X_LSB   = 0x2E,
        GRV_DATA_X_MSB   = 0x2F,
        GRV_DATA_Y_LSB   = 0x30,
        GRV_DATA_Y_MSB   = 0x31,
        GRV_DATA_Z_LSB   = 0x32,
        GRV_DATA_Z_MSB   = 0x33,
        TEMP             = 0x34,
        CALIB_STAT       = 0x35,
        ST_RESULT        = 0x36,
        INT_STA          = 0x37,
        SYS_CLK_STATUS   = 0x38,
        SYS_STATUS       = 0x39,
        SYS_ERR          = 0x3A,
        UNIT_SEL         = 0x3B,
        OPR_MODE         = 0x3D,
        PWR_MODE         = 0x3E,
        SYS_TRIGGER      = 0x3F,
        TEMP_SOURCE      = 0x40,
        AXIS_MAP_CONFIG  = 0x41,
        AXIS_MAP_SIGN    = 0x42,
        ACC_OFFSET_X_LSB = 0x55,
        ACC_OFFSET_X_MSB = 0x56,
        ACC_OFFSET_Y_LSB = 0x57,
        ACC_OFFSET_Y_MSB = 0x58,
        ACC_OFFSET_Z_LSB = 0x59,
        ACC_OFFSET_Z_MSB = 0x5A,
        MAG_OFFSET_X_LSB = 0x5B,
        MAG_OFFSET_X_MSB = 0x5C,
        MAG_OFFSET_Y_LSB = 0x5D,
        MAG_OFFSET_Y_MSB = 0x5E,
        MAG_OFFSET_Z_LSB = 0x5F,
        MAG_OFFSET_Z_MSB = 0x60,
        GYR_OFFSET_X_LSB = 0x61,
        GYR_OFFSET_X_MSB = 0x62,
        GYR_OFFSET_Y_LSB = 0x63,
        GYR_OFFSET_Y_MSB = 0x64,
        GYR_OFFSET_Z_LSB = 0x65,
        GYR_OFFSET_Z_MSB = 0x66,
        ACC_RADIUS_LSB   = 0x67,
        ACC_RADIUS_MSB   = 0x68,
        MAG_RADIUS_LSB   = 0x69,
        MAG_RADIUS_MSB   = 0x6A,
    };

    /*!

     */
    enum RegisterMapPage2 {
        ACC_CONFIG       = 0x08,
        MAG_CONFIG       = 0x09,
        GYR_CONFIG_0     = 0x0A,
        GYR_CONFIG_1     = 0x0B,
        ACC_SLEEP_CONFIG = 0x0C,
        GYR_SLEEP_CONFIG = 0x0D,
        INT_MASK         = 0x0F,
        INT_EN           = 0x10,
        ACC_AM_THRES     = 0x11,
        ACC_INT_SETTINGS = 0x12,
        ACC_HG_DURATION  = 0x13,
        ACC_HG_THRES     = 0x14,
        ACC_NM_THRES     = 0x15,
        ACC_NM_SET       = 0x16,
        GYR_INT_SETTING  = 0x17,
        GYR_HR_X_SET     = 0x18,
        GYR_DUR_X        = 0x19,
        GYR_HR_Y_SET     = 0x1A,
        GYR_DUR_Y        = 0x1B,
        GYR_HR_Z_SET     = 0x1C,
        GYR_DUR_Z        = 0x1D,
        GYR_AM_THRES     = 0x1E,
        GYR_AM_SET       = 0x1F,
    };

    /*!

     */
    BNO055Drv(const uint8_t & t_unit);

    /*!

    */
    uint8_t read(const uint8_t & t_page, const uint8_t & t_register);

    /*!

    */
    void write(const uint8_t & t_page, const uint8_t & t_register, const uint8_t & t_data);

    //!
    bool m_init;

    //!
    uint8_t m_unit;

    //!
    uint8_t m_page;

    //!
    uint8_t m_operation_mode;

    //!
    uint8_t m_power_mode;

    //!
    uint8_t m_accel_unit;

    //!
    uint8_t m_gyro_unit;

};

#endif
