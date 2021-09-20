/*! \file structcmd.h
    \brief A Documented file.

    Details.
*/

#ifndef __IMU_BOARD_LSM9DS1_MAG_H__
#define __IMU_BOARD_LSM9DS1_MAG_H__

//!  A test class.
/*!
  A more elaborate class description.
*/

class LSM9DS1Mag
{
public:

    /*!

     */
    enum Unit {
        MPU_1,
        MPU_2,
        MPU_3,
        MPU_4,
    };

    //! An enum.
    /*! More detailed enum description. */
    enum {
        OFFSET_X_REG_L_M = 0x05,    /*!< Enum value TVal1. */
        OFFSET_X_REG_H_M = 0x06,
        OFFSET_Y_REG_L_M = 0x07,
        OFFSET_Y_REG_H_M = 0x08,
        OFFSET_Z_REG_L_M = 0x09,
        OFFSET_Z_REG_H_M = 0x0A,
        WHO_AM_I_M       = 0x0F,
        CTRL_REG1_M      = 0x20,
        CTRL_REG2_M      = 0x21,
        CTRL_REG3_M      = 0x22,
        CTRL_REG4_M      = 0x23,
        CTRL_REG5_M      = 0x24,
        STATUS_REG_M     = 0x27,
        OUT_X_L_M        = 0x28,
        OUT_X_H_M        = 0x29,
        OUT_Y_L_M        = 0x2A,
        OUT_Y_H_M        = 0x2B,
        OUT_Z_L_M        = 0x2C,
        OUT_Z_H_M        = 0x2D,
        INT_CFG_M        = 0x30,
        INT_SRC_M        = 0x31,
        INT_THS_L_M      = 0x32,
        INT_THS_H_M      = 0x33,
    };

    //! A constructor.
    /*!
      A more elaborate description of the constructor.
    */
    LSM9DS1Mag();

    //! A normal member taking two arguments and returning an integer value.
    /*!
      \param a an integer argument.
      \param s a constant character pointer.
      \return The test results
      \sa QTstyle_Test(), ~QTstyle_Test(), testMeToo() and publicVar()
    */
    uint_fast8_t init();

    //! A normal member taking two arguments and returning an integer value.
    /*!
      \param a an integer argument.
      \param s a constant character pointer.
      \return The test results
      \sa QTstyle_Test(), ~QTstyle_Test(), testMeToo() and publicVar()
    */
    uint_fast8_t read();

    //! A normal member taking two arguments and returning an integer value.
    /*!
      \param a an integer argument.
      \param s a constant character pointer.
      \return The test results
      \sa QTstyle_Test(), ~QTstyle_Test(), testMeToo() and publicVar()
    */
    uint_fast8_t write();

    //! A normal member taking two arguments and returning an integer value.
    /*!
      \param a an integer argument.
      \param s a constant character pointer.
      \return The test results
      \sa QTstyle_Test(), ~QTstyle_Test(), testMeToo() and publicVar()
    */
    uint_fast8_t readWriteModify();

    //! A normal member taking two arguments and returning an integer value.
    /*!
      \param a an integer argument.
      \param s a constant character pointer.
      \return The test results
      \sa QTstyle_Test(), ~QTstyle_Test(), testMeToo() and publicVar()
    */
    uint_fast8_t dataRead();

    //! A normal member taking two arguments and returning an integer value.
    /*!
      \param a an integer argument.
      \param s a constant character pointer.
      \return The test results
      \sa QTstyle_Test(), ~QTstyle_Test(), testMeToo() and publicVar()
    */
    void dataMagGetRaw();

    //! A normal member taking two arguments and returning an integer value.
    /*!
      \param a an integer argument.
      \param s a constant character pointer.
      \return The test results
      \sa QTstyle_Test(), ~QTstyle_Test(), testMeToo() and publicVar()
    */
    void dataMagGetFloat();

private:

    //! A public variable.
    /*!
      Details.
    */
    uint8_t m_mag_fs_sel;

    //! A public variable.
    /*!
      Details.
    */
    uint8_t m_data_buffer[6];

};

#endif /* __IMU_BOARD_LSM9DS1_MAG_H__ */

