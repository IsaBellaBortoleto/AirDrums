#include <Arduino.h>
#include <string.h>
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
#define I2S_LRC       15 //wesl
#define I2S_DOUT      22 //din
#define BUMBO  33 // Defina o pino correto para o sensor de bumbo

// Buffer size
#define AUDIO_BUFFER_SIZE 512

bool bumboTocado = false; // Flag para controle do bumbo

File audioFile;
File audioFile2;
File audioFile3;
File audioFile4;
File audioFile5;

void setup() {
  Serial.begin(115200); // Para debug no monitor serial

  Serial1.setTimeout(10);
  Serial2.setTimeout(10);
  // UART1 para a baqueta esquerda (ESQ)
  Serial1.begin(115200, SERIAL_8N1, 16, -1); // RX em GPIO16, sem TX

  // UART2 para a baqueta direita (DIR)
  Serial2.begin(115200, SERIAL_8N1, 4, -1);  // RX em GPIO9, sem TX

  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

  if (!SD.begin(SD_CS)) {
      Serial.println("Falha ao inicializar o cartão SD!");
      while (1);
  }
  Serial.println("Cartão SD inicializado!");

  // Configura o I2S
  setupI2S();

  // Abre os arquivos de áudio no SD
  audioFile = SD.open("/PDir.wav", FILE_READ);
  audioFile2 = SD.open("/PEsq.wav", FILE_READ);
  audioFile3 = SD.open("/TDir.wav", FILE_READ);
  audioFile4 = SD.open("/TEsq.wav", FILE_READ);
  audioFile5 = SD.open("/Bumbo.wav", FILE_READ);

  if (!audioFile || !audioFile2 || !audioFile3 || !audioFile4 || !audioFile5) {
      Serial.println("Falha ao abrir os arquivos de áudio!");
      while (1);
  }
  Serial.println("Arquivos de áudio carregados!");
  pinMode(BUMBO, INPUT);
}

void loop() {
  int bumboState = digitalRead(BUMBO);
  String baqueta1 = Serial1.readStringUntil('\n');
  String baqueta2 = Serial2.readStringUntil('\n');

  bool somTocando = false;

  if (baqueta1 == "Tom direito") {
      Serial.println("Reproduzindo: Tom Direito (/TDir.wav)");
      playAudio(audioFile3);
      somTocando = true;
  } 
  if (baqueta1 == "Tom esquerdo") {
      Serial.println("Reproduzindo: Tom Esquerdo (/TEsq.wav)");
      playAudio(audioFile4);
      somTocando = true;
  } 
  if (baqueta1 == "Prato esquerdo") {
      Serial.println("Reproduzindo: Prato Esquerdo (/PEsq.wav)");
      playAudio(audioFile2);
      somTocando = true;
  }
  if (baqueta1 == "Prato direito") {
      Serial.println("Reproduzindo: Prato Direito (/PDir.wav)");
      playAudio(audioFile);
      somTocando = true;
  }

  if (baqueta2 == "Tom direito") {
      Serial.println("Reproduzindo: Tom Direito (/TDir.wav)");
      playAudio(audioFile3);
      somTocando = true;
  } 
  if (baqueta2 == "Tom esquerdo") {
      Serial.println("Reproduzindo: Tom Esquerdo (/TEsq.wav)");
      playAudio(audioFile4);
      somTocando = true;
  } 
  if (baqueta2 == "Prato direito") {
      Serial.println("Reproduzindo: Prato Direito (/PDir.wav)");
      playAudio(audioFile);
      somTocando = true;
  } 
  if (baqueta2 == "Prato esquerdo") {
      Serial.println("Reproduzindo: Prato Esquerdo (/PEsq.wav)");
      playAudio(audioFile2);
      somTocando = true;
  }

  if (bumboState == HIGH) {
      Serial.println("Reproduzindo: Bumbo (/Bumbo.wav)");
      playAudio(audioFile5);
      somTocando = true;
  } 

  if (!somTocando) {
      playSilence(); // Enviar silêncio quando nenhum som está tocando
  }
}

void playAudio(File &audio) {
    if (audio.available()) {
        while (audio.available()) {
            uint8_t audioBuffer[AUDIO_BUFFER_SIZE];
            int bytesRead = audio.read(audioBuffer, AUDIO_BUFFER_SIZE);
            size_t bytesWritten;
            i2s_write(I2S_NUM_0, audioBuffer, bytesRead, &bytesWritten, portMAX_DELAY);
        }
        Serial.println("Áudio reproduzido com sucesso.");
    } else {
        Serial.println("Fim do arquivo de áudio.");
    }

    // Envia silêncio para suavizar a transição
    playSilence();

    // Limpa o buffer DMA do I2S
    i2s_zero_dma_buffer(I2S_NUM_0);

    audio.seek(0); // Reseta o arquivo para o início para futura reprodução
}


void playSilence() {
    uint8_t silenceBuffer[AUDIO_BUFFER_SIZE];
    memset(silenceBuffer, 0, sizeof(silenceBuffer)); // Preenche com zeros
    size_t bytesWritten;
    i2s_write(I2S_NUM_0, silenceBuffer, sizeof(silenceBuffer), &bytesWritten, portMAX_DELAY);
    Serial.println("Reproduzindo silêncio.");
}

void setupI2S() {
    // Configuração do driver I2S
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX), // Modo Master e transmissão
        .sample_rate = 44100,                               // Taxa de amostragem: 44.1 kHz
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,       // 16 bits por amostra
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,       // Formato estéreo
        .communication_format = I2S_COMM_FORMAT_I2S,        // Comunicação I2S padrão
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,           // Interrupção
        .dma_buf_count = 8,                                 // Número de buffers DMA
        .dma_buf_len = 64                                   // Tamanho de cada buffer
    };

    // Inicializa o driver I2S
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);

    // Configuração dos pinos I2S
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
