// -*-c++-*-

/*!
  \file spi_drv.h
  \brief A Documented file.
 */

#ifndef __IMU_BOARD_SPI_DRV_H__
#define __IMU_BOARD_SPI_DRV_H__

#include <stdint.h>

/*!
  \class SPIDrv
  \brief
 */
class SPIDrv
{
public:

    /*!
      \brief
      \return
     */
    static SPIDrv & instance();

    /*!
      \brief
      \return
     */
    static const SPIDrv & i() {
        return instance();
    }

    /*!
      \brief
      \param t_bit_rate
     */
    void bitRate(const uint32_t & t_bit_rate);

    /*!
      \brief
      \param t_data_width
     */
    void dataWidth(const uint32_t & t_data_width);

    /*!
      \brief
      \return
     */
    uint32_t bitRate() const {
        return m_bit_rate;
    }

    /*!
      \brief
      \return
     */
    uint32_t dataWidth() const {
        return m_data_width;
    }

    /*!
      \brief
      \param t_register
      \param t_data
     */
    void send(const uint32_t & t_register, const uint32_t & t_data);

    /*!
      \brief
      \param t_register
      \return
     */
    uint32_t receive(const uint32_t & t_register);

private:

    /*!
      \brief
     */
    SPIDrv();

    //!
    uint32_t m_bit_rate;

    //!
    uint32_t m_data_width;

};

#endif
