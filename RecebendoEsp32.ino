/*#include <SoftwareSerial.h>
SoftwareSerial a(16,17);
int data;

void setup() {
a.begin(9600);
Serial.begin(9600);  // put your setup code here, to run once:

}

void loop() {
  a.write("a");
  if (a.available()>0){
    data=a.read();
    Serial.println(data);
    }// put your main code here, to run repeatedly:

}
*/
int bpm = 0;
int spo2 = 0;
float temperatura = 0.0;
// this sample code provided by www.programmingboss.com
#define RXp2 16
#define TXp2 17
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);


}
void loop() {
    Serial.println(Serial2.readString());
    String data = Serial2.readString();
    Serial.print(data);
    // Extraindo os valores de BPM, SPO2 e temperatura da string
    sscanf(data.c_str(), "BPM: %d\nSPO2:%d\ntemperatura: %f", &bpm, &spo2, &temperatura);
    
    // Agora você tem os valores armazenados nas variáveis bpm, spo2 e temperatura
    // Faça o que quiser com esses valores
    Serial.print("batimento: ");
    Serial.println(bpm);
    Serial.print("sat: ");
    Serial.println(spo2);
    Serial.print("temp: ");
    Serial.println(temperatura);
  

}