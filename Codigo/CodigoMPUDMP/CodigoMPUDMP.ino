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
unsigned long lastHitTime = 0; // Armazena o tempo da última batida
const unsigned long hitDelay = 250; // Tempo mínimo entre as batidas (em ms)
bool tocou=false;

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
  Serial.begin(115200);
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
    mpu.CalibrateAccel(10);
    mpu.CalibrateGyro(10);
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
  
  Serial2.begin(115200, SERIAL_8N1, -1, 17);
}

void loop() {
  if (!DMPReady) return;

  if (MPUInterrupt) {
    MPUInterrupt = false;
    //Serial2.print("Comunicando com 1");
    float yaw, pitch, roll;
    getYawPitchRoll(yaw, pitch, roll);

    //Serial.print("DIR:");  
    //Serial.print(yaw);
    //Serial.print(",");
    //Serial.print(pitch);
    //Serial.print(",");
    //Serial.println(roll);

    printYawPitchRoll(yaw, pitch, roll);
    if(tocou==false){
      tocou=tocar(yaw,pitch,roll);
      /*Serial.print("Tocou eh ");
      Serial.println(tocou);*/
    }
    else{
      //Serial.println("Tocou eh verdadeiro");
    }
    if(foraAreaBatida(roll)){
      tocou=false;
      //Serial.println("Tocou eh false, ForaAreaBatida eh true");
    }

  }
  //delay(500);
}

void getYawPitchRoll(float &yaw, float &pitch, float &roll) {
  // Lê os valores de yaw, pitch e roll
  if (mpu.dmpGetCurrentFIFOPacket(FIFOBuffer)) {
    mpu.dmpGetQuaternion(&q, FIFOBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

    // Converter para graus
    yaw = ypr[0] * 180 / M_PI;
    pitch = ypr[1] * 180 / M_PI;
    roll = ypr[2] * 180 / M_PI;
  }
}

bool tocar(float yaw, float pitch, float roll) {
  unsigned long currentTime = millis(); // Tempo atual

  // Verifica o "Tom direito"
  if (yaw > 15 && yaw < 45 && roll < -10 && (currentTime - lastHitTime > hitDelay)) {// antes a condição do roll era -40 e o yaw era 20 e 45
    Serial.println("Batida detectada");
    Serial2.print("Tom direito");
    printYawPitchRoll(yaw, pitch, roll);
    lastHitTime = currentTime; // Atualiza o tempo da última batida
    return true;
  }

  // Verifica o "Prato direito"
  else if (yaw > 60 && roll < -10 && (currentTime - lastHitTime > hitDelay)) {//antes a condição do roll era -40 e yaw era 70
    Serial.println("Batida detectada");
    Serial2.print("Prato direito");
    printYawPitchRoll(yaw, pitch, roll);
    lastHitTime = currentTime;
    return true;
  }

  // Verifica o "Tom esquerdo"
  else if (yaw > -45 && yaw < -15 && roll < -10 && (currentTime - lastHitTime > hitDelay)) {//antes a condição do roll era -40 e yaw era -50 e -20
    Serial.println("Batida detectada");
    Serial2.print("Tom esquerdo");
    printYawPitchRoll(yaw, pitch, roll);
    lastHitTime = currentTime;
    return true;
  }

  // Verifica o "Prato esquerdo"
  else if (yaw < -60 && roll < -10 && (currentTime - lastHitTime > hitDelay)) {//antes a condição do roll era -40 e yaw era -70
    Serial.println("Batida detectada");
    Serial2.print("Prato esquerdo");
    printYawPitchRoll(yaw, pitch, roll);
    lastHitTime = currentTime;
    return true;
  }
  else{
    return false;
    Serial.println("Tocou esta retornando falso");
  }
  // Exemplo: Adicione mais condições se necessário
}

void printYawPitchRoll(float yaw, float pitch, float roll) {
  Serial.print("Yaw: ");
  Serial.print(yaw);
  Serial.print(", Pitch: ");
  Serial.print(pitch);
  Serial.print(", Roll: ");
  Serial.println(roll);
}

bool foraAreaBatida(float roll) {
  if (roll >5) {
    //Serial.println("Fora da area de batida!");
    return true; // Retorna verdadeiro se uma batida for detectada
  }
  return false; // Retorna falso caso contrário
}