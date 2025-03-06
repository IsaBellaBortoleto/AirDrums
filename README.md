## ~ AirDrums ~
<img src="https://github.com/IsaBellaBortoleto/AirDrums/tree/main/Fotos/FotoPrincipal.jpeg">

Como projeto final da disciplina de Oficina de Integração I foi realizado o AirDrums, um sistema que utiliza sensores de movimento e tecnologia digital para simular a experiência de tocar bateria sem a necessidade de um instrumento físico volumoso.
A proposta do projeto é oferecer uma alternativa compacta, permitindo a pratica sem restrições de espaçoou ruído.

## 3.2 Componentes Utilizados  

- **ESP32 Dev Kit V1**: Microcontrolador principal do projeto, responsável por processar os dados dos sensores, interpretar os movimentos e gerenciar a reprodução dos sons da bateria virtual. Nesse contexto, dois ESP32 são usados para capturar os dados das baquetas e um ESP32 que processa os sinais e executa o áudio. A partir de agora, ele será chamado de ESP32 central.  

- **IMU MPU6050 (Inertial Measurement Unit - Unidade de Medição Inercial)**: Unidade de acelerômetro e giroscópio integrado às baquetas, detecta os movimentos do usuário, identificando a posição das batidas na bateria virtual.  

- **DAC UDA1334A (Digital-to-Analog Converter - Conversor Digital-para-Analógico)**: Converte os sinais digitais de áudio do ESP32 central em sinais analógicos, permitindo a reprodução dos sons da bateria através de alto-falantes ou fones de ouvido.  

- **MicroSD**: Armazena os arquivos de áudio das peças da bateria (tons, pratos e bumbo) e fornece os dados para reprodução conforme os comandos recebidos.  

- **Botão Push-Button**: Botão de ativação, aciona manualmente o som do bumbo, simulando o pedal tradicional da bateria.  

- **Fonte de Alimentação para protoboard 3,3/5 V**: Responsável por fornecer energia para todos os módulos eletrônicos do sistema.  

Além deles, foram utilizadas placas universais (PCBs) para a integração elétrica das baquetas e do ESP32 central, uma estrutura de madeira projetada para acomodar os componentes e garantir sua segurança, cabos Manga para comunicação e alimentação das baquetas virtuais e uma estrutura de tubos de PVC para proteger e realizar o acabamento das baquetas, um resistor de 10kΩ para limitar a corrente no circuito do botão e massa para madeira para aperfeiçoar o acabamento da estrutura externa. Também foram utilizados jumpers para conectar os diversos módulos do circuito.

