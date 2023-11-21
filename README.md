# Monitor-de-Sinais-Vitais-

O projeto relata como construir um prototipo de monitor de sinais vitais utilizando a plataforma arduino.
Os dispositivos utilizados foram:
-Arduino Mega 2560
-Modulo Sensor De Batimentos Cardíacos - Ad8232 ECG Arduino
-Sensor oxímetro MAX30100 
- Modulo WI-FI + Bluetooth ESP32
- -LCD I2C

  
A montagem do esquematico foi feita utilizando o programa Fritzing. 

A montagem do circuito foi feito em uma placa protoboard. O sensor de batimentos cardiacos tem uma alimentação de 3.3 volts e uma saída analogica, já o sensor de oximetria possui comunicação I2C e alimentação de 5V

O LCD também possui um modulo de comunicação I2C. No caso do arduino Mega2560, há 2 pinos para o barramento de clock SCL e o barramento de dados SDA então cada dispositivo utilizou uma entrada. No caso do arduino Uno ou nano eles podem compartilhar o mesmo barramento para a escrita de dados.

O dispositivo utilizado para se comunicar com a internet foi o ESP32 que possui Wi-fi e bluetooth que é mais robusto mas poderia ser utilizado também um modulo MCU somente com wi-fi. Para a comunicação com o ESP32 com arduino utilizou-se a entrada Serial1 TX1 e RX1.

Uma vez montado e alimentado o circuito, foi iniciado o teste com os codigos. Para os sensores há uma grande disponibilidade de biblioteca de testes dentro do arduino. Para o sensor de batimentos e oximetria a sparkfun oferece codigos prontos. Existem outras bibliotecas como adafrtuit dallas para testar outras funcionalidades como temperatura.

Os sensores foram testados separadamente para averiguar seu funcionamento. Uma vez que os sensores estavam funcionando os codigos foram ajustados juntos. Depois foi iniciado o LCD e printado uma mensagem sobre a tela. 

Para a comunicação entre o Arduino e o modulo ESP32, como houve dificuldade de implementação, os codigos RecebendoEsp32 e EnviandoArduino podem ser utilizados para testar essa comunicação. Importante notar que o modulo ESP32 normalmente utiliza uma alimentação de 3v enquanto o arduino de 5v. No meu esquematico não utilizei divisor de tensão, então eu aterrei o ESP32 no ground do Arduino. Porém é recomendado faz a divisão de tensão. 
Importante anotar também como eu utilizei o serial1 (tx1 e rx1) no arduino eu precisei iniciar o Serial1 e escrever no serial1 meus dados para que o Esp32 pudesse receber. O Esp32 recebeu os dados no Serial2. 

Finalmente com o codigo completo, o ProjetoSensores.ino é o codigo final. Ele envia os dados de temperatura batimentos e oximetria para o mudulo Esp32. O codigo que descreve o ESP32 está no arquivo ESP32.ino. 
