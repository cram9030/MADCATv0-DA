#include <apa.h>

int pwmOut = 1;
int adc1 = 2;
int adc2 = 4;
int adc3 = 3;
int analog1;
int analog2;
int analog3;

void setup() {
  // put your setup code here, to run once:
  pinMode(pwmOut, OUTPUT);
  pinMode(adc1, INPUT);
  pinMode(adc2, INPUT);
  pinMode(adc3, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  analog1 = analogRead(adc1);
  analog2 = analogRead(adc2);
  analog3 = analogRead(adc3);

  analogWrite(pwmOut, analog1);

}
