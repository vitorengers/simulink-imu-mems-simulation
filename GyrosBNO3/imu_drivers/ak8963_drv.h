// -*-c++-*-

/*!
  \file ak8963_drv.h
  \brief
 */

#ifndef __IMU_BOARD_AK8963_DRV_H__
#define __IMU_BOARD_AK8963_DRV_H__

#include <stdint.h>
#include "data_format.h"

/*!
  \class AK8963
  \brief
 */
class AK8963Drv
{
public:

    /*!

     */
    enum Unit {
        AK8963_1,
        AK8963_2,
        AK8963_3,
        AK8963_4,
    };

    /*!

     */
    enum RegisterMap {
        WIA     = 0x00,
        INFO    = 0x01,
        ST1     = 0x02,
        HXL     = 0x03,
        HXH     = 0x04,
        HYL     = 0x05,
        HYH     = 0x06,
        HZL     = 0x07,
        HZH     = 0x08,
        ST2     = 0x09,
        CNTL    = 0x0A,
        RSV     = 0x0B,
        ASTC    = 0x0C,
        TS1     = 0x0D,
        TS2     = 0x0E,
        I2CDIS  = 0x0F,
        ASAX    = 0x10,
        ASAY    = 0x11,
        ASAZ    = 0x12,
    };

    /*!

    */
    static AK8963Drv & instance(const uint8_t & t_unit);

    /*!

    */
    static const AK8963Drv & i(const uint8_t & t_unit) {
        return instance(t_unit);
    }

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
    RawData dataMagGetRaw();

    /*!

     */
    FloatData dataMagGetFloat();

private:

    /*!

     */
    AK8963Drv(const uint8_t & t_unit);

    //!
    uint8_t m_unit;

    //!
    uint8_t m_mag_fs_sel;

    //!
    uint8_t m_data_buffer[6];

};

#endif
