#include "Arduino.h"
#include "SD.h"
#include "FS.h"
#include "driver/i2s.h"

// Pin definitions
#define SD_CS          5
#define SPI_MOSI      23
#define SPI_MISO      19
#define SPI_SCK       18
#define I2S_BCLK      14
#define I2S_LRC       15
#define I2S_DOUT      22
#define BUMBO 4 // Porta do Bumbo
// Buffer size
#define AUDIO_BUFFER_SIZE 512

File audioFile;
unsigned long lastHitTime = 0;       // Armazena o tempo da última batida
const unsigned long hitDelay = 250;  // Tempo mínimo entre as batidas (em ms)
bool tocou = false;

void setupI2S() {
    // ConfiguraÃ§Ã£o do driver I2S
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX), // Modo Master e transmissÃ£o
        .sample_rate = 44100,                               // Taxa de amostragem: 44.1 kHz
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,       // 16 bits por amostra
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,       // Formato estÃ©reo
        .communication_format = I2S_COMM_FORMAT_I2S,        // ComunicaÃ§Ã£o I2S padrÃ£o
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,           // InterrupÃ§Ã£o
        .dma_buf_count = 8,                                 // NÃºmero de buffers DMA
        .dma_buf_len = 64                                   // Tamanho de cada buffer
    };

    // Inicializa o driver I2S
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);

    // ConfiguraÃ§Ã£o dos pinos I2S
    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_BCLK,     // Pino do BCLK
        .ws_io_num = I2S_LRC,       // Pino do LRC
        .data_out_num = I2S_DOUT,   // Pino do DOUT
        .data_in_num = I2S_PIN_NO_CHANGE // Sem pino de entrada
    };

    i2s_set_pin(I2S_NUM_0, &pin_config);

    // Configura o clock I2S
    i2s_set_clk(I2S_NUM_0, 44100, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_STEREO);
}

void setup() {
    // Inicializa o monitor serial
    Serial.begin(115200);

    // Inicializa o cartÃ£o SD
    pinMode(SD_CS, OUTPUT);
    digitalWrite(SD_CS, HIGH);
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

    if (!SD.begin(SD_CS)) {
        Serial.println("Falha ao inicializar o cartÃ£o SD!");
        while (1);
    }
    Serial.println("CartÃ£o SD inicializado!");

    // Configura o I2S
    setupI2S();

    // Abre o arquivo de Ã¡udio no SD
    audioFile = SD.open("/teste.wav", FILE_READ);
    if (!audioFile) {
        Serial.println("Falha ao abrir o arquivo de Ã¡udio!");
        while (1);
    }
    Serial.println("Arquivo de Ã¡udio carregado!");
    pinMode(BUMBO, INPUT_PULLDOWN);
}

void loop() {
 handleButtonPress();
}

void handleButtonPress() {
  // Read the state of the button
  int pinState = digitalRead(BUMBO);

  // Play the audio if the button is pressed
  if (pinState == LOW) { // Assuming LOW means pressed
    unsigned long currentTime = millis();

    // Ensure a delay between consecutive plays
    if (currentTime - lastHitTime > hitDelay) {
      Serial.println("BUMBO detected via button press");
      playAudio("/Bumbo.wav");
      lastHitTime = currentTime;
    }
  }
}
void playAudio(const char *fileName) {
  // Open the file from SD card
  audioFile = SD.open(fileName, FILE_READ);
  if (!audioFile) {
    Serial.println("Erro ao abrir o arquivo de áudio!");
    return;
  }

  Serial.println("Reproduzindo áudio...");

   if (audioFile.available()) {
        // Cria um buffer para armazenar os dados do arquivo
        uint8_t audioBuffer[AUDIO_BUFFER_SIZE];
        int bytesRead = audioFile.read(audioBuffer, AUDIO_BUFFER_SIZE);

        // Envia os dados para o I2S
        size_t bytesWritten;
        i2s_write(I2S_NUM_0, audioBuffer, bytesRead, &bytesWritten, portMAX_DELAY);
    } 
  Serial.println("Áudio finalizado.");
}