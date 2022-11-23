
/*ALL LIB INCLUDES*/
#include <ros.h>
#include <std_msgs/String.h>
#include <Wire.h>//FOR I2C COMMS
/*ALL LIB INCLUDES*/



const int sensor_address=0x68;  // from datadheet
int16_t AcX,AcY,AcZ,GyX,GyY,GyZ;



//nodes and publisher

std_msgs::String sensor_msg;
ros::Publisher imu("imu", &sensor_msg);
ros::NodeHandle nH;
                           
void setup()
{
  nH.initNode();
  nH.advertise(imu);
  
  Wire.begin();
  Wire.beginTransmission(sensor_address);
 
  Wire.write(0);     // iniitialise mpu
  Wire.endTransmission(1);
  Serial.begin(9600);
}
long publisher_timer;

void loop()

{

  /*from mpu i2c old code*/
  Wire.beginTransmission(sensor_address);
  Wire.write(0x3B);  
  Wire.endTransmission(0);
  Wire.requestFrom(sensor_address,14,true); 
  
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  String AX = String(AcX);
  String AY = String(AcY);
  String AZ = String(AcZ);
  String GX = String(GyX);
  String GY = String(GyY);
  String GZ = String(GyZ);
 
  String data = "A" + AX + "B"+ AY + "C" + AZ + "D" + GX + "E" + GY + "F" + GZ + "G" ;
  Serial.println(data);
  int length = data.indexOf("G") +2;
  char data_final[length+1];
  data.toCharArray(data_final, length+1);


  //mpu sending data 
  if (millis() > publisher_timer) {
    
    sensor_msg.data = data_final;
    imu.publish(&sensor_msg);
    publisher_timer = millis() + 100; //(sending frq is 10Hz)
    nH.spinOnce();
  }}
