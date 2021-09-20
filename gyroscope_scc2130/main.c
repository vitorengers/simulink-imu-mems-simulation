#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ssi.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/ssi.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

// Standard requests
#define REQ_READ_RATE  0x040000f7
#define REQ_READ_ACC_X  0x100000e9
#define REQ_READ_ACC_Y  0x140000ef
#define REQ_READ_ACC_Z  0x180000e5
#define REQ_READ_TEMP  0x1c0000e3
#define REQ_WRITE_FLT_60 0xfc200006
#define REQ_WRITE_FLT_10 0xfc1000c7
#define REQ_READ_STAT_SUM  0x7c0000b3
#define REQ_READ_RATE_STAT1 0x240000c7
#define REQ_READ_RATE_STAT2 0x280000cd
#define REQ_READ_ACC_STAT 0x3c0000d3
#define REQ_READ_COM_STAT1 0x6c0000ab
// Special requests
#define REQ_HARD_RESET 0xD8000431
// Frame field masks
#define OPCODE_FIELD_MASK 0xFC000000
#define RS_FIELD_MASK 0x03000000
#define DATA_FIELD_MASK 0x00FFFF00
#define CRC_FIELD_MASK 0x000000FF

//Function Prototypes
void init_SSI0(void);
uint32_t send_request_SCC_1(uint32_t request);
uint32_t send_request_SCC_2(uint32_t request);
void read_and_process_data(void);
void init_led(void);
void init_clock(void);
void ConfigureUART(void);
void delayMs(uint32_t ui32Ms);
void delayUs(uint32_t ui32Us);

uint32_t response_rate;
uint32_t response_acc_x;
uint32_t response_acc_y;
uint32_t response_acc_z;
uint32_t response_temp;
uint16_t rate;
uint16_t acc_x;
uint16_t acc_y;
uint16_t acc_z;
uint16_t temp;
uint8_t RSdata;
bool data_error;

void delayMs(uint32_t ui32Ms) {

    // 1 clock cycle = 1 / SysCtlClockGet() second
    // 1 SysCtlDelay = 3 clock cycle = 3 / SysCtlClockGet() second
    // 1 second = SysCtlClockGet() / 3
    // 0.001 second = 1 ms = SysCtlClockGet() / 3 / 1000

    SysCtlDelay(ui32Ms * (SysCtlClockGet() / 3 / 1000));
}

void delayUs(uint32_t ui32Us) {
    SysCtlDelay(ui32Us * (SysCtlClockGet() / 3 / 1000000));
}

void Config_UART0(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); // Habilita a PORTA.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0); // Habilita UART0
    GPIOPinConfigure(GPIO_PA0_U0RX); // Configura os pinos da GPIO para a UART0
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
     // Usa o oscilador interno preciso de 16MHz como fonte de clock da UART0
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
     // Inicializa a UART0 para 115200 bits/s com clock de 16MHz
    UARTStdioConfig(0, 115200, 16000000);
}

void init_SSI0(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    GPIOPinConfigure(GPIO_PA4_SSI0RX);
    GPIOPinConfigure(GPIO_PA5_SSI0TX);
    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_2 | GPIO_PIN_4);

    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 1000000, 8); //TODO: Check SPI bitrate again...
    SSIEnable(SSI0_BASE);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE); // Reset_scc
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_2);

    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6); // CS SCC 1
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0xff);    // Chip select high

    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_7); // CS SCC 2
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 0xff);    // Chip select high

    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_4); // CS MPU4
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 0xff);    // Chip select high
}

// Send Request to sensor_1
// Send Request to sensor_1
uint32_t send_request_SCC_1(uint32_t request)
{
    uint32_t response;
    uint32_t temp;

    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0x00); // Chip select off
    SSIDataGetNonBlocking(SSI0_BASE, &temp); // read rx buffer to clear it.
    temp = 0x00;

    SSIDataPut(SSI0_BASE, request>>16);
    while (SSIBusy(SSI0_BASE)); // wait until the busy bit is cleared

    SSIDataGet(SSI0_BASE, &temp); // Read response high word
    response = temp <<= 16;
    temp = 0x00;

    SSIDataPut(SSI0_BASE, request & 0x000FFFF); //send request low word
    while (SSIBusy(SSI0_BASE));

    SSIDataGet(SSI0_BASE, &temp); // Read response low word
    response = response | temp;

    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0xff);

    return response;
}

// Send Request to sensor_2
uint32_t send_request_SCC_2(uint32_t request)
{
    uint32_t response;

    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 0x00); // Chip select on
    SSIDataGetNonBlocking(SSI0_BASE, &response); // read rx buffer to clear it.
    response = 0x00;

    SSIDataPut(SSI0_BASE, request >> 16);
    while (SSIBusy(SSI0_BASE)); // wait until the busy bit is cleared
    SSIDataGet(SSI0_BASE, &response); // Read response high word
    response <<= 16;

    SSIDataPut(SSI0_BASE, request & 0x000FFFF); //send request low word
    while (SSIBusy(SSI0_BASE));
    SSIDataGet(SSI0_BASE, &response); // Read response low word

    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 0xff);
    return response;
}

