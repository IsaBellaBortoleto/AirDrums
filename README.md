## ~ AIRDRUMS ~
<img src="https://github.com/IsaBellaBortoleto/AirDrums/blob/main/Fotos/FotoPrincipal.jpeg">

Como projeto final da disciplina de Oficina de Integração I foi realizado o AirDrums, um sistema que utiliza sensores de movimento e tecnologia digital para simular a experiência de tocar bateria sem a necessidade de um instrumento físico volumoso.
A proposta do projeto é oferecer uma alternativa compacta, permitindo a prática sem restrições de espaço ou ruído.

## Desenvolvedores  

- Bruna Medeiros Teixeira  
- Isabela Bella Bortoleto  
- Guilherme de Souza Carneiro Garcia  


## 3.2 Componentes Utilizados  

- **ESP32 Dev Kit V1**: Microcontrolador principal do projeto, responsável por processar os dados dos sensores, interpretar os movimentos e gerenciar a reprodução dos sons da bateria virtual. Nesse contexto, dois ESP32 são usados para capturar os dados das baquetas e um ESP32 que processa os sinais e executa o áudio. A partir de agora, ele será chamado de ESP32 central.  

- **IMU MPU6050 (Inertial Measurement Unit - Unidade de Medição Inercial)**: Unidade de acelerômetro e giroscópio integrado às baquetas, detecta os movimentos do usuário, identificando a posição das batidas na bateria virtual.  

- **DAC UDA1334A (Digital-to-Analog Converter - Conversor Digital-para-Analógico)**: Converte os sinais digitais de áudio do ESP32 central em sinais analógicos, permitindo a reprodução dos sons da bateria através de alto-falantes ou fones de ouvido.  

- **MicroSD**: Armazena os arquivos de áudio das peças da bateria (tons, pratos e bumbo) e fornece os dados para reprodução conforme os comandos recebidos.  

- **Botão Push-Button**: Botão de ativação, aciona manualmente o som do bumbo, simulando o pedal tradicional da bateria.  

- **Fonte de Alimentação para protoboard 3,3/5 V**: Responsável por fornecer energia para todos os módulos eletrônicos do sistema.  

Além deles, foram utilizadas placas universais (PCBs) para a integração elétrica das baquetas e do ESP32 central, uma estrutura de madeira projetada para acomodar os componentes e garantir sua segurança, cabos Manga para comunicação e alimentação das baquetas virtuais e uma estrutura de tubos de PVC para proteger e realizar o acabamento das baquetas, um resistor de 10kΩ para limitar a corrente no circuito do botão e massa para madeira para aperfeiçoar o acabamento da estrutura externa. Também foram utilizados jumpers para conectar os diversos módulos do circuito.

### 3.4 Desenvolvimento  

Os tópicos do desenvolvimento seguirão o fluxo de informações enviadas pelos componentes da bateria: ao serem acionadas, as baquetas ou o bumbo enviam os dados para o ESP32 central, que então processa e reproduz o som correspondente.  

A **Figura 2** apresenta uma representação geral do projeto.  

<img src="https://github.com/IsaBellaBortoleto/AirDrums/blob/main/Vetores/diagrama.png">



### Funcionamento das Baquetas  

O funcionamento das baquetas é baseado na integração de um **ESP32** e um **módulo MPU-6050** em cada baqueta, como mostra a **Figura 2**. Ao ligar o aparelho, o módulo realiza uma calibração inicial e começa a captar os giros de dois eixos principais:  

- O giro no eixo **Y** é usado para captar os movimentos de batida.  
- O giro no eixo **Z** é utilizado para determinar a região das partes da bateria.  

No código das baquetas, foi utilizada a biblioteca do módulo desenvolvida **Electronic Cats (Cats, 2025)**, que possibilita a ativação do **Digital Motion Processor (DMP)**.  

O **DMP** é um circuito integrado ao **MPU-6050**, que tem como objetivo refinar a carga do processamento das informações fornecidas pelo módulo, realizando cálculos complexos e aplicando filtros nos dados brutos captados pelo sensor. Esse processamento é realizado por um código proprietário, desenvolvido pela fabricante do módulo, a **InvenSense**, de código fechado.  

Como resultado, ele entrega informações mais elaboradas, na forma dos ângulos **Yaw** (Guinada, rotação no eixo Z), **Pitch** (Arfagem, rotação no eixo X) e **Roll** (Rolamento, rotação no eixo Y), como são chamados após a aplicação dos filtros internos, processados diretamente pelo DMP.  

Com isso, definimos zonas de ativação para cada parte da bateria através das baquetas, utilizando a posição relativa ao eixo **Z (Yaw)**. A **Figura 4** descreve os pontos de ativação de cada componente da bateria, assim como os ângulos de ativação.  

As zonas foram assim determinadas para respeitar as partes naturais da bateria, garantir precisão e fácil utilização do aparelho, sendo que as **faixas neutras** não correspondem a nenhum som e foram definidas para evitar conflito entre os pratos e tons.  

