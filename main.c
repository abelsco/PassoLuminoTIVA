#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "driverlib/pwm.h"
#include "driverlib/adc.h"

//*****************************************************************************

#define USER_LEDA  GPIO_PIN_0 //N�mero do pino, nao especifica a porta
#define USER_LEDB  GPIO_PIN_1
#define USER_LEDC  GPIO_PIN_2
#define USER_LEDD  GPIO_PIN_3

uint32_t g_ui32SysClock;

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

//*****************************************************************************
//
// Configure the UART and its pins.  This must be called before UARTprintf().
//
//*****************************************************************************
void InitConsole(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    //
    // Abrir o terminal (view > terminal) e configurar para serial, 115,200 8-N-1.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    //
    // Enable UART0 so that we can configure the clock.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    //
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}

void pwm_mul(uint32_t valorpwm_init, uint32_t ui32SysClock)
{
    unsigned long incremento = valorpwm_init - 2095;
    unsigned long valorpwm = incremento;
    //if((GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_1))){
    while ((valorpwm) < 2000)
    {
        valorpwm += incremento;
        SysCtlDelay(ui32SysClock / 20);
        UARTprintf("duty: %i\n", valorpwm);
        PWMPulseWidthSet(0, 2000, valorpwm);
    }

    while ((valorpwm) > 1)
    {
        valorpwm -= incremento;
        SysCtlDelay(ui32SysClock / 20);
        UARTprintf("duty: %i\n", valorpwm);
        PWMPulseWidthSet(0, 2000, valorpwm);
    }
}

