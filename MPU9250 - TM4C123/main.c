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
#define SELF_TEST_X_GYRO     0x00
#define SMPLRT_DIV           0x19
#define CONFIG               0x1A
#define  PWR_MGMT_1           0x6B
#define  PWR_MGMT_2           0x6C
#define GYRO_CONFIG          0x1B
#define INT_PIN_CFG          0x37
#define  INT_ENABLE           0x38
#define BITS_DLPF_CFG_188HZ         0x01
#define MPUREG_CONFIG 0x1A
#define MPUREG_GYRO_CONFIG 0x1B
#define BITS_FS_250DPS              0x00
#define MPUREG_TEMP_OUT_H 0x41
#define MPUREG_TEMP_OUT_L 0x42


void initPeripherals () {
    uint32_t ulDataRx, ulDataRxEx, ulDataTx;
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
    UARTStdioConfig(0, 500000, 16000000);

    GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    GPIOPinConfigure(GPIO_PA5_SSI0TX);
    GPIOPinConfigure(GPIO_PA4_SSI0RX);
    GPIOPinTypeSSI(GPIO_PORTA_BASE,GPIO_PIN_5|GPIO_PIN_2|GPIO_PIN_4);
    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_3, SSI_MODE_MASTER, 1000000, 8);
    SSIEnable(SSI0_BASE);
    while (SSIDataGetNonBlocking(SSI0_BASE, &ulDataRx));


    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_3);
    GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_3,0xFF);

}

short int getTemp();
short int getXAxis();


void Config_Timer0(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    //enable Timer to read sensor data at 2.3khz, TODO: revise the calculations.
    uint32_t ui32Period = (SysCtlClockGet()/500);
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period - 1);
    //enable timer0 int
    IntMasterEnable(); // enable processor interrupts
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntEnable(INT_TIMER0A);
}


void csLow() {
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0x00);
}

void csHigh() {
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0xFF);
}

void writeReg(uint32_t addr, uint32_t value) {
    SSIDataPut(SSI0_BASE, addr);
    SSIDataPut(SSI0_BASE, value);
    while (SSIBusy(SSI0_BASE)) continue;
}

void readReg(uint32_t addr, uint32_t *value) {

    SSIDataPut(SSI0_BASE, addr | 0x80);
    SSIDataPut(SSI0_BASE, 0x00);
    while (SSIBusy(SSI0_BASE)) continue;
    SSIDataGet(SSI0_BASE, value);
    SSIDataGet(SSI0_BASE, value);
    while (SSIBusy(SSI0_BASE)) continue;

}

short int getXAxis() {
    uint32_t High = 0x00, Low = 0x00, degress = 0x00;
    uint32_t ulDataRx;
    short int signed_degree;

    csLow();
    readReg(0x43, &High);
    csHigh();

//    UARTprintf("%d.", High);

    degress = (High << 8);

    SysCtlDelay(10);


//    WHO AM I BEGIN
    csLow();
    readReg(0x75, &ulDataRx);
    csHigh();

    //UARTprintf("WHO AM I: %d\n", ulDataRx);
    //WHO AM I END

    SysCtlDelay(10);

    csLow();
    readReg(0x44, &Low);
    csHigh();

//    UARTprintf("%d.", Low);

    degress = degress | Low;

    signed_degree = (short int)degress;
    return signed_degree;
}

short int getYAxis() {

    uint32_t High = 0x00, Low = 0x00, degress = 0x00;
    uint32_t ulDataRx;
    short int signed_degree;
    csLow();
    readReg(0x45, &High);
    csHigh();

    //UARTprintf("High1: %d\n", High);

    degress = (High << 8);

    SysCtlDelay(10);


    //WHO AM I BEGIN
    csLow();
    readReg(0x75, &ulDataRx);
    csHigh();

    //UARTprintf("WHO AM I: %d\n", ulDataRx);
    //WHO AM I END

    SysCtlDelay(10);

    csLow();
    readReg(0x46, &Low);
    csHigh();

    //UARTprintf("Low: %d\n", Low);

    degress = degress | Low;

    signed_degree = (short int)degress;
    return signed_degree;

}

