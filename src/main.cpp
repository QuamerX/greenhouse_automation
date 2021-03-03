#include "main.h"

void setup() 
{
  Assign_Default_Values();
  pinMode(soil_humidity_sensor_pin, INPUT);                        //Defining Input and Output pins
  pinMode(light_sensor_pin, INPUT);
  pinMode(rain_sensor_pin, INPUT);
  pinMode(water_pump_pin, OUTPUT);
  pinMode(uv_light_relay_pin, OUTPUT);
  pinMode(water_sprayer_pin, OUTPUT);
  pinMode(co2_sensor_pin, OUTPUT);
  pinMode(roof_motor_pin, OUTPUT);
  pinMode(cooler_relay_pin, OUTPUT);
  pinMode(heater_relay_pin, OUTPUT);
  pinMode(fan_turn_way_left_pin, OUTPUT);
  pinMode(fan_turn_way_right_pin, OUTPUT);
  pinMode(fan_motor_speed_control_pin, OUTPUT);
  
  digitalWrite(fan_turn_way_right_pin, HIGH);                       //Choosing the direction of rotation for ventilation fan
  digitalWrite(fan_turn_way_left_pin, LOW);
  
  lcd.begin(16,4);
  DHT11_sensor.begin();
  serialBluetooth.begin("Greenhouse automation");
}

void loop() 
{ 
  Read_Sensor_Values();
  Bluetooth_Send();                                                 //Calling previously declared functions
  Bluetooth_Receive();
  LCD_Send();
  Temp_Control();
  CO2_Control();
  Humid_Control();
  Soil_Humid_Control();
  Light_Control();
  Roof_Control();

  delay(2000);                                                      //2 seconds delay for ease the observation and parameter control
}

void Bluetooth_Send()                                              //Creating a function to send greenhouse parameters to app via bluetooth
{
  if (serialBluetooth.available()) 
  {
    serialBluetooth.print("Temp: ");
    serialBluetooth.print(DHT11_temprature);
    serialBluetooth.println(" C");

    serialBluetooth.print("Humid: %");
    serialBluetooth.println(DHT11_humidity);
    
    serialBluetooth.print("Humid in soil: %");
    serialBluetooth.println(soil_humidity_value);

    serialBluetooth.print("CO2 in air: ");
    serialBluetooth.print(soil_humidity_value);
    serialBluetooth.println(" ppm");
  }
}

void LCD_Send()                                                    //Creating a function to send greenhouse parameters to LCD screen for monitoring
{
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(DHT11_temprature);
  lcd.print(" C");

  lcd.setCursor(0,1);
  lcd.print("Humid: %");
  lcd.print(DHT11_humidity);
  
  lcd.setCursor(0,2);
  lcd.print("Humid in soil: %");
  lcd.print(soil_humidity_value);

  lcd.setCursor(0,3);
  lcd.print("CO2 in air: ");
  lcd.print(soil_humidity_value);
  lcd.print(" ppm");
}

void CO2_Control()                                                 //Creating a function to control fan usage and speed
{
  if(co2_value<co2_control_value)
  {
    ledcWrite(pwmChannel, fan_speed_control_value);
  }
  else
  {
    ledcWrite(pwmChannel, 0);
  }
  
}

void Temp_Control()                                                //Creating a function to control cooler and heater to stabilize the temprature
{
  if(DHT11_temprature>DHT11_cool_control_value)
  {
    digitalWrite(cooler_relay_pin, HIGH);
    digitalWrite(heater_relay_pin, LOW);
  }
  else if (DHT11_temprature<DHT11_heat_control_value)
  {
    digitalWrite(heater_relay_pin, HIGH);
    digitalWrite(cooler_relay_pin, LOW);
  }
  else
  {
    digitalWrite(heater_relay_pin, LOW);
    digitalWrite(cooler_relay_pin, LOW);
  }
}

