
## <h1 align="center">AIRDRUMS</h1>
<img src="https://github.com/IsaBellaBortoleto/AirDrums/blob/main/Fotos/FotoPrincipal.jpeg">

Como projeto final da disciplina de Oficina de Integração I foi realizado o AirDrums, um sistema que utiliza sensores de movimento e tecnologia digital para simular a experiência de tocar bateria sem a necessidade de um instrumento físico volumoso.
A proposta do projeto é oferecer uma alternativa compacta, permitindo a prática sem restrições de espaço ou ruído.

## Desenvolvedores  

- Bruna Medeiros Teixeira  
- Isabela Bella Bortoleto  
- Guilherme de Souza Carneiro Garcia  


## ⚙️ Componentes Utilizados  

- **ESP32 Dev Kit V1**: Microcontrolador principal do projeto, responsável por processar os dados dos sensores, interpretar os movimentos e gerenciar a reprodução dos sons da bateria virtual. Nesse contexto, dois ESP32 são usados para capturar os dados das baquetas e um ESP32 que processa os sinais e executa o áudio. A partir de agora, ele será chamado de ESP32 central.  

- **IMU MPU6050 (Inertial Measurement Unit - Unidade de Medição Inercial)**: Unidade de acelerômetro e giroscópio integrado às baquetas, detecta os movimentos do usuário, identificando a posição das batidas na bateria virtual.  

- **DAC UDA1334A (Digital-to-Analog Converter - Conversor Digital-para-Analógico)**: Converte os sinais digitais de áudio do ESP32 central em sinais analógicos, permitindo a reprodução dos sons da bateria através de alto-falantes ou fones de ouvido.  

- **MicroSD**: Armazena os arquivos de áudio das peças da bateria (tons, pratos e bumbo) e fornece os dados para reprodução conforme os comandos recebidos.  

- **Botão Push-Button**: Botão de ativação, aciona manualmente o som do bumbo, simulando o pedal tradicional da bateria.  

- **Fonte de Alimentação para protoboard 3,3/5 V**: Responsável por fornecer energia para todos os módulos eletrônicos do sistema.  

Além deles, foram utilizadas placas universais (PCBs) para a integração elétrica das baquetas e do ESP32 central, uma estrutura de madeira projetada para acomodar os componentes e garantir sua segurança, cabos Manga para comunicação e alimentação das baquetas virtuais e uma estrutura de tubos de PVC para proteger e realizar o acabamento das baquetas, um resistor de 10kΩ para limitar a corrente no circuito do botão e massa para madeira para aperfeiçoar o acabamento da estrutura externa. Também foram utilizados jumpers para conectar os diversos módulos do circuito.

## 🔨 Desenvolvimento  

Os tópicos do desenvolvimento seguirão o fluxo de informações enviadas pelos componentes da bateria: ao serem acionadas, as baquetas ou o bumbo enviam os dados para o ESP32 central, que então processa e reproduz o som correspondente.  

A **Figura 2** apresenta uma representação geral do projeto.  

> <img src="https://github.com/IsaBellaBortoleto/AirDrums/blob/main/Vetores/diagrama-novo.jpg"> <br/>
> Figura 2: Diagrama funcional das AirDrums, Fonte: Autoria Própria.


### 🎯 Funcionamento das Baquetas  

O funcionamento das baquetas é baseado na integração de um **ESP32** e um **módulo MPU-6050** em cada baqueta, como mostra a **Figura 2**. Ao ligar o aparelho, o módulo realiza uma calibração inicial e começa a captar os giros de dois eixos principais:  

- O giro no eixo **Y** é usado para captar os movimentos de batida.  
- O giro no eixo **Z** é utilizado para determinar a região das partes da bateria.  

No código das baquetas, foi utilizada a biblioteca do módulo desenvolvida **Electronic Cats (Cats, 2025)**, que possibilita a ativação do **Digital Motion Processor (DMP)**.  

