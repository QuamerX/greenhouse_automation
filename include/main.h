#ifndef MAIN_H_   
#define MAIN_H_

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <DHT.h>
#include "BluetoothSerial.h"

#define DHTPIN 6
#define DHTTYPE DHT11   

const int freq;                                             //Defining PWM output configurations
const int pwmChannel;
const int resolution;

const int fan_turn_way_right_pin;                             //Defining Input and Output pin numbers
const int fan_turn_way_left_pin;
const int fan_motor_speed_control_pin;
const int cooler_relay_pin;
const int heater_relay_pin;
const int water_sprayer_pin;
const int uv_light_relay_pin;
const int co2_sensor_pin;
const int soil_humidity_sensor_pin;
const int water_pump_pin;
const int light_sensor_pin;
const int rain_sensor_pin;
const int roof_motor_pin;

int soil_humidity_value;                                       //Defining variables for sensor data readings
int co2_value;
int DHT11_temprature;
int DHT11_humidity;
int light_value;
int rain_value;

int fan_speed_control_value;                                 //Defining mobile app controlled variables and initial values
int co2_control_value;
int DHT11_heat_control_value;
int DHT11_cool_control_value;
int DHT11_humid_control_value;
int soil_humid_control_value;
int light_control_value;
int rain_value_control;

DHT DHT11_sensor (DHTPIN, DHTTYPE);                                //Defining DHT11 sensor pins
LiquidCrystal lcd(13, 14, 34, 35, 36, 39);                         //Defining 4x16 LCD pins
BluetoothSerial serialBluetooth;          
ledcSetup(pwmChannel, freq, resolution);
ledcAttachPin(fan_motor_speed_control_pin, pwmChannel);       

void Assign_Default_Values();
void Read_Sensor_Values();
void Bluetooth_Send();                                                 //Calling previously declared functions
void Bluetooth_Receive();
void LCD_Send();
void Temp_Control();
void CO2_Control();
void Humid_Control();
void Soil_Humid_Control();
void Light_Control();
void Roof_Control();

#endif // MAIN_H_