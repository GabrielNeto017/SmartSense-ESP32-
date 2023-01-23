
#include <Setup.h>


//MCP3909 - GAIN
float G1, G0 = 1;


void Setup_Init()
{
    //Declaração de variáveis
    pinMode (DI0, INPUT);
    pinMode (DI1, INPUT);
    pinMode (DI2, INPUT);
    pinMode (DI3, INPUT);
    pinMode (SW1, INPUT);
    
    pinMode (DO0, OUTPUT);
    pinMode (DO1, OUTPUT);
    pinMode (DO2, OUTPUT);
}


void Setup_Init2()
{
    //ENTRADAS ANALÓGICAS
    pinMode (AI0, INPUT_PULLUP);
    pinMode (AI1, INPUT_PULLUP);
    analogReadResolution(12);
    //analogSetWidth(12);

    //LEDS
    pinMode (LED1, OUTPUT);
    pinMode (LED2, OUTPUT);
}


 void Analisador_Init()
{
     //ANALISADOR - MCP 3909
    pinMode (MCP_MISO, INPUT);
    pinMode (MCP_MOSI, OUTPUT);
    pinMode (MCP_SCK, OUTPUT);
    pinMode (MCP_CS, OUTPUT);
    pinMode (MCLR, OUTPUT);
    pinMode (G1_gain, OUTPUT);
    pinMode (G0_gain, OUTPUT);


    digitalWrite (MCLR, HIGH);
    digitalWrite (MCP_CS, HIGH);
    digitalWrite (G0, HIGH);
    digitalWrite (G1, HIGH);

    digitalWrite (MCLR, 1);
    digitalWrite (MCP_CS, 1);

    //ENTRY CODE
    digitalWrite (MCLR, LOW);
    digitalWrite (MCP_CS, HIGH);
    digitalWrite (MCP_SCK, LOW);

    digitalWrite (MCP_SCK, LOW);
    digitalWrite (MCLR, HIGH);
    digitalWrite (MCP_CS, LOW);

    //POST HP1 DUAL CHANNEL OUTPUT (0xAC) - check manual MCP3909

    //1
    digitalWrite (MCP_SCK, 0);
    digitalWrite (MCP_MOSI, 1);
    digitalWrite (MCP_SCK, 1);

    //2
    digitalWrite (MCP_SCK, 0);
    digitalWrite (MCP_MOSI, 0);
    digitalWrite (MCP_SCK, 1);

    //3
    digitalWrite (MCP_SCK, 0);
    digitalWrite (MCP_MOSI, 1);
    digitalWrite (MCP_SCK, 1);

    //4
    digitalWrite (MCP_SCK, 0);
    digitalWrite (MCP_MOSI, 0);
    digitalWrite (MCP_SCK, 1);

    //5
    digitalWrite (MCP_SCK, 0);
    digitalWrite (MCP_MOSI, 1);
    digitalWrite (MCP_SCK, 1);

    //6
    digitalWrite (MCP_SCK, 0);
    digitalWrite (MCP_MOSI, 1);
    digitalWrite (MCP_SCK, 1);

    //7
    digitalWrite (MCP_SCK, 0);
    digitalWrite (MCP_MOSI, 0);
    digitalWrite (MCP_SCK, 1);

    //8
    digitalWrite (MCP_SCK, 0);
    digitalWrite (MCP_MOSI, 0);
    digitalWrite (MCP_SCK, 1);

    digitalWrite (MCP_CS, 1);

    digitalWrite (MCP_SCK, 0);

}