short int getZAxis() {

    uint32_t High = 0x00, Low = 0x00, degress = 0x00;
    uint32_t ulDataRx;
    short int signed_degree;

    csLow();
    readReg(0x47, &High);
    csHigh();

    //UARTprintf("High1: %d\n", High);

    degress = (High << 8);

    SysCtlDelay(10);


    //WHO AM I BEGIN
    csLow();
    readReg(0x75, &ulDataRx);
    csHigh();

    //UARTprintf("WHO AM I: %d\n", ulDataRx);
    //WHO AM I END

    SysCtlDelay(10);

    csLow();
    readReg(0x48, &Low);
    csHigh();

    //UARTprintf("Low: %d\n", Low);

    degress = degress | Low;

    signed_degree = (short int)degress;
    return signed_degree;

}

short int getTemp() {

    uint32_t High = 0x00, Low = 0x00, degress = 0x00;
    uint32_t ulDataRx;
    short int signed_degree;

    csLow();
    readReg(0x41, &High);
    csHigh();

    //UARTprintf("High1: %d\n", High);

    degress = (High << 8);

    SysCtlDelay(10);


    //WHO AM I BEGIN
    csLow();
    readReg(0x75, &ulDataRx);
    csHigh();

    //UARTprintf("WHO AM I: %d\n", ulDataRx);
    //WHO AM I END

    SysCtlDelay(10);

    csLow();
    readReg(0x42, &Low);
    csHigh();

    //UARTprintf("Low: %d\n", Low);

    degress = degress | Low;

    signed_degree = (short int)degress;
    return signed_degree;


}

int counter = 0;

void timerFunc() {

    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT); // clear the timer interrupt
//    if (counter == 0) {
      //getTemp();


    UARTprintf("%d\t",  getYAxis());
    UARTprintf("%d\t",  getZAxis());
    UARTprintf("%d\t", getTemp());
    UARTprintf("%d\n",  getXAxis());

//      getYAxis();
//      UARTprintf("X: %d\n", getYAxis());
//
//      getZAxis();
//      UARTprintf("X: %d\n\n", getZAxis());




}

int main(void)
{
    uint32_t ulDataRx, ulDataRxEx, ulDataTx;
    bool bTest[14];


    initPeripherals ();
    Config_Timer0();

    uint32_t u32_data;

    csLow();
    writeReg(PWR_MGMT_1, 0x80);
    csHigh();


    SysCtlDelay(SysCtlClockGet()/100);

    csLow();
    writeReg(PWR_MGMT_1, 0x00);
    csHigh();

    SysCtlDelay(SysCtlClockGet()/100);

    csLow();
    writeReg(PWR_MGMT_1, 0x01);
    csHigh();


    SysCtlDelay(SysCtlClockGet()/200);


    csLow();
    writeReg(PWR_MGMT_2, 0x00);
    csHigh();


    SysCtlDelay(10);

    csLow();
    writeReg(0x6A, 0x50);
    csHigh();


    SysCtlDelay(10);


    csLow();
    writeReg(MPUREG_CONFIG, BITS_DLPF_CFG_188HZ);
    csHigh();


    SysCtlDelay(10);


    csLow();
    //MPUREG_SMPLRT_DIV
    writeReg(0x19, 0x00);
    csHigh();


    SysCtlDelay(10);


    csLow();
    writeReg(MPUREG_GYRO_CONFIG, BITS_FS_250DPS);
    csHigh();
    SysCtlDelay(10);
    csLow();
    writeReg(0x19, 0x00);
    csHigh();
    SysCtlDelay(10);

    csLow();
    writeReg(INT_PIN_CFG, 0x12);
    csHigh();


    SysCtlDelay(10);


    csLow();
    writeReg(INT_ENABLE, 0x01);
    csHigh();


    SysCtlDelay(SysCtlClockGet());
    TimerEnable(TIMER0_BASE, TIMER_A); //enable timer0
    while (1)
    {
////            timerFunc();
//        if (count == 0) {
//            UARTprintf("TEMPERATURAAAA: %d\n\n", getTemp());
//        }
//        else {
//            UARTprintf("X: %d\n\n", getXAxis());
//        }
//        count++;
//        if (count == 10) count = 0;
//        SysCtlDelay(10);
////        UARTprintf("Z: %d\n\n", getZAxis());
////        SysCtlDelay(10);
//

        SysCtlDelay(SysCtlClockGet()/10);

    }
}
