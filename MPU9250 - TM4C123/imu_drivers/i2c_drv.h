// -*-c++-*-

/*!
  \file i2c_drv.h
  \brief A Documented file.
 */

#ifndef __IMU_BOARD_I2C_DRV_H__
#define __IMU_BOARD_I2C_DRV_H__

#include <stdint.h>

/*!
  \class SPIDrv
  \brief
 */
class I2CDrv
{
public:

    /*!
      \brief
      \return
     */
    static I2CDrv & instance();

    /*!
      \brief
      \return
     */
    static const I2CDrv & i() {
        return instance();
    }

    /*!
      \brief
      \param t_register
      \param t_data
     */
    void send(const uint32_t & t_addr, const uint32_t & t_register, const uint32_t & t_data);

    /*!
      \brief
      \param t_register
      \return
     */
    uint32_t receive(const uint32_t & t_addr, const uint32_t & t_register);

private:

    /*!
      \brief
     */
    I2CDrv();

};

#endif
