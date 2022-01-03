
#include "math.h"

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h" //you can find it on the github
#include "MPU6050.h"

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;


struct Vector
{
    float X;
    float Y;
    float Z;
};


struct arr2d {
 double m[3][3];
} ;

struct arr2d flx;

float amx,amy,amz;
double rangePerDigit = 0.000061f;

Vector na, ng;
Vector normalization;

#define LED_PIN 13
bool blinkState = false;
int8_t MPU6050_buffer[14];

Vector readNormalizeAccel(void)
{
    int16_t x, y, z ; 
    accelgyro.getAcceleration(&x, &y, &z);
    na.X = x * rangePerDigit * .980665f;
    na.Y = y * rangePerDigit * .980665f;
    na.Z = z * rangePerDigit * .980665f;

    return na;
}

void GenerateInitVector (float *ax, float *ay, float *az){
  int16_t counter=0;
  //int16_t x, y, z ; 
  float AX ,AY ,AZ; 
  struct Vector nrml;
  Serial.print("wating for init vector to generate rotation matrix ..");
  for (int16_t i=0 ; i<50 ; i++ ){
    nrml = readNormalizeAccel();
    counter++;
    AX += nrml.X; 
    AY += nrml.Y;
    AZ += nrml.Z;
    delay(50);
  }
  *ax = AX/(counter);
  *ay = AY/(counter);
  *az = AZ/(counter); 
 
}


//create 2D array
arr2d GenerateRotationMatrix(){
  struct arr2d arr ;
  float Accel_total;
  float ax, ay , az;
  GenerateInitVector (&ax, &ay, &az);
  char gravity_vector[4]= {0,0,-1};
  Accel_total = sqrt (ax*ax + ay*ay + az*az);
  float AF[3] = {ax/Accel_total, ay/Accel_total, az/Accel_total};
  float d =AF[0]*AF[0] + AF[1]*AF[1];
  arr.m[0][0]=(AF[1]*AF[1]-AF[0]*AF[0]*AF[2])/d;
  arr.m[0][1]=(-AF[0]*AF[1] - AF[0]*AF[1]*AF[2])/d;
  arr.m[0][2]= AF[0];
  arr.m[1][0]=(-(AF[0]*AF[1]) - (AF[0]*AF[1]*AF[2]))/d;
  arr.m[1][1]=((AF[0]*AF[0]) -(AF[1]*AF[1]*AF[2]))/d ;
  arr.m[1][2]= AF[1] ;
  arr.m[2][0]=(AF[0])*(-1) ; 
  arr.m[2][1]=(AF[1])*(-1) ;
  arr.m[2][2]=(AF[2])*(-1) ;
  //Serial.print(arr);
  return arr;
}

// this function return final dat(after optimization)
Vector Final_Data (struct arr2d *mat)
{
  float ax,ay,az;
  struct Vector norm;
  struct Vector normalmatrix;
  norm = readNormalizeAccel();
  normalmatrix.X = (norm.X)*mat->m[0][0]+(norm.Y)*mat->m[0][1]+ (norm.Z)*mat->m[0][2];
  normalmatrix.Y = (norm.Y)*mat->m[1][0]+(norm.Y)*mat->m[1][1]+ (norm.Z)*mat->m[1][2];
  normalmatrix.Z = (norm.Z)*mat->m[2][0]+(norm.Y)*mat->m[2][1]+ (norm.Z)*mat->m[2][2];
  return normalmatrix;
}

void setup() {
   
    Wire.begin();
    Serial.begin(9600);
    
    accelgyro.initialize();
    
    delay(200);

    flx = GenerateRotationMatrix(); 
    for (int i=0 ;i<3;i++){
      Serial.println();
      for (int j=0 ; j<3 ;j++ ){
        Serial.print(flx.m[i][j]);Serial.print("\t");
      }
    } 
}


void loop() {

    normalization = readNormalizeAccel();
    Serial.print("raw normal -->");
    Serial.print(normalization.X); Serial.print("\t");
    Serial.print(normalization.Y); Serial.print("\t");
    Serial.println(normalization.Z);
  
    delay(200);
    normalization = Final_Data (&flx);

    
    Serial.print(normalization.X); Serial.print("\t");
    Serial.print(normalization.Y); Serial.print("\t");
    Serial.print(normalization.Z); Serial.print("\t");

    delay(200);
  
    
 
}
