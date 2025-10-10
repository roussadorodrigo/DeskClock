#define QueueSize 10
#define DEBUG 1


//Define Tasks
void NTP_Task(void *pvParameters);
void RTC_Task(void *pvParameters);
void APP_Task(void *pvParameters);
void Display_Task(void *pvParameters);
void Sensor_Task(void *pvParameters);


//Define Queues handle
QueueHandle_t QueueHandle_Sensor;
QueueHandle_t QueueHandle_RTC;
QueueHandle_t QueueHandle_NTP;
QueueHandle_t QueueHandle_LCD;


//Mudar quando se decidir as estroturas de dados
typedef struct {
  char line[10];
  uint8_t line_length;
} message_t;


void setup() 
{
  Serial.begin(115200);// Initialize serial communication at 115200 bps
  QueueHandle_Sensor = xQueueCreate(QueueSize, sizeof(message_t));
  QueueHandle_NTP = xQueueCreate(QueueSize, sizeof(message_t));
  QueueHandle_RTC = xQueueCreate(QueueSize, sizeof(message_t));
  QueueHandle_LCD = xQueueCreate(QueueSize, sizeof(message_t));

  // Check if the queue was successfully created
  if (QueueHandle_Sensor == NULL || QueueHandle_NTP == NULL || QueueHandle_RTC == NULL || QueueHandle_LCD == NULL) 
  {
    if(DEBUG) Serial.println("Queue could not be created. Halt.");
    while (1) 
    {
      delay(1000);  // Halt at this point as is not possible to continue
    }
  }

  xTaskCreate(NTP_Task,"NTP",2048,NULL,1,NULL);
  xTaskCreate(RTC_Task,"RTC",2048,NULL,1,NULL);
  xTaskCreate(Sensor_Task,"Sensor",2048,NULL,1,NULL);
  xTaskCreate(APP_Task,"APP",2048,NULL,1,NULL);
  xTaskCreate(Display_Task,"APP",2048,NULL,1,NULL);
}

void loop() 
{
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}


void NTP_Task(void *pvParameters)
{
//Quando inicializa vais buscar as horas, 
//depois acorada periodicamente(1 dia) para verificar que as horas estão certas

//Envia pela QueueHandle_NTP para a APP_taks uma estrutura de dados relacionada com o NTP

}

void RTC_Task(void *pvParameters)
{
//Enviao pela QueueHandle_RTC as horas para a APP_TASK, 
//é responsável por ler o valor do RTC por I2C

}

void Sensor_Task(void *pvParameters)
{
//Enviao pela QueueHandle_Sensor a temperatura para a APP_TASK, 
//é responsável por ler o valor do sensor por I2C

}


void APP_Task(void *pvParameters)
{
// Recebe estruturas de dados e envia para a Display_Task pela QueueHandle_LCD
}

void Display_Task(void *pvParameters)
{
//Envia para o display via I2C os dados já processados
}


