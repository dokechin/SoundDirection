#include <Math.h>
#include <Servo.h>

const int          DATA_COUNT       =  1000; /* DATA STOCK SIZE */
const unsigned int THREHOLD_VALUE   =    10; /* ADJUST SOUND DETECT */
const unsigned int THREHOLD_COUNT   =    50; /* ADJUST SOUND DETECT */
const unsigned int MIDIUM_VOLTAGE   =   512; /* 10bit AD CONVERTER CENTER VALUE */
const double       SOUND_SPEED      = 0.034; /* 0.034 cm/microsec */
const unsigned int SPEAKER_DISTANCE =    10; /* TWO SPEAKER'S DISTANCE cm */
const unsigned int SAMPLING_MICROSEC =   55; /* 2*ANALOG READ MICROSEC DEPENDS ON YOUR HARDWARE */
const int          OFFSET            =    5; /* ABOUT SPEKER_DISTANCE / (SOUND_SPEED * SAMPLING_MICROSEC) */

unsigned int waveLeft[DATA_COUNT];
unsigned int waveRight[DATA_COUNT];

Servo myservo;  // create servo object to control a servo

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {

  unsigned int threholdOverCount = 0;
  for(int i=0;i<DATA_COUNT;i++){
    signed int valLeft = analogRead(0) - MIDIUM_VOLTAGE;
    signed int valRight = analogRead(1) - MIDIUM_VOLTAGE;
    waveLeft[i] = valLeft;
    waveRight[i] = valRight;
    Serial.print(valLeft);
    Serial.print(" ");
    Serial.println(valRight);
    if (abs(valLeft) > THREHOLD_VALUE){
      threholdOverCount++;
    }    
  }
  if (threholdOverCount > THREHOLD_COUNT){
    int maxIndex = 0;
    signed long maxValue = 0;
    for(int j=-OFFSET; j<OFFSET; j++){
      signed long total = 0;
      for(int i=OFFSET; i<DATA_COUNT - OFFSET; i++){
        total = total + (waveLeft[i] * waveRight[i+j]);
      }
      if ( total > maxValue){
        maxValue = total;
        maxIndex = j;
      }
    }
    long tdoa = SAMPLING_MICROSEC * maxIndex;
    long theta = acos((tdoa * SOUND_SPEED) / SPEAKER_DISTANCE);
    Serial.print("maxIndex");
    Serial.println(maxIndex);
    Serial.print("theta");
    Serial.println(theta);
    myservo.write(theta);     
  }
  delay(2000);
}