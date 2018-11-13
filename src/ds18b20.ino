#include <OneWire.h> // Biblioteca de comunicação One Wire
#include <DallasTemperature.h> // Biblioteca do sensor de temperatura 
#define PINO_ONE_WIRE 4 // Pino onde os sensores devem estar conectados
#define PRECISAO_TEMPERATURA 12 // Precisão da temperatura lida do sensor ( pode variar entre 9 mínimo e 12 máximo )
#define TEMPOESPERA 2000 // Tempo de espera entre leituras
OneWire oneWire(PINO_ONE_WIRE); // Objeto responsável pela comunicação onewire
DallasTemperature sensores(&oneWire); // Objeto responsável pela configuração e leitura de todos os sensores de temperatura
DeviceAddress enderecoDispositivo; // Endereço do dispositivo que realizaremos a leitura

int dispositivosConectados = 0; // Quantidade de dispositivos conectados ao barramento One-Wire

void setup() {
  Serial.begin(9600); // Inicialização da serial
  sensores.begin(); // Inicialização do objeto de comunicação com os sensores
  dispositivosConectados = sensores.getDeviceCount(); // Conta o número de dispositivos conectados ao barramento One-Wire
  Serial.print("Dispositivos Encontrados: "); // Mostra a quantidade dispositivos conectados ao barramento
  Serial.println(dispositivosConectados,DEC);

  for (int i=0; i<dispositivosConectados; i++){ // Itera sob todos os dispositivos conectados no barramento One Wire
    if (sensores.getAddress(enderecoDispositivo,i)){ // Se um dispositivo estiver conectado
      Serial.println("Dispositivo Encontrado"); // Mostro que o dispositivo foi encontrado e o endereço onde ele está
      Serial.print("Endereço do dispositivo: ");
      mostrarEndereco();
      sensores.setResolution(enderecoDispositivo,PRECISAO_TEMPERATURA); // Configuro a taxa de precisão do sensor de temperatura ( 9 mínimo e 12 máximo)
      Serial.print("Sensor: ");
      Serial.print(i);
      Serial.print(" Configurado com precisão de: ");
      Serial.print(PRECISAO_TEMPERATURA);
      Serial.println(" Bits");
    }
  }
}


void mostrarEndereco(){ // Função que mostra o endereço do dispositivo conectado ao barramento
  for (byte i=0; i<8; i++){ // Endereço composto por um de 8 bytes (64 bits)
    if (enderecoDispositivo[i] <16){ // Se o número for menor que 16 escrevemos o valor 0
      Serial.print("0");
    }
    else{
      Serial.print(enderecoDispositivo[i],HEX); // Caso contrário escrevemos o byte correspondente ao endereço
    }
  }
  Serial.println();
}


void loop() {
  sensores.requestTemperatures(); // Envia um comando em broadcast para que todos os sensores realizem a leitura da temperatura
  for (int i=0; i<dispositivosConectados; i++){ // Realizo uma iteração sob todos os sensores conectados
    if (sensores.getAddress(enderecoDispositivo,i)){ // Se um dispositivo estiver conectado
      float temperaturaCelsius = sensores.getTempC(enderecoDispositivo); // Pego a temperatura do sensor ( valor em Celsius )
      Serial.print("Temperatura sensor: "); // Mostro o valor da tempratura em celsius e Fahrenheit
      Serial.print(i);
      Serial.print(" Celsius: ");
      Serial.print(temperaturaCelsius);
      Serial.print(" Fahrenheit: ");
      Serial.println(DallasTemperature::toFahrenheit(temperaturaCelsius));
    }
  }
  delay(TEMPOESPERA); // Aguardo o intervalo de tempo necessário entre as leituras
}