void Humid_Control()                                               //Creating a function to control humidity level depending on the plant
{
  if(DHT11_humidity<DHT11_humid_control_value)
  {
    digitalWrite(water_sprayer_pin, HIGH);
  }  
  else
  {
    digitalWrite(water_sprayer_pin, LOW);
  }
}

void Soil_Humid_Control()                                          //Creating a function to measure humidity in soil and control water pump
{
  if(soil_humidity_value>soil_humid_control_value)
  {
    digitalWrite(water_pump_pin, HIGH);
  }
  else
  {
    digitalWrite(water_pump_pin, LOW);
  }
}

void Light_Control()                                               //Creating a function to control uv lights in greenhouse at night
{
  if (light_value<light_control_value)
  {
    digitalWrite(uv_light_relay_pin, HIGH);
  }
  else
  {
    digitalWrite(uv_light_relay_pin, LOW);
  }
}

void Roof_Control()                                                //Creating a function to control openable roof of greenhouse against rain
{
  if (rain_value>rain_value_control)
  {
    digitalWrite(roof_motor_pin, HIGH);
  }
  else
  {
    digitalWrite(roof_motor_pin, LOW);
  }
}

void Bluetooth_Receive()                                           //Creating a function to change control variables via bluetooth app for different situations
{
  if(SerialBT.available()>=4)
  {
    unsigned int data_byte1 = SerialBT.read();
    unsigned int data_byte2 = SerialBT.read();
    unsigned int data = (data_byte2 *256) + data_byte1;
    if (data>=1000 && data<2000)
    {
      fan_speed_control_value = data-1000;
    }
    else if (data>=2000 && data<3000)
    {
      co2_control_value = data-2000;
    }
    else if (data>=3000 && data<4000)
    {
      DHT11_heat_control_value = data-3000;
    }      
    else if (data>=4000 && data<5000)
    {
      DHT11_cool_control_value = data-4000;
    }    
    else if (data>=5000 && data<6000)
    {
      DHT11_humid_control_value = data-5000;
    }    
    else if (data>=6000 && data<7000)
    {
      soil_humid_control_value = data-6000;
    }    
    else if (data>=7000 && data<8000)
    {
      rain_value_control = data-7000;
    }
  }      
}

void Assign_Default_Values()
{
  freq = 1000;                                                    //Defining PWM output configurations
  pwmChannel = 0;
  resolution = 8;

  fan_turn_way_right_pin = 16;                                    //Defining Input and Output pin numbers
  fan_turn_way_left_pin = 17;
  fan_motor_speed_control_pin = 4;
  cooler_relay_pin = 20;
  heater_relay_pin = 21;
  water_sprayer_pin = 33;
  uv_light_relay_pin = 23;
  co2_sensor_pin = 2;
  soil_humidity_sensor_pin = 0;
  water_pump_pin = 22;
  light_sensor_pin = 4;
  rain_sensor_pin = 12;
  roof_motor_pin = 19;

  soil_humidity_value = 0;                                       //Defining variables for sensor data readings
  co2_value = 0;
  DHT11_temprature = 0;
  DHT11_humidity = 0;
  light_value = 0;
  rain_value = 0;

  fan_speed_control_value = 127;                                 //Defining mobile app controlled variables and initial values
  co2_control_value = 350;
  DHT11_heat_control_value = 15;
  DHT11_cool_control_value = 25;
  DHT11_humid_control_value = 70;
  soil_humid_control_value = 800;
  light_control_value = 500;
  rain_value_control = 150;
}

void Read_Sensor_Values()
{
  soil_humidity_value = analogRead(soil_humidity_sensor_pin);       //Reading sensor datas and mapping them for common usage
  soil_humidity_value =  map(soil_humidity_value,600,4095,0,100);
  co2_value = analogRead(co2_sensor_pin);
  co2_value = map(co2_value,0,1350,0,5000);
  DHT11_temprature = DHT11_sensor.readTemperature();
  DHT11_humidity = DHT11_sensor.readHumidity();
  light_value = analogRead(light_sensor_pin);
  rain_value = analogRead(rain_sensor_pin);
}
