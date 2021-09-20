/*! \file structcmd.h
    \brief A Documented file.

    Details.
*/

#ifndef __IMU_BOARD_LSM9DS1_ACCEL_GYRO_H__
#define __IMU_BOARD_LSM9DS1_ACCEL_GYRO_H__

//!  A test class.
/*!
  A more elaborate class description.
*/
#if 0
class LSM9DS1AccelGyro
{
public:

    //! An enum.
    /*! More detailed enum description. */
    enum {
        ACT_THS          = 0x04,    /*!< Enum value TVal1. */
        ACT_DUR          = 0x05,
        INT_GEN_CFG_XL   = 0x06,
        INT_GEN_THS_X_XL = 0x07,
        INT_GEN_THS_Y_XL = 0x08,
        INT_GEN_THS_Z_XL = 0x09,
        REFERENCE_G      = 0x0A,
        INT1_CTRL        = 0x0B,
        INT2_CTRL        = 0x0C,
        WHO_AM_I         = 0x0F,
        CTRL_REG1_G      = 0x10,
        CTRL_REG2_G      = 0x10,
        CTRL_REG3_G      = 0x10,
        ORIENT_CFG_G     = 0x10,
        INT_GEN_SRC_G    = 0x10,
        OUT_TEMP_L       = 0x10,
        OUT_TEMP_H       = 0x10,
        STATUS_REG_G     = 0x10,
        OUT_X_L_G        = 0x10,
        OUT_X_H_G        = 0x10,
        OUT_Y_L_G        = 0x10,
        OUT_Y_H_G        = 0x10,
        OUT_Z_L_G        = 0x10,
        OUT_Z_H_G        = 0x10,
        CTRL_REG4        = 0x10,
        CTRL_REG5_XL     = 0x10,
        CTRL_REG6_XL     = 0x10,
        CTRL_REG7_XL     = 0x10,
        CTRL_REG8        = 0x10,
        CTRL_REG9        = 0x10,
        CTRL_REG10       = 0x10,
        INT_GEN_SRC_XL   = 0x10,
        STATUS_REG_XL    = 0x10,
        OUT_X_L_XL       = 0x10,
        OUT_X_H_XL       = 0x10,
        OUT_Y_L_XL       = 0x10,
        OUT_Y_H_XL       = 0x10,
        OUT_Z_L_XL       = 0x10,
        OUT_Z_H_XL       = 0x10,
        FIFO_CTRL        = 0x10,
        FIFO_SRC         = 0x10,
        INT_GEN_CFG_G    = 0x10,
        INT_GEN_THS_XH_G = 0x10,
        INT_GEN_THS_XL_G = 0x10,
        INT_GEN_THS_YH_G = 0x10,
        INT_GEN_THS_YL_G = 0x10,
        INT_GEN_THS_ZH_G = 0x10,
        INT_GEN_THS_ZL_G = 0x10,
        INT_GEN_DUR_G    = 0x10,
    };

    //! A constructor.
    /*!
      A more elaborate description of the constructor.
    */
    LSM9DS1AccelGyro();

    //! A destructor.
    /*!
      A more elaborate description of the destructor.
    */
    ~LSM9DS1AccelGyro();

    //! A normal member taking two arguments and returning an integer value.
    /*!
      \param a an integer argument.
      \param s a constant character pointer.
      \return The test results
      \sa QTstyle_Test(), ~QTstyle_Test(), testMeToo() and publicVar()
    */
    uint8_t init();

    //! A normal member taking two arguments and returning an integer value.
    /*!
      \param a an integer argument.
      \param s a constant character pointer.
      \return The test results
      \sa QTstyle_Test(), ~QTstyle_Test(), testMeToo() and publicVar()
    */
    uint8_t read();

    //! A normal member taking two arguments and returning an integer value.
    /*!
      \param a an integer argument.
      \param s a constant character pointer.
      \return The test results
      \sa QTstyle_Test(), ~QTstyle_Test(), testMeToo() and publicVar()
    */
    uint8_t write();

    //! A normal member taking two arguments and returning an integer value.
    /*!
      \param a an integer argument.
      \param s a constant character pointer.
      \return The test results
      \sa QTstyle_Test(), ~QTstyle_Test(), testMeToo() and publicVar()
    */
    uint8_t readWriteModify();

    //! A normal member taking two arguments and returning an integer value.
    /*!
      \param a an integer argument.
      \param s a constant character pointer.
      \return The test results
      \sa QTstyle_Test(), ~QTstyle_Test(), testMeToo() and publicVar()
    */
    uint8_t dataRead();

    //! A normal member taking two arguments and returning an integer value.
    /*!
      \param a an integer argument.
      \param s a constant character pointer.
      \return The test results
      \sa QTstyle_Test(), ~QTstyle_Test(), testMeToo() and publicVar()
    */
    void dataAccelGetRaw();

    //! A normal member taking two arguments and returning an integer value.
    /*!
      \param a an integer argument.
      \param s a constant character pointer.
      \return The test results
      \sa QTstyle_Test(), ~QTstyle_Test(), testMeToo() and publicVar()
    */
    void dataAccelGetFloat();

    //! A normal member taking two arguments and returning an integer value.
    /*!
      \param a an integer argument.
      \param s a constant character pointer.
      \return The test results
      \sa QTstyle_Test(), ~QTstyle_Test(), testMeToo() and publicVar()
    */
    void dataGyroGetRaw();

    //! A normal member taking two arguments and returning an integer value.
    /*!
      \param a an integer argument.
      \param s a constant character pointer.
      \return The test results
      \sa QTstyle_Test(), ~QTstyle_Test(), testMeToo() and publicVar()
    */
    void dataGyroGetFloat();

private:

    //! A public variable.
    /*!
      Details.
    */
    SpiDrv m_spi_drv;

    //! A public variable.
    /*!
      Details.
    */
    uint8_t m_accel_fs_sel;

    //! A public variable.
    /*!
      Details.
    */
    uint8_t m_gyro_fs_sel;

    //! A public variable.
    /*!
      Details.
    */
    uint8_t m_data_buffer[12];

};
#endif

#endif /* __IMU_BOARD_LSM9DS1_ACCEL_GYRO_H__ */
