#include <SO.h>

WiFiMQTTManager wmm2(SW2, AP_PASSWORD); 

//-------Sensor de °C e Umidade-----------
#define DHTPIN 25
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

//------Definições Tasks----------------
static uint8_t TaskCoreZero = 0;
static uint8_t TaskCoreOne = 1;


//------Variáveis de Processo----------
int statusled;
String ESTADO, csts = "Hello";


int flag = 0;
int flag2 = 1;
int count1 = 0;

int periodico = 0;

float t, h, hic = 0;

int leituraAI0 = 0;
int leituraAI1 = 0;

int CONT_AMOSTRAS = 0;
float acc_v, acc_i, acc_p = 0;
float valorcorrenteanterior = 0;
float GANHO=1;
float VRMS, IRMS, P_ativa, P_aparente, P_reativa, kWh, avol = 0;
int f_sample = 0;
int contAmostra = 0;
int mcp_ch0 = 0;
int mcp_ch1 = 0;
int tensaoCH0 = 0;
int tensaoCH1 = 0;
static int habilitaPIN = 0;
int ch1[15];
int ch0[15];


void FitaLed_Init()
{
    xTaskCreatePinnedToCore(
            FitaLed,
            "FitaLed",
            1500,
            NULL,
            6,
            NULL,
            TaskCoreZero);    //ZERO
    delay(500);
}

void SO_Init()
{

    dht.begin();
    //INICIALIZANDO TASKS CORE 0
    xTaskCreatePinnedToCore(
                FeedWatchdog,
                "FeedWatchdog",
                1000,
                NULL,
                0,
                NULL,
                TaskCoreZero);
    delay(300);

    xTaskCreatePinnedToCore(
                DHT11e,
                "DHT11e",
                1500,
                NULL,
                1,
                NULL,
                TaskCoreZero);
    delay(300);

    xTaskCreatePinnedToCore(
                EntradasAnalogicas,
                "EntradasAnalogicas",
                2000,
                NULL,
                8,
                NULL,
                TaskCoreZero);
    delay(300);




    xTaskCreatePinnedToCore(
                subs,
                "subs",
                5000,
                NULL,
                10,
                NULL,
                TaskCoreZero);
    delay(300);


    //INICIALIZANDO TASKS CORE 1
    xTaskCreatePinnedToCore(
                Analisador,
                "Analisador",
                
                10000,
                NULL,
                9,
                NULL,
                TaskCoreOne);
    delay(300);
}