O **DMP** é um circuito integrado ao **MPU-6050**, que tem como objetivo refinar a carga do processamento das informações fornecidas pelo módulo, realizando cálculos complexos e aplicando filtros nos dados brutos captados pelo sensor. Esse processamento é realizado por um código proprietário, desenvolvido pela fabricante do módulo, a **InvenSense**, de código fechado.  

Como resultado, ele entrega informações mais elaboradas, na forma dos ângulos **Yaw** (Guinada, rotação no eixo Z), **Pitch** (Arfagem, rotação no eixo X) e **Roll** (Rolamento, rotação no eixo Y), como são chamados após a aplicação dos filtros internos, processados diretamente pelo DMP.  

Com isso, definimos zonas de ativação para cada parte da bateria através das baquetas, utilizando a posição relativa ao eixo **Z (Yaw)**. A **Figura 3** descreve os pontos de ativação de cada componente da bateria, assim como os ângulos de ativação.  

As zonas foram assim determinadas para respeitar as partes naturais da bateria, garantir precisão e fácil utilização do aparelho, sendo que as **faixas neutras** não correspondem a nenhum som e foram definidas para evitar conflito entre os pratos e tons.  

> <img src="https://github.com/IsaBellaBortoleto/AirDrums/blob/main/Vetores/mapa_dos_pratos2.jpg"> <br/>
> Figura 3: Ângulos de Ativação das Partes da Bateria, Fonte: Autoria Própria.<br />

Para o lançamento do som, utilizamos a posição relativa ao eixo **Y (Roll)**, é utilizado um ângulo de 10° para ativação, uma vez que corresponde ao movimento natural de tocar a bateria.  
O **ESP32** trabalha de forma sequencial para identificar a ativação da peça, o módulo envia essa informação para a parte central. Toda a comunicação dos módulos **MPU** é feita através do protocolo **I2C**, que manda os dados pós-processados para as **ESP32** da baqueta, então esses identificam a parte da bateria e enviam essa informação em formato de texto através da porta de comunicação **TX** utilizando o protocolo de comunicação **UART** (**Universal Asynchronous Receiver-Transmitter**), utilizado na troca de informações entre o microcontrolador da baqueta e o central, responsável por processar e reproduzir o som correto.  

Essa informação é transmitida por meio de um cabo **Manga** de três vias, que, além de transmitir os dados, também fornece alimentação de 5 V para todo o sistema.  

Por fim, as baquetas foram montadas em uma estrutura de **PVC**, projetada para se assemelhar ao instrumento original e proporcionar maior ergonomia durante o uso. A estrutura é composta por dois tubos de **PVC**, um com diâmetro de 40 mm e outro de 20 mm, conectados por luvas de redução por meio de encaixe por pressão. Para acabamento e proteção das extremidades, foram utilizadas tampas apropriadas para cada diâmetro. As baquetas possuem um comprimento de 28,5 cm no total.  

### 🔊 Implementação do som  
Após as baquetas detectarem a parte correspondente da bateria, essas informações são enviadas para o **ESP32** central, responsável pelo processamento dos dados e reprodução do som. Nele, temos três módulos integrados ao projeto:

- **Módulo Micro-SD**: O sistema utiliza um cartão microSD para armazenar os arquivos de áudio das diferentes peças do instrumento. O microprocessador central acessa esses arquivos, que estão em formato digital, através da biblioteca **SD.h**. Para otimizar a reprodução, os dados dos arquivos são lidos sequencialmente e armazenados temporariamente em um buffer na memória do **ESP32**, antes de serem processados e enviados para os alto-falantes.
  
