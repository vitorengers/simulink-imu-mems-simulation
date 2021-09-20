// -*-c++-*-

/*!
  \file scc2130_drv.h
  \brief A Documented file.
 */

#ifndef __IMU_BOARD_SCC2130_DRV_H__
#define __IMU_BOARD_SCC2130_DRV_H__

#include <stdint.h>
#include "data_format.h"

/*!

*/

class SCC2130Drv
{
public:

    /*!

     */
    enum Unit {
        SCC_1,
        SCC_2
    };

    /*!

     */
    enum RegisterMap {
        SELF_TEST_X_GYRO    = 0x00,
        SELF_TEST_Y_GYRO    = 0x01,
        SELF_TEST_Z_GYRO    = 0x02,
        SELF_TEST_X_ACCEL   = 0x0D,
        SELF_TEST_Y_ACCEL   = 0x0E,
        SELF_TEST_Z_ACCEL   = 0x0F,
    };

    /*!

     */
    SCC2130Drv(const uint8_t & t_unit);

    /*!

     */
    void init();

    /*!

     */
    uint8_t read(const uint8_t & t_register);

    /*!

     */
    void write(const uint8_t & t_register, const uint8_t & t_data);

    /*!

     */
    void dataRead();

    /*!

     */
    RawData dataAccelGetRaw();

    /*!

     */
    FloatData dataAccelGetFloat();

    /*!

     */
    RawData dataGyroGetRaw();

    /*!

     */
    FloatData dataGyroGetFloat();

private:

    //!
    uint8_t m_unit;

    //!
    uint8_t m_accel_fs_sel;

    //!
    uint8_t m_gyro_fs_sel;

    //!
    uint8_t m_data_buffer[12];

};

#endif
