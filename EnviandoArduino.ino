/*//Enviando data entre Arduino e NodeMCU
//arduino 

#include <SoftwareSerial.h>
SoftwareSerial a(18,19);
void setup(){
  a.begin(9600);
}
void loop(){
  int data=50;
  if (a.available()>0){
    char c=a.read();
    if(c=='a'){
      a.write(data);
    }
  }
}*/

int batimentos = 78;  // Substituir pelo valor real dos batimentos
int spo2 = 97; 
// this sample code provided by www.programmingboss.com
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
}
void loop() {
    Serial1.print("Batimentos:");
    Serial1.println(batimentos);
    Serial1.print(F(", SPO2="));
    Serial1.println(spo2, DEC);
    delay(1500);
}