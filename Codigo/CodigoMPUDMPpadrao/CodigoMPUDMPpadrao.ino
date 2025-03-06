#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

// Definição do MPU6050 no endereço padrão
MPU6050 mpu;

// Definição de pinos no ESP32
#define SDA_PIN 21 // SDA conectado ao pino D21
#define SCL_PIN 22 // SCL conectado ao pino D22
#define INTERRUPT_PIN 4 // Pino de interrupção configurado como D4

/* Variáveis de controle */
volatile bool MPUInterrupt = false;  // Indica se ocorreu interrupção
bool DMPReady = false;               // Indica se o DMP está pronto
uint8_t devStatus;                   // Status da inicialização do DMP
uint16_t packetSize;                 // Tamanho do pacote esperado no FIFO
uint8_t FIFOBuffer[64];              // Buffer do FIFO do MPU6050

/* Variáveis de orientação */
Quaternion q;        // Quaternion [w, x, y, z]
VectorFloat gravity; // Vetor de gravidade [x, y, z]
float ypr[3];        // Yaw, Pitch, Roll em radianos

/* Rotina de interrupção */
void IRAM_ATTR DMPDataReady() {
  MPUInterrupt = true;
}

void setup() {
  // Inicialização da comunicação serial
  Serial.begin(9600);
  while (!Serial); // Aguarda conexão serial

  // Inicialização do I2C no ESP32
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(400000); // Velocidade do barramento I2C

  // Inicialização do MPU6050
  Serial.println("Inicializando MPU6050...");
  mpu.initialize();
  pinMode(INTERRUPT_PIN, INPUT_PULLUP); // Configuração do pino de interrupção

  // Verificar conexão com o MPU6050
  if (!mpu.testConnection()) {
    Serial.println("Erro: Conexão com o MPU6050 falhou!");
    while (1);
  }
  Serial.println("Conexão com MPU6050 bem-sucedida.");

  // Inicialização do DMP
  Serial.println("Inicializando DMP...");
  devStatus = mpu.dmpInitialize();

  // Configurar offsets (ajustar conforme necessário para sua unidade)
  mpu.setXAccelOffset(-1886);
  mpu.setYAccelOffset(-1146);
  mpu.setZAccelOffset(520);
  mpu.setXGyroOffset(82);
  mpu.setYGyroOffset(-1);
  mpu.setZGyroOffset(0);

  // Verificar se a inicialização do DMP foi bem-sucedida
  if (devStatus == 0) {
    Serial.println("DMP inicializado com sucesso. Calibrando...");

    // Calibrar acelerômetro e giroscópio
    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    Serial.println("Calibração concluída. Ativando DMP...");

    // Ativar o DMP
    mpu.setDMPEnabled(true);

    // Habilitar a detecção de interrupções
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), DMPDataReady, RISING);
    DMPReady = true;

    // Obter o tamanho esperado do pacote do DMP
    packetSize = mpu.dmpGetFIFOPacketSize();
    Serial.println("DMP pronto. Aguardando dados...");
  } else {
    // Caso de falha na inicialização do DMP
    Serial.print("Erro: Falha ao inicializar DMP (código ");
    Serial.print(devStatus);
    Serial.println(")");
    while (1);
  }
}

void loop() {
  // Se o DMP não estiver pronto, encerra o loop
  if (!DMPReady) return;

  // Verificar se houve interrupção
  if (MPUInterrupt) {
    MPUInterrupt = false;

    // Lê o pacote do FIFO
    if (mpu.dmpGetCurrentFIFOPacket(FIFOBuffer)) {
      // Obter os valores do quaternion
      mpu.dmpGetQuaternion(&q, FIFOBuffer);

      // Calcular o vetor de gravidade
      mpu.dmpGetGravity(&gravity, &q);

      // Calcular yaw, pitch e roll
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

      // Converter para graus e exibir os valores
      Serial.print("Yaw: ");
      Serial.print(ypr[0] * 180 / M_PI);
      Serial.print(", Pitch: ");
      Serial.print(ypr[1] * 180 / M_PI);
      Serial.print(", Roll: ");
      Serial.println(ypr[2] * 180 / M_PI);
    }
  }
}
