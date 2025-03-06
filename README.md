## ~ AIRDRUMS ~
<img src="https://github.com/IsaBellaBortoleto/AirDrums/tree/main/Fotos/FotoPrincipal">

Como projeto final da disciplina de Oficina de Integração I foi realizado o AirDrums, um sistema que utiliza sensores de movimento e tecnologia digital para simular a experiência de tocar bateria sem a necessidade de um instrumento físico volumoso.
A proposta do projeto é oferecer uma alternativa compacta, permitindo a pratica sem restrições de espaçoou ruído.

<h3>3.2 Componentes Utilizados</h3>

<ul>
    <li><strong>ESP32 Dev Kit V1:</strong> Microcontrolador principal do projeto, responsável por processar os dados dos sensores, interpretar os movimentos e gerenciar a reprodução dos sons da bateria virtual. São usados dois ESP32 para capturar os dados das baquetas e um ESP32 central para processar os sinais e executar o áudio.</li>

    <li><strong>IMU MPU6050 (Inertial Measurement Unit - Unidade de Medição Inercial):</strong> Acelerômetro e giroscópio integrados às baquetas, detectam os movimentos do usuário e identificam a posição das batidas na bateria virtual.</li>

    <li><strong>DAC UDA1334A (Digital-to-Analog Converter - Conversor Digital-para-Analógico):</strong> Converte os sinais digitais de áudio do ESP32 central em sinais analógicos, permitindo a reprodução dos sons por alto-falantes ou fones de ouvido.</li>

    <li><strong>MicroSD:</strong> Armazena os arquivos de áudio das peças da bateria (tons, pratos e bumbo) e fornece os dados para reprodução conforme os comandos recebidos.</li>

    <li><strong>Botão Push-Button:</strong> Aciona manualmente o som do bumbo, simulando o pedal tradicional da bateria.</li>

    <li><strong>Fonte de Alimentação para protoboard 3,3/5 V:</strong> Fornece energia para todos os módulos eletrônicos do sistema.</li>
</ul>

<p>Além desses componentes, foram utilizadas:</p>

<ul>
    <li><strong>Placas universais (PCBs):</strong> Para integração elétrica das baquetas e do ESP32 central.</li>
    <li><strong>Estrutura de madeira:</strong> Projetada para acomodar os componentes e garantir segurança.</li>
    <li><strong>Cabos Manga:</strong> Utilizados para comunicação e alimentação das baquetas virtuais.</li>
    <li><strong>Tubos de PVC:</strong> Para proteger e dar acabamento às baquetas.</li>
    <li><strong>Resistor de 10kΩ:</strong> Para limitar a corrente no circuito do botão.</li>
    <li><strong>Massa para madeira:</strong> Utilizada para aprimorar o acabamento da estrutura externa.</li>
    <li><strong>Jumpers:</strong> Para conectar os diversos módulos do circuito.</li>
</ul>