//*****************************************************************************
//
// Print "Hello World!" to the UART on the Intelligent UART Module.
//
//*****************************************************************************
int angulo;
int angulo1;
char a;
int main(void)
{
    uint32_t ui32SysClock;
    uint32_t pui32ADC0Value[1];
    // Habilitando porta e pinos
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE))
    {
    }

    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_2);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_3);

    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);

    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

    ADCSequenceStepConfigure(ADC0_BASE, 3, 0,
                             ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);

    ADCSequenceEnable(ADC0_BASE, 3);

    ADCIntClear(ADC0_BASE, 3);

    //
    // Run from the PLL at 120 MHz.
    //
    ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
    SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
    SYSCTL_CFG_VCO_480),
                                          120000000);
    //
    // Initialize the UART.
    //
    InitConsole();
    //
    UARTprintf("\nIniciando...\n");
    int x[1], i;
    angulo = 45;
    angulo1 = ((angulo + 1) / (0.088 * 8));

    for (i = 0; i < angulo1; i++)
    {
        GPIOPinWrite(GPIO_PORTE_BASE, (USER_LEDA | USER_LEDB),
        USER_LEDA);
        SysCtlDelay(ui32SysClock / 1000);

        GPIOPinWrite(GPIO_PORTE_BASE, (USER_LEDA | USER_LEDB),
        USER_LEDB);
        SysCtlDelay(ui32SysClock / 1000);

        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0);
        GPIOPinWrite(GPIO_PORTE_BASE, (USER_LEDC | USER_LEDD),
        USER_LEDC);
        SysCtlDelay(ui32SysClock / 1000);

        GPIOPinWrite(GPIO_PORTE_BASE, (USER_LEDC | USER_LEDD),
        USER_LEDD);
        SysCtlDelay(ui32SysClock / 1000);
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0);
    }
    x[0] = 0;
    x[1] = 0;
    while (1)
    {
        // Rotina para ler a entrada serial e calcular a quantidade de passos

        ADCProcessorTrigger(ADC0_BASE, 3);
        while (!ADCIntStatus(ADC0_BASE, 3, false))
        {
        }
        ADCIntClear(ADC0_BASE, 3);
        ADCSequenceDataGet(ADC0_BASE, 3, pui32ADC0Value);
        SysCtlDelay(ui32SysClock / 20);
        pwm_mul(pui32ADC0Value[0], ui32SysClock);




        if(pui32ADC0Value[0] < 100){
        x[0]=1;
      }else if(pui32ADC0Value[0] > 3000){
        x[0]=2;
      }else{
        x[1]=3;
      }

        if (x[0] == 1)
        {
            angulo = 90;
            angulo1 = ((angulo + 1) / (0.088 * 8));
            UARTprintf("\n");
            UARTprintf("Valor angulo = %4d\r\n", angulo);
            UARTprintf("Valor passos = %4d\r\n", angulo1 * 4);
            UARTprintf("\n");


            // Rotina de execu��o dos passos

            for (i = 0; i < angulo1; i++)
            {
                GPIOPinWrite(GPIO_PORTE_BASE, (USER_LEDA | USER_LEDB),
                USER_LEDA);
                SysCtlDelay(ui32SysClock / 1000);

                GPIOPinWrite(GPIO_PORTE_BASE, (USER_LEDA | USER_LEDB),
                USER_LEDB);
                SysCtlDelay(ui32SysClock / 1000);

                GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0);
                GPIOPinWrite(GPIO_PORTE_BASE, (USER_LEDC | USER_LEDD),
                USER_LEDC);
                SysCtlDelay(ui32SysClock / 1000);

                GPIOPinWrite(GPIO_PORTE_BASE, (USER_LEDC | USER_LEDD),
                USER_LEDD);
                SysCtlDelay(ui32SysClock / 1000);
                GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0);
            }
        }
        if (x[0] == 2)
        {
            angulo = 90;
            angulo1 = ((angulo + 1) / (0.088 * 8));
            UARTprintf("\n");
            UARTprintf("Valor angulo = %4d\r\n", angulo);
            UARTprintf("Valor passos = %4d\r\n", angulo1 * 4);
            angulo = 0;

            // Rotina de execu��o dos passos

            for (i = 0; i < angulo1; i++)
            {
                GPIOPinWrite(GPIO_PORTE_BASE, (USER_LEDC | USER_LEDD),
                8);
                SysCtlDelay(ui32SysClock / 1000);

                GPIOPinWrite(GPIO_PORTE_BASE, (USER_LEDC | USER_LEDD),
                4);
                SysCtlDelay(ui32SysClock / 1000);
                GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0);

                GPIOPinWrite(GPIO_PORTE_BASE, (USER_LEDA | USER_LEDB),
                2);
                SysCtlDelay(ui32SysClock / 1000);

                GPIOPinWrite(GPIO_PORTE_BASE, (USER_LEDA | USER_LEDB),
                1);
                SysCtlDelay(ui32SysClock / 1000);
                GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0);

            }
        }
        if(x[0] == 3){
          if(x[0] == 1)
          {
            angulo = 90;
            angulo1 = ((angulo + 1) / (0.088 * 8));
            UARTprintf("\n");
            UARTprintf("Valor angulo = %4d\r\n", angulo);
            UARTprintf("Valor passos = %4d\r\n", angulo1 * 4);
            angulo = 0;

            // Rotina de execu��o dos passos

            for (i = 0; i < angulo1; i++)
            {
                GPIOPinWrite(GPIO_PORTE_BASE, (USER_LEDC | USER_LEDD),
                8);
                SysCtlDelay(ui32SysClock / 1000);

                GPIOPinWrite(GPIO_PORTE_BASE, (USER_LEDC | USER_LEDD),
                4);
                SysCtlDelay(ui32SysClock / 1000);
                GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0);

                GPIOPinWrite(GPIO_PORTE_BASE, (USER_LEDA | USER_LEDB),
                2);
                SysCtlDelay(ui32SysClock / 1000);

                GPIOPinWrite(GPIO_PORTE_BASE, (USER_LEDA | USER_LEDB),
                1);
                SysCtlDelay(ui32SysClock / 1000);
                GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0);

            }
          }
          else if(x[0] = 2){
          angulo = 90;
          angulo1 = ((angulo + 1) / (0.088 * 8));

          for (i = 0; i < angulo1; i++)
          {
              GPIOPinWrite(GPIO_PORTE_BASE, (USER_LEDA | USER_LEDB),
              USER_LEDA);
              SysCtlDelay(ui32SysClock / 1000);

              GPIOPinWrite(GPIO_PORTE_BASE, (USER_LEDA | USER_LEDB),
              USER_LEDB);
              SysCtlDelay(ui32SysClock / 1000);

              GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0);
              GPIOPinWrite(GPIO_PORTE_BASE, (USER_LEDC | USER_LEDD),
              USER_LEDC);
              SysCtlDelay(ui32SysClock / 1000);

              GPIOPinWrite(GPIO_PORTE_BASE, (USER_LEDC | USER_LEDD),
              USER_LEDD);
              SysCtlDelay(ui32SysClock / 1000);
              GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0);
          }
        }
      }
        angulo1 = 0;
    }
}
