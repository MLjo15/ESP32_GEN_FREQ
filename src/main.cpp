#include <Arduino.h>

// Pino GPIO que será a saída de frequência (por exemplo, GPIO 2, LED onboard na maioria dos ESP32 Dev Kits)
const int FREQUENCY_OUT_PIN = 2; 

// Frequências disponíveis em Hz (de 1 Hz a 2 kHz)
// Você pode ajustar esta lista conforme suas necessidades
const int frequencies[] = {1, 10, 50, 100, 250, 500, 750, 1000, 1500, 2000}; 
const int numFrequencies = sizeof(frequencies) / sizeof(frequencies[0]);
int currentFrequencyIndex = 0;

// Variáveis para controle do tempo de troca de frequência
unsigned long lastFrequencyChangeTime = 0;
const unsigned long frequencyChangeInterval = 5000; // 5 segundos em milissegundos

void setup() {
  Serial.begin(115200);

  // Configura o pino de saída de frequência como OUTPUT
  pinMode(FREQUENCY_OUT_PIN, OUTPUT);

  //Serial.println("ESP32 Gerador de Frequência Automático");
  //Serial.print("Frequência inicial: ");
  //Serial.print(frequencies[currentFrequencyIndex]);
  //Serial.println(" Hz");

  // Configura o canal LEDC inicial
  ledcSetup(0, frequencies[currentFrequencyIndex], 10); 
  ledcAttachPin(FREQUENCY_OUT_PIN, 0);
  ledcWrite(0, 512); // 50% de ciclo de trabalho
}

void loop() {
  // Verifica se já se passaram 5 segundos desde a última troca de frequência
  if (millis() - lastFrequencyChangeTime >= frequencyChangeInterval) {
    // Avança para a próxima frequência na lista
    currentFrequencyIndex = (currentFrequencyIndex + 1) % numFrequencies;
    
    //Serial.print("Trocando para frequência: ");
    //Serial.print(frequencies[currentFrequencyIndex]);
    //Serial.println(" Hz");

    // Atualiza o tempo da última troca
    lastFrequencyChangeTime = millis();

    // Reconfigura o canal LEDC com a nova frequência
    ledcSetup(0, frequencies[currentFrequencyIndex], 10); 
    ledcAttachPin(FREQUENCY_OUT_PIN, 0);
    ledcWrite(0, 512); // Mantém 50% de ciclo de trabalho
  }

  // Não há necessidade de delay aqui, o loop pode rodar livremente
  // A geração de frequência é feita em hardware pelo LEDC
}