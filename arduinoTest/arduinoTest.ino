#include<Servo.h>
Servo servo1; //X axis
Servo servo2; //Y axis

void setup() {
  servo1.attach(5);
  servo2.attach(6);
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT); //задаем режим работы на вывод данных для контакта 13, к которому подключен светодиод
  digitalWrite (LED_BUILTIN, LOW);
  Serial.println("Hi!, I am Arduino"); // передаем приветственное сообщение
}

void myXy (char *s)
{
  int x, y;
  sscanf(s, "%d,%d", &x, &y);

  char t[40];
  sprintf(t, "x %d, y %d", x, y);
  Serial.println(t);
  servo1.write(x);
  servo2.write(y);
}

void loop()
{
  static char s[40];
  static int  i = 0;

  if (Serial.available())  {
    s[i] = Serial.read();
    if (s[0] > '4')
      digitalWrite(LED_BUILTIN, HIGH);
    else if (s[0] < '4')
      digitalWrite(LED_BUILTIN, LOW);

    if ('\n' == s[i++])  {
      s [i - 1] = 0;
      i = 0;
      myXy(s);
    }
  }
}