void FeedWatchdog(void * pvParameters)
{
    while(true) 
    {
      rtc_wdt_feed();
      vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void DHT11e(void * pvParameters)
{
  while(true){
      // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    t = dht.readTemperature();

    // Compute heat index in Celsius (isFahreheit = false)
    hic = dht.computeHeatIndex(t, h, false);
    

    /*Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("°C \n"));*/
    
     
    
    vTaskDelay(700 / portTICK_PERIOD_MS);
  }
}

void FitaLed(void * pvParameters){
  while(true){
     
    if(digitalRead(SW1)==0 && statusled!=6 && flag2==1)
    {
      for(int i = 0; i<20; i++)
        {
            delay(100);
            count1++;
            if(count1>=20)
            {
            statusled = 6; 

            flag = 1;
            flag2 = 0;
            }
        }
    }

    else if(digitalRead(SW1)==1)
    {
      count1 = 0;
      flag2 = 1;
    }

    else if(statusled==6 && digitalRead(SW1)==0 && count1 == 0 && flag2 == 1)
    {
      flag=0;
      flag2 = 0;
    }


    if(flag==0)
    {
    //0 = acionado c/ nível lógico alto   1 = desacionado
    if(digitalRead(DI0) == 0) statusled = 1;
    else if((digitalRead(DI0) == 1) && (digitalRead(DI1) == 1) && (digitalRead(DI2) == 1) && (digitalRead(DI3) == 1)) statusled = 2;
    else if((digitalRead(DI0) == 1) && (digitalRead(DI1) == 1) && (digitalRead(DI2) == 1) && (digitalRead(DI3) == 0)) statusled = 2;
    else if((digitalRead(DI0) == 1) && (digitalRead(DI1) == 0) && (digitalRead(DI2) == 1) && (digitalRead(DI3) == 1)) statusled = 3;
    else if((digitalRead(DI0) == 1) && (digitalRead(DI1) == 0) && (digitalRead(DI2) == 1) && (digitalRead(DI3) == 0)) statusled = 4;
    else if((digitalRead(DI0) == 1) && (digitalRead(DI1) == 1) && (digitalRead(DI2) == 0) && (digitalRead(DI3) == 0)) statusled = 4;
    else if((digitalRead(DI0) == 1) && (digitalRead(DI1) == 1) && (digitalRead(DI2) == 0) && (digitalRead(DI3) == 1)) statusled = 4;
    else if((digitalRead(DI0) == 1) && (digitalRead(DI1) == 0) && (digitalRead(DI2) == 0) && (digitalRead(DI3) == 1)) statusled = 4;
    else if((digitalRead(DI0) == 1) && (digitalRead(DI1) == 0) && (digitalRead(DI2) == 0) && (digitalRead(DI3) == 0)) statusled = 5;
    
    else
    {
      statusled = statusled;
    }
     }
    

    if(statusled==1){
      ESTADO = "EMERG";
      analogWrite(DO0, 255);
      analogWrite(DO1, 0);
      analogWrite(DO2, 0);
      
    }
    else if(statusled==2){
      ESTADO = "RESET";
      analogWrite(DO0, 0);
      analogWrite(DO1, 0);
      analogWrite(DO2, 255);
    }
    else if(statusled==3){
      ESTADO = "DESLI";
      analogWrite(DO0, 255);
      analogWrite(DO1, 120);
      analogWrite(DO2, 0);
    }
    else if(statusled==4){
      ESTADO = "AOPER";
      analogWrite(DO0, 0);
      analogWrite(DO1, 255);
      analogWrite(DO2, 0);
    }
    else if(statusled==5){
      ESTADO = "PRODU";
      analogWrite(DO0, 0);
      analogWrite(DO1, 255);
      analogWrite(DO2, 0);
      delay(700);
      analogWrite(DO1, 0);
     // digitalWrite(DO1, LOW);
    }
    
    else if(statusled==6){
      ESTADO = "CARNA";
      long randNumber, randNumber2, randNumber3;
      randNumber = random(255);
      randNumber2 = random(255);
      randNumber3 = random(255);
      analogWrite(DO0, randNumber);
      analogWrite(DO1, randNumber2);
      analogWrite(DO2, randNumber3);
      delay(1500);
      //analogWrite(DO0, 0);
      //analogWrite(DO1, 0);
      //analogWrite(DO2, 0);
    }
    else{
      //error
    }
    
  
    
 
    vTaskDelay(900 / portTICK_PERIOD_MS);
    
  } //FIM DO FOR
} //FIM DA FUNÇÃO

void EntradasAnalogicas(void * pvParameters){
  while(true){
      leituraAI0 = analogRead(AI0);
      leituraAI1 = analogRead(AI1);
      
      digitalWrite(LED2, 1);
      delay(300);
      digitalWrite(LED2, 0);
      
    vTaskDelay(1200 / portTICK_PERIOD_MS);
  }
}

void Analisador(void * pvParameters){
  while(true){
  wmm2.loop();
  if(habilitaPIN<=2){
      habilitaPIN++;
     
      //ENTRY CODE  0xAC
      digitalWrite(MCLR, 0);
      digitalWrite(MCP_CS, 1);
      digitalWrite(MCP_SCK, 0);

      GPIO.out_w1tc = (1 << MCP_SCK); 
      GPIO.out_w1tc = (1 << MCP_SCK); 
      GPIO.out_w1ts = (1 << MCLR);
      GPIO.out_w1ts = (1 << MCLR);
      GPIO.out_w1ts = (1 << MCLR);
      GPIO.out_w1ts = (1 << MCLR);
      GPIO.out_w1ts = (1 << MCLR);
      GPIO.out_w1ts = (1 << MCLR);

      GPIO.out_w1tc = (1 << MCP_CS);
      GPIO.out_w1tc = (1 << MCP_CS);
      GPIO.out_w1tc = (1 << MCP_CS);
      GPIO.out_w1tc = (1 << MCP_CS);
      GPIO.out_w1tc = (1 << MCP_CS);
      GPIO.out_w1tc = (1 << MCP_CS);
      GPIO.out_w1tc = (1 << MCP_CS);
      GPIO.out_w1tc = (1 << MCP_CS);


      //POST HPF1 DUAL CHANNEL OUTPUT  (0xAC)
      //1

      GPIO.out_w1tc = (1 << MCP_SCK); 
      GPIO.out_w1ts = (1 << MCP_MOSI); 
      GPIO.out_w1ts = (1 << MCP_SCK); 

              
      //2
      GPIO.out_w1tc = (1 << MCP_SCK);
      GPIO.out_w1tc = (1 << MCP_MOSI);  
      GPIO.out_w1ts = (1 << MCP_SCK); 


      //3
      GPIO.out_w1tc = (1 << MCP_SCK); 
      GPIO.out_w1ts = (1 << MCP_MOSI);
      GPIO.out_w1ts = (1 << MCP_SCK);


      //4
      GPIO.out_w1tc = (1 << MCP_SCK); 
      GPIO.out_w1tc = (1 << MCP_MOSI); 
      GPIO.out_w1ts = (1 << MCP_SCK); 

      //5
      GPIO.out_w1tc = (1 << MCP_SCK); 
      GPIO.out_w1ts = (1 << MCP_MOSI);  
      GPIO.out_w1ts = (1 << MCP_SCK); 


      //6
      GPIO.out_w1tc = (1 << MCP_SCK); 
      GPIO.out_w1ts = (1 << MCP_MOSI); 
      GPIO.out_w1ts = (1 << MCP_SCK); 


      //7
      GPIO.out_w1tc = (1 << MCP_SCK); 
      GPIO.out_w1tc = (1 << MCP_MOSI); 
      GPIO.out_w1ts = (1 << MCP_SCK); 


      //8
      GPIO.out_w1tc = (1 << MCP_SCK);  
      GPIO.out_w1tc = (1 << MCP_MOSI); 
      GPIO.out_w1ts = (1 << MCP_SCK);  

      GPIO.out_w1ts = (1 << MCP_CS); 
      GPIO.out_w1ts = (1 << MCP_CS); 

      digitalWrite(MCP_CS,0);

      digitalWrite(MCP_SCK,0);
      digitalWrite(MCP_SCK,0);

      
      vTaskDelay(5/portTICK_RATE_MS);
  }
      if(habilitaPIN>2){
        
        f_sample = 0;
        contAmostra=1;

        mcp_ch1 = 0x00;
        mcp_ch0 = 0x00;

        tensaoCH0 = 0;
        tensaoCH1 = 0;

        while(contAmostra<=3)
        {       
            GPIO.out_w1tc = (1 << MCP_CS); 

            if(digitalRead(MCP_MISO)==1)
            {
                    f_sample = 1;
                    mcp_ch0=0;
                    mcp_ch1=0;
            }

            if((digitalRead(MCP_MISO)==0) && (f_sample==1))
            {
                    for(int j=0; j<32; j++)
                    {
                        
                        GPIO.out_w1ts = (1 << MCP_SCK); 
                        GPIO.out_w1tc = (1 << MCP_SCK); 

                        if(j<15)
                        {
                                mcp_ch1 = mcp_ch1 | digitalRead(MCP_MISO); // talvez seja interessante | 0
                                mcp_ch1 = mcp_ch1 << 1;
                        }
                        else if(j==15)
                        {
                                mcp_ch1 = mcp_ch1 | digitalRead(MCP_MISO);
                        }
                        else if(j>=16 && j<31)
                        {

                                mcp_ch0 = mcp_ch0 | digitalRead(MCP_MISO); // talvez seja interessante | 0
                                mcp_ch0 = mcp_ch0 << 1;
                        }
                        else if(j==31)
                        {
                                mcp_ch0 = mcp_ch0 | digitalRead(MCP_MISO);
                        }
                            
                    }  
            contAmostra++;
            f_sample=0;
            }
        }

        if(mcp_ch1>=32768)
        {
            mcp_ch1 = - 65535 + mcp_ch1;
        }
        if(mcp_ch0>=32768){
            mcp_ch0 =  - 65535 + mcp_ch0;
        }
        
        
        tensaoCH1=0;
        tensaoCH0=0;
        tensaoCH1=mcp_ch1;
        tensaoCH0=mcp_ch0;

       


        if(CONT_AMOSTRAS < QNT_AMOSTRAS)
        {
            acc_v = acc_v + powf(tensaoCH1, 2);
            acc_i = acc_i + powf(tensaoCH0, 2);
            acc_p = acc_p + abs(tensaoCH1*tensaoCH0);
            CONT_AMOSTRAS++;
        }

        if(CONT_AMOSTRAS >= QNT_AMOSTRAS)
        {
          
            VRMS = acc_v/QNT_AMOSTRAS;
            VRMS = sqrtf(VRMS)/75.4;
            
            IRMS = acc_i/QNT_AMOSTRAS;
            IRMS = sqrtf(IRMS)/1.32;   //m[A]

            P_aparente = (VRMS * IRMS)/1000;
            
            P_ativa = acc_p /QNT_AMOSTRAS;
            P_ativa = P_ativa/95000; // /0,93939  ou divide tudo por 

             if(P_ativa > P_aparente){
              P_ativa = P_ativa * 0.98;
              if(P_ativa > P_aparente){
                 P_ativa = P_ativa * 0.94;
                 if(P_ativa > P_aparente){
                    P_ativa = P_ativa * 0.94;
                    if(P_ativa > P_aparente){
                       P_ativa = P_ativa * 0.9;
                       if(P_ativa > P_aparente){
                    P_ativa = P_ativa * 0.9;
                    }
                  }
                }
              }
            } 

      
            P_reativa = sqrtf(powf(P_aparente,2) - powf(P_ativa,2));


           /* Serial.print("VRMS: ");
            Serial.println(VRMS);
            Serial.print("IRMS: ");
            Serial.println(IRMS);
            Serial.print("PATIVA: ");
            Serial.println(P_ativa);
            Serial.print("PARENT: ");
            Serial.println(P_aparente);*/

            
            
             

            digitalWrite(LED1, 1);
            delay(300);
            digitalWrite(LED1, 0);
            acc_v = 0;
            acc_i = 0;
            acc_p = 0;
            CONT_AMOSTRAS = 0;

            kWh = P_ativa + kWh;
           
        
            

        }
        GPIO.out_w1ts = (1 << MCP_CS); 
      }
  }
}

void subs(void * pvParameters){
  while(true){

    //-----------------------------------------------------------------------------------------
    //DHT11
    String post8 = String("[{\"variable\":\"humi\",\"value\":")+String(h)+String("}]");
    wmm2.client->publish("\\smart4.0", post8.c_str(), true);
  
    String post9 = String("[{\"variable\":\"temp\",\"value\":")+String(t)+String("}]");
    wmm2.client->publish("\\smart4.0", post9.c_str(), true);
  
    //-----------------------------------------------------------------------------------------
    
    //AI0
    String post = String("[{\"variable\":\"ai00\",\"value\":")+String(leituraAI0)+String("}]");
    wmm2.client->publish("\\smart4.0", post.c_str(), true);
  
    //AI1
    String post2 = String("[{\"variable\":\"ai01\",\"value\":")+String(leituraAI1)+String("}]");
    wmm2.client->publish("\\smart4.0", post2.c_str(), true);

    //-----------------------------------------------------------------------------------------

  
    //ANALISADOR
    String post3 = String("[{\"variable\":\"vrms\",\"value\":")+String(VRMS)+String("}]");
    wmm2.client->publish("\\smart4.0", post3.c_str(), true);
  
    String post4 = String("[{\"variable\":\"irms\",\"value\":")+String(IRMS)+String("}]");
    wmm2.client->publish("\\smart4.0", post4.c_str(), true);
  
    String post5 = String("[{\"variable\":\"appp\",\"value\":")+String(P_aparente)+String("}]");
    wmm2.client->publish("\\smart4.0", post5.c_str(), true);
  
    String post6 = String("[{\"variable\":\"actp\",\"value\":")+String(P_ativa)+String("}]");
    wmm2.client->publish("\\smart4.0", post6.c_str(), true);
  
    String post7 = String("[{\"variable\":\"reap\",\"value\":")+String(P_reativa)+String("}]");
    wmm2.client->publish("\\smart4.0", post7.c_str(), true);

    //-----------------------------------------------------------------------------------------

    
    periodico++;
    
    if(periodico>=180)
    {
      kWh = kWh/1.83392;     //1seg / 0.54528s(varredura variavel) = 0,000284(CS) * 1920 (AMOSTRAS)
      //avol = 

      String post12 = String("[{\"variable\":\"cons\",\"value\":")+String(kWh)+String("}]");
      wmm2.client->publish("\\smart4.0", post12.c_str(), true);
  
      //String post13 = String("[{\"variable\":\"avol\",\"value\":")+String(avol)+String("}]");
      //wmm.client->publish("\\smart4.0", post13.c_str(), true);

      avol = 0;
      kWh = 0;
      periodico=0;
      
    }


    if(csts != ESTADO){
        //ESTADOS
        String post10 = String("[{\"variable\":\"psts\",\"value\":")+String(ESTADO)+String("}]");
        wmm2.client->publish("\\smart4.0", post10.c_str(), true);
        csts = ESTADO;
    }
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}