- **Botão de Ativação do Bumbo**: O **ESP32** central controla a ativação do bumbo por meio de um botão (**push button**), a detecção do acionamento ocorre através da variação da diferença de potencial (**DDP**) no pino digital do **ESP32** ao qual o botão está conectado. Quando pressionado, o botão fecha o circuito, alterando o nível lógico do pino de entrada, permitindo que o **ESP32** identifique a ativação do comando. O botão está conectado eletricamente à placa de circuito impresso (**PCB**) central por meio de um fio soldado de 2,0 m de comprimento. Para facilitar o acionamento com o pé, o botão foi integrado a uma estrutura de madeira com uma tampa angulada, projetada para dar mais conforto e ergonomia durante o uso. A **Figura 6** demonstra o modelo 3D projetado para a caixa do bumbo, feito no **SolidWorks**.  

As dimensões do bumbo são:  
- Parte superior: quadrado de 8,0 cm x 8,0 cm;  
- Frente: retângulo de 7,1 cm x 8,1 cm;  
- Laterais trapezoidais: base inferior de 8 cm, base superior inclinada de 8,5 cm, altura variando entre 7,0 cm e 9,0 cm;  
- Parte traseira: retângulo de 10,3 cm x 8,1 cm;  
- Fundo: retângulo de 8,2 cm x 8,0 cm.  

> <img src="https://github.com/IsaBellaBortoleto/AirDrums/blob/main/Modelos_3D/bumbo.png" height: auto width="545.5"> <br/>
> Figura 4. Modelo 3D da Estrutura do Bumbo. Fonte: Autoria Própria.


- **Módulo DAC UDA1334**: Após a abertura dos arquivos de áudio no módulo microSD e o acionamento de uma peça da bateria por meio das baquetas ou o toque no botão que integra o bumbo, inicia-se o processo de reprodução do som. O ESP32 utiliza a interface I2S, um protocolo otimizado para transmissão de áudio digital com baixa latência. A biblioteca I2S.h permite maior flexibilidade e ajustes na configuração do áudio com o DAC UDA1334. Para o projeto, escolhemos um padrão de áudio amplamente utilizado, com taxa de amostragem de 44,1 kHz e 16 bits. Além disso, foi utilizado o formato WAV, um formato de áudio não compactado que armazena os dados no padrão PCM (Pulse Code Modulation).

O UDA1334 recebe esses dados digitais enviados pelo ESP32 e os converte em sinais analógicos, que são representações contínuas das ondas sonoras. Diferente do áudio digital, que processa o som em valores discretos (bits), o sinal analógico corresponde às oscilações da pressão do ar, tornando o som audível para o ser humano. Como alto-falantes e fones de ouvido operam exclusivamente com sinais analógicos, eles convertem essas variações elétricas em ondas sonoras e reproduzem o áudio para esses dispositivos através de uma saída P2.

Por fim, todo esse sistema foi soldado em uma PCB central de 10 por 10 cm, junto com um módulo regulador de tensão de 5 V para protoboard e um cabo de alimentação de 9 V, responsável por fornecer energia ao sistema. Tudo isso foi colocado em uma estrutura quadrada de madeira, com dimensões de 20,2 x 20,2 x 14 cm.

O esquema elétrico, desenvolvido por meio do software EasyEDA, pode ser visualizado na Figura 5.

> <img src="https://github.com/IsaBellaBortoleto/AirDrums/blob/main/Diagrama_eletrico/Schematic_airdrums_2_2025-02-15%20(1).png"> <br/>
> Figura 5. Diagrama do Circuito Elétrico. Fonte: Autoria Própria.

---


Para mais informações, leia o [artigo disponível no repositório.](https://github.com/IsaBellaBortoleto/AirDrums/blob/main/Artigo.pdf)


<p float="left">
  <img src="https://github.com/IsaBellaBortoleto/AirDrums/blob/main/Fotos/WhatsApp Image 2025-02-23 at 17.46.06 (1).jpeg" height="340" width="255" />
  <img src="https://github.com/IsaBellaBortoleto/AirDrums/blob/main/Fotos/WhatsApp Image 2025-02-23 at 17.46.50 (2).jpeg" height="340" width="255" /> 
  <img src="https://github.com/IsaBellaBortoleto/AirDrums/blob/main//Fotos/lenalee.jpeg" height="340" width="255" />
</p>