void read_and_process_data_SCC_1(void)
{
    response_temp = send_request_SCC_1(REQ_READ_RATE);
    response_rate = send_request_SCC_1(REQ_READ_ACC_X);
    response_acc_x = send_request_SCC_1(REQ_READ_ACC_Y);
    response_acc_y = send_request_SCC_1(REQ_READ_ACC_Z);
    response_acc_z = send_request_SCC_1(REQ_READ_TEMP);

    //Handle rate data
    rate = (response_rate & DATA_FIELD_MASK) >> 8;
    RSdata = (response_rate & RS_FIELD_MASK) >> 24;
    if (RSdata != 1) data_error = true;

    //Handle accelerometer data
    acc_x = (response_acc_x & DATA_FIELD_MASK) >> 8;
    RSdata = (response_acc_x & DATA_FIELD_MASK) >> 24;
    if(RSdata != 1) data_error = true;

    //Handle accelerometer data
    acc_y = (response_acc_y & DATA_FIELD_MASK) >> 8;
    RSdata = (response_acc_y & DATA_FIELD_MASK) >> 24;
    if(RSdata != 1) data_error = true;

    //Handle accelerometer data
    acc_z = (response_acc_z & DATA_FIELD_MASK) >> 8;
    RSdata = (response_acc_z & DATA_FIELD_MASK) >> 24;
    if(RSdata != 1) data_error = true;

    //Handle temperature data
    temp = (response_temp & DATA_FIELD_MASK) >> 8;
    RSdata = (response_temp & RS_FIELD_MASK) >> 24;
    if(RSdata != 1) data_error = true;

}

void read_and_process_data_SCC_2(void)
{
    response_temp = send_request_SCC_2(REQ_READ_RATE);
    response_rate = send_request_SCC_2(REQ_READ_ACC_X);
    response_acc_x = send_request_SCC_2(REQ_READ_ACC_Y);
    response_acc_y = send_request_SCC_2(REQ_READ_ACC_Z);
    response_acc_z = send_request_SCC_2(REQ_READ_TEMP);

    //Handle rate data
    rate = (response_rate & DATA_FIELD_MASK) >> 8;
    RSdata = (response_rate & RS_FIELD_MASK) >> 24;
    if (RSdata != 1) data_error = true;

    //Handle accelerometer data
    acc_x = (response_acc_x & DATA_FIELD_MASK) >> 8;
    RSdata = (response_acc_x & DATA_FIELD_MASK) >> 24;
    if(RSdata != 1) data_error = true; 

    //Handle accelerometer data
    acc_y = (response_acc_y & DATA_FIELD_MASK) >> 8;
    RSdata = (response_acc_y & DATA_FIELD_MASK) >> 24;
    if(RSdata != 1) data_error = true; 

    //Handle accelerometer data
    acc_z = (response_acc_z & DATA_FIELD_MASK) >> 8;
    RSdata = (response_acc_z & DATA_FIELD_MASK) >> 24;
    if(RSdata != 1) data_error = true; 

    //Handle temperature data
    temp = (response_temp & DATA_FIELD_MASK) >> 8;
    RSdata = (response_temp & RS_FIELD_MASK) >> 24;
    if(RSdata != 1) data_error = true;

}

void init_led(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);
}

void init_clock(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
}

int main(void)
{
    uint32_t response_StatSum;
    uint32_t response_RateStat1;
    uint32_t response_RateStat2;
    uint32_t response_AccStat;
    uint32_t response_ComStat1;
    bool StartupOK;

    delayMs(125); // +- 125ms para 16mhz t=(num_clock/16mhz)

    init_clock(); 
    init_SSI0();
    init_led();
    Config_UART0();

    //led on
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x08); //led on

    //Reset Sensor
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0x00);
    delayMs(10);
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0xff);

    //sensor power up...
    delayMs(25); // 25ms
    send_request_SCC_2(REQ_WRITE_FLT_60); // set output filter to 60 hz
    delayMs(595); // 595 ms;

    //Clear status registers...
    send_request_SCC_2(REQ_READ_RATE_STAT1);
    send_request_SCC_2(REQ_READ_RATE_STAT2);
    send_request_SCC_2(REQ_READ_ACC_STAT);
    send_request_SCC_2(REQ_READ_COM_STAT1);
    send_request_SCC_2(REQ_READ_STAT_SUM);

    while(1)
    {
        //Check functionality...
        //StartupOK = true;
        //response_StatSum = send_request_SCC_2(REQ_READ_RATE); // Read Status Summary register again to get
        //response_ComStat1 = send_request_SCC_2(REQ_READ_STAT_SUM);
        //RSdata = 1;
        //RSdata = (response_StatSum & RS_FIELD_MASK) >> 24; // the correct status data
        //UARTprintf("%5x \r\n", response_ComStat1);
        read_and_process_data_SCC_1();
        UARTprintf("%5d %5d %5d %5d %5d %2d \r\n", rate,acc_x,acc_y,acc_z,temp, data_error);
    }
    return 0;
}
