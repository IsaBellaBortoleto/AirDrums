#include "Arduino.h"
#include "SD.h"
#include "FS.h"

// Pin definitions
#define SD_CS      5   // Pino CS do SD
#define SPI_MOSI  23   // Pino MOSI
#define SPI_MISO  19   // Pino MISO
#define SPI_SCK   18   // Pino SCK

// Declaração da função listarArquivos
void listarArquivos(fs::FS &fs, const char *dirname, uint8_t levels = 0);

void setup() {
    
    Serial.begin(9600);
    Serial.println("Teste iniciado!");


    Serial.println("Iniciando o teste do microSD...");

    // Inicializa o SPI antes de configurar o SD
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SD_CS);

    // Inicializa o cartão SD
    if (!SD.begin(SD_CS, SPI)) {
        Serial.println("Falha ao inicializar o cartão SD!");
        Serial.println("Verifique a conexão e tente novamente.");
        while (1); // Para execução caso falhe
    }

    Serial.println("Cartão SD inicializado com sucesso!");
    listarArquivos(SD, "/"); // Lista os arquivos no cartão SD
}

void listarArquivos(fs::FS &fs, const char *dirname, uint8_t levels) {
    Serial.printf("Listando arquivos no diretório: %s\n", dirname);

    File root = fs.open(dirname);
    if (!root) {
        Serial.println("Falha ao abrir o diretório!");
        return;
    }

    if (!root.isDirectory()) {
        Serial.println("Não é um diretório!");
        return;
    }

    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            Serial.printf("DIR: %s\n", file.name());
            if (levels) {
                listarArquivos(fs, file.name(), levels - 1);
            }
        } else {
            Serial.printf("FILE: %s  SIZE: %d bytes\n", file.name(), file.size());
        }
        file = root.openNextFile();
    }
}

void loop() {
    // Nada a fazer no loop principal para o teste
}
