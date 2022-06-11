#include <PID_v1.h>
#include <stdint.h>
#include <SPI.h>
#include <Wire.h>
#include <Servo.h>

int flag, flagZ;

float xVal, yVal;
int cCount = 0;
int flagC = 0;
int flagK = 0;
int fl = 0;

double l = 0.00;
unsigned int noTouchCount = 0; // viariable for noTouch
// PID values
double Setpoint, Input, Output;    // for X
double Setpoint1, Input1, Output1; // for Y
//
int Modulo;
long lastcas = 0;
// servos variables
Servo servo1; // X axis
Servo servo2; // Y axis

uint16_t homeX = 550; // raw data value for center of touchscreen
uint16_t homeY = 550; // raw data value for center of touchscreen

/////TIME SAMPLE
int Ts = 50;
unsigned long Stable = 0;
// PID const
float Kp = 0.3;
float Ki = 0.03;
float Kd = 0.13;

float Kp1 = 0.3;
float Ki1 = 0.08;
float Kd1 = 0.13;
long cas = 0;
// INIT PID
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
PID myPID1(&Input1, &Output1, &Setpoint1, Kp1, Ki1, Kd1, DIRECT);

void setup()
{
  //    servo1.attach(5);
  //    servo2.attach(6);
  //    Serial.begin(115200);
  //    pinMode(LED_BUILTIN, OUTPUT); //задаем режим работы на вывод данных для контакта 13, к которому подключен светодиод
  //    digitalWrite (LED_BUILTIN, LOW);
  Serial.println("Hi!, I am Arduino"); // передаем приветственное сообщение

  servo1.attach(5);
  servo2.attach(6);
  Output = 80;
  Output1 = 85;
  servo1.write(Output);
  servo2.write(Output1);

  Serial.begin(115200);

  Input = 240;
  Input1 = 255;
  // INIT SETPOINT
  Setpoint = 247;
  Setpoint1 = 238;
  //// Make plate flat
  servo1.attach(5);
  servo2.attach(6);
  Output = 80;
  Output1 = 85;
  servo1.write(Output);
  servo2.write(Output1);

  // Zapnutie PID
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(20, 160);
  myPID1.SetMode(AUTOMATIC);
  myPID1.SetOutputLimits(20, 160);
  // TIME SAMPLE
  myPID1.SetSampleTime(Ts);
  myPID.SetSampleTime(Ts);
  /////
  delay(100);
}

void myXy(char *s)
{
  int x, y;
  sscanf(s, "%d,%d", &x, &y);

  char t[40];
  sprintf(t, "x %d, y %d", x, y);
  //  Serial.println(t);

  while (Stable < 125) // REGULATION LOOP
  {
    if (true) // ball is on plate
    {
      noTouchCount = 0;
      Input = x;  // read and convert X coordinate
      Input1 = y; // read and convert Y coordinate

      if ((Input > Setpoint - 30 && Input < Setpoint + 30 && Input1 > Setpoint1 - 30 && Input1 < Setpoint1 + 30)) // if ball is close to setpoint
      {
        Stable = Stable + 1; // increment STABLE
      }
      myPID.Compute();  // action control X compute
      myPID1.Compute(); //   action control  Y compute
    }
    servo1.write(Output);  // control
    servo2.write(Output1); // control
    
    /*Serial.print(Output);
    Serial.print(",");
    Serial.print(Output1);
    Serial.println(";");
    
    Serial.print(Input);
    Serial.print(",");
    Serial.println(Input1);
    Serial.println("---");*/
  } ////END OF REGULATION LOOP///

  /// KONTROLA STABILITY////
  while (Stable == 125)                                                                                   // if is stable
  { // still measure actual postiion
    Input = x;                                                                                            // read X
    Input1 = y;                                                                                           // read Y
    if (Input < Setpoint - 2 || Input > Setpoint + 2 || Input1 > Setpoint1 + 2 || Input1 < Setpoint1 - 2) // if ball isnt close to setpoint
    {
      Stable = 0; // change STABLE state
    }

  } // end of STABLE LOOP
}

void loop()
{
  static char s[40];
  static int i = 0;

  if (Serial.available())
  {
    s[i] = Serial.read();
    if (s[0] > '4')
    {
      servo1.write(180);
      servo2.write(180);
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else if (s[0] < '4')
    {
      digitalWrite(LED_BUILTIN, LOW);
      servo1.write(0);
      servo2.write(0);
    }
    if ('\n' == s[i++])
    {
      s[i - 1] = 0;
      i = 0;
      myXy(s);
    }
  }
}
