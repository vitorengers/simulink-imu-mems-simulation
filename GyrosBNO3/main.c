#include <stdbool.h>
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ssi.h"
#include "inc/hw_i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/fpu.h"
#include "driverlib/ssi.h"
#include "driverlib/i2c.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"

#include "uartstdio.h"
#include <math.h>
//#define SELF_TEST_X_GYRO     0x00
//#define SMPLRT_DIV           0x19
//#define CONFIG               0x1A
//#define  PWR_MGMT_1           0x6B
//#define  PWR_MGMT_2           0x6C
//#define GYRO_CONFIG          0x1B
//#define INT_PIN_CFG          0x37
//#define  INT_ENABLE           0x38
//#define BITS_DLPF_CFG_188HZ         0x01
//#define MPUREG_CONFIG 0x1A
//#define MPUREG_GYRO_CONFIG 0x1B
//#define BITS_FS_250DPS              0x00
//#define MPUREG_TEMP_OUT_H 0x41
//#define MPUREG_TEMP_OUT_L 0x42
//

//DEFINES BNO
//operation mode
#define CONFIGMODE 0x00
#define MAGONLY 0x02
#define GYROONLY 0x03
#define ACCGYRO 0x05

//power mode
#define NORMAL_MODE 0x00

//page id
#define PAGE_0 0x00
#define PAGE_1 0x01

//gyro unit
#define DPS 0x00

//registers page 0
#define GYR_DATA_X_LSB   0x14
#define GYR_DATA_X_MSB   0x15
#define GYR_DATA_Y_LSB   0x16
#define GYR_DATA_Y_MSB   0x17
#define GYR_DATA_Z_LSB   0x18
#define GYR_DATA_Z_MSB   0x19
#define TEMP             0x34
#define UNIT_SEL         0x3B
#define OPR_MODE         0x3D
#define PWR_MODE         0x3E
#define TEMP_SOURCE      0x40



//registers page 1
#define GYR_CONFIG_0     0x0A
#define GYR_CONFIG_1     0x0B
#define GYR_SLEEP_CONFIG 0x0D
#define INT_MASK         0x0F
#define OPR_MODE         0x3D
#define PAGE_ID          0x07

#define BNO_1 0x28

uint8_t m_unit = BNO_1;
uint8_t m_page = 0;
uint8_t m_operation_mode = CONFIGMODE;
uint8_t m_power_mode = NORMAL_MODE;
uint8_t m_gyro_unit = DPS;

void I2C_send (uint32_t t_addr, uint32_t t_register, uint32_t t_data) {

//    I2CMasterSlaveAddrSet(I2C0_BASE,0x28,false);
//    ulDataTx = 0x00; //register
//    I2CMasterDataPut(I2C0_BASE,ulDataTx);
//    I2CMasterControl(I2C0_BASE,I2C_MASTER_CMD_BURST_SEND_START);
//    while(I2CMasterBusy(I2C0_BASE));
//    I2CMasterSlaveAddrSet(I2C0_BASE,0x28, true);
//    I2CMasterControl(I2C0_BASE,I2C_MASTER_CMD_SINGLE_RECEIVE);
//    while(I2CMasterBusy(I2C0_BASE));
//    ulDataRx = I2CMasterDataGet(I2C0_BASE);
//    ulDataRx = I2CMasterDataGet(I2C0_BASE);


    // Tell the master module what address it will place on the bus when
    // communicating with the slave.
    I2CMasterSlaveAddrSet(I2C0_BASE, t_addr, false);

    //put data to be sent into FIFO
    I2CMasterDataPut(I2C0_BASE, t_register);

    //Initiate send of data from the MCU
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);

    // Wait until MCU is done transferring.
    while(I2CMasterBusy(I2C0_BASE));

    //put last piece of data into I2C FIFO
    I2CMasterDataPut(I2C0_BASE, t_data);

    //send next data that was just placed into FIFO
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);

    // Wait until MCU is done transferring.
    while(I2CMasterBusy(I2C0_BASE));

}

uint32_t I2C_receive(uint32_t t_addr, uint32_t t_register) {

    //specify that we are writing (a register address) to the
    //slave device
    I2CMasterSlaveAddrSet(I2C0_BASE, t_addr, false);

    //specify register to be read
    I2CMasterDataPut(I2C0_BASE, t_register);

    //send control byte and register address byte to slave device
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);

    //wait for MCU to finish transaction
    while(I2CMasterBusy(I2C0_BASE));

    //specify that we are going to read from slave device
    I2CMasterSlaveAddrSet(I2C0_BASE, t_addr, true);

    //send control byte and read from the register we
    //specified
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);

    //wait for MCU to finish transaction
    while(I2CMasterBusy(I2C0_BASE));

    //return data pulled from the specified register
    I2CMasterDataGet(I2C0_BASE);
    return I2CMasterDataGet(I2C0_BASE);

}

void BNO055_write(uint8_t t_page, uint8_t t_register, uint8_t t_data) {

    if (m_page != t_page) {
        I2C_send(m_unit, PAGE_ID, t_page);
        m_page = t_page;
    }

    I2C_send(m_unit, t_register, t_data);

}

uint8_t BNO055_read(uint8_t t_page, uint8_t t_register) {

    if (m_page != t_page) {
        I2C_send(m_unit, PAGE_ID, t_page);
        m_page = t_page;
    }

    uint32_t ret_val = I2C_receive(m_unit, t_register);
    //UARTprintf("ret_val: %d\n", ret_val);
    return ret_val;
}

void BNO055_set_operation_mode(uint8_t t_operation_mode) {

    uint8_t prev_operation_mode = m_operation_mode;
    if (prev_operation_mode == CONFIGMODE) {
        BNO055_write(PAGE_0, OPR_MODE, t_operation_mode);
        SysCtlDelay(SysCtlClockGet()/426);
    } else {
        BNO055_write(PAGE_0, OPR_MODE, CONFIGMODE);
        SysCtlDelay(SysCtlClockGet()/159);
        if (t_operation_mode != CONFIGMODE) {
            BNO055_write(PAGE_0, OPR_MODE, t_operation_mode);
            SysCtlDelay(SysCtlClockGet()/426);
        }
    }
    m_operation_mode = t_operation_mode;
}

void BNO055_set_power_mode(uint8_t t_power_mode) {

    uint8_t prev_operation_mode = m_operation_mode;
    if (prev_operation_mode != CONFIGMODE) {
        BNO055_set_operation_mode(CONFIGMODE);
    }
    m_power_mode = t_power_mode;
    BNO055_write(PAGE_0, PWR_MODE, t_power_mode);
    if (prev_operation_mode != CONFIGMODE) {
        BNO055_set_operation_mode(prev_operation_mode);
    }
}

short int BNO055_read_gyro_x_raw() {

    short int signed_degree;
    int16_t lsb, msb, ret_val;

    lsb = BNO055_read(PAGE_0, 0x14);
    msb = BNO055_read(PAGE_0, 0x15);

    ret_val = (msb << 8) | lsb;
    signed_degree = (short int)ret_val;
    return signed_degree;
}

short int BNO055_read_gyro_y_raw() {

    short int signed_degree;
    int16_t lsb, msb, ret_val;

    lsb = BNO055_read(PAGE_0, 0x16);
    msb = BNO055_read(PAGE_0, 0x17);

    ret_val = (msb << 8) | lsb;
    signed_degree = (short int)ret_val;
    return signed_degree;
}


short int BNO055_read_gyro_z_raw() {

    short int signed_degree;
    int16_t lsb, msb, ret_val;

    lsb = BNO055_read(PAGE_0, 0x18);
    msb = BNO055_read(PAGE_0, 0x19);

    ret_val = (msb << 8) | lsb;
    signed_degree = (short int)ret_val;
    return signed_degree;
}

short int BNO055_read_gyro_temp_raw() {

    short int signed_degree;
    int16_t temp;

    temp = BNO055_read(PAGE_0, 0x34 );

    signed_degree = (short int)temp;
    return signed_degree;
}



void BNO055_init() {

    // Select BNO055 config mode
    BNO055_set_operation_mode(CONFIGMODE);
    SysCtlDelay(SysCtlClockGet()/200); //7ms
    // Configure GYR
    //230 hz and 250 dps
    uint8_t hold = 0x1B;
    UARTprintf("%d\n", hold);
//    BNO055_write(PAGE_1, GYR_CONFIG_0, 0x01 << 3 | 0x03);
    BNO055_write(PAGE_1, GYR_CONFIG_0, hold);
    BNO055_write(PAGE_1, GYR_CONFIG_1, 0x00);

    // Select BNO055 gyro temperature source
    BNO055_write(PAGE_0, TEMP_SOURCE, 0x01);

    SysCtlDelay(SysCtlClockGet()/200);
    BNO055_set_operation_mode(GYROONLY);
    SysCtlDelay(SysCtlClockGet()/200);

    BNO055_write(PAGE_0, UNIT_SEL, 0x00);
    // Select BNO055 system power mode
    BNO055_set_power_mode(NORMAL_MODE);
    SysCtlDelay(SysCtlClockGet()/500);

    // Select BNO055 system operation mode

}

void initPeripherals () {
    uint32_t ulDataRx;
    //SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    FPULazyStackingEnable();
    FPUEnable();

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
//    UARTStdioConfig(0, 115200, 16000000);
    UARTStdioConfig(0, 115200, 16000000);

    GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    GPIOPinConfigure(GPIO_PA5_SSI0TX);
    GPIOPinConfigure(GPIO_PA4_SSI0RX);
    GPIOPinTypeSSI(GPIO_PORTA_BASE,GPIO_PIN_5|GPIO_PIN_2|GPIO_PIN_4);
    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_3, SSI_MODE_MASTER, 1000000, 8);
    SSIEnable(SSI0_BASE);
    while (SSIDataGetNonBlocking(SSI0_BASE, &ulDataRx));


    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_3);
    GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_3,0xFF);


    //INIT I2C
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE,GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);
    HWREG(I2C0_BASE + I2C_O_FIFOCTL) = 80008000;
}

void Config_Timer0(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    //enable Timer to read sensor data at 2.3khz, TODO: revise the calculations.
    uint32_t ui32Period = (SysCtlClockGet()/100);
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period - 1);
    //enable timer0 int
    IntMasterEnable(); // enable processor interrupts
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntEnable(INT_TIMER0A);
}


void timerFunc() {

    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT); // clear the timer interrupt

    UARTprintf("%d\t", BNO055_read_gyro_y_raw());
    UARTprintf("%d\t", BNO055_read_gyro_y_raw());
    UARTprintf("%d\t", BNO055_read_gyro_z_raw());
    UARTprintf("%d\n", BNO055_read_gyro_temp_raw());
}

int main(void)
{

    initPeripherals ();
    BNO055_init();
    Config_Timer0();


    SysCtlDelay(SysCtlClockGet());
    TimerEnable(TIMER0_BASE, TIMER_A); //enable timer0
    while (1)
    {
        SysCtlDelay(SysCtlClockGet()/10);
    }
}
