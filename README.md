# IOInterface

<details>
<summary>Texto do Problema</summary>

---

## Tema

Projeto de sensor analógico/digital em microcontrolador utilizando comunicação serial.

## Objetivos de Aprendizagem

Ao final da realização deste problema, o/a discente deverá ser capaz de:

- Entender como integrar código assembly e códigos C para produzir um programa executável;
- Compreender e executar a programação de dispositivos microcontroladores;
- Assimilar conceitos básicos sobre protocolos de comunicação serial.

## Problema
Dando prosseguimento ao desenvolvimento do protótipo de um sistema digital baseado em um processador ARM, o próximo passo compreende a implementação de um protótipo de sistema de sensoriamento genérico. Na fase de protótipo do projeto será utilizada uma plataforma baseada na NodeMCU para confecção das unidades de sensoriamento. Elas são muito flexíveis e versáteis, sendo ideais para a criação de um ecossistema de Internet das Coisas (IoT). Para simplificar a prova de  conceito será utilizado um sensor analógico e dois sensores digitais, mas o sistema deve ser modular, permitindo a substituição na versão de produção.

O sistema será comandado por um Single Board Computer (SBC), e deve ser capaz de controlar o acionamento de um conjunto variável de sensores, assim como monitorar o seu funcionamento, de forma automatizada. Cada operação de leitura ou monitoramento deve ser representada por um código. Dessa forma, o sistema embarcado na NodeMCU deve ser capaz de interpretá-los e realizá-los de maneira adequada, por meio de uma comunicação UART.

---

</details>

## Autores
<div align="justify">
    <li><a href="https://github.com/ozenilsoncruz">@Ozenilson Cruz</a></li>  <li><a href="https://github.com/traozin">@Antônio Neto</a></li>
</div>

## UART [^rohde-uart] [^freebsd-uart]

O UART(Transmissor/receptor assíncrono universal) é um protocolo de transmissão de dados muito simples, onde só é necessário apenas dois fios para a comunicação entre suas extremidades em ambas as direções. Além disso, o UART é um dos padrões seriais mais antigos do mundo, onde acabou sendo amplamente utilizado em dispositivos que faziam uso de portas seriais.

Existem dois tipos de comunicação Serial, a Síncrona e a Assíncrona. 

A comunicação Síncrona requer que o emissor e o receptor dos dados tenham um clock em comum, ou no mínimo, o emissor forneça um sinal de tempo para que o receptor saiba quando se deve "ler" o próximo bit dos dados. Na maioria das formas de comunicação serial síncrona, se não houver dados disponíveis em um dado instante para transmitir, um caractere de preenchimento deve ser enviado para que os dados sejam sempre transmitidos.

Já a transmissão assíncrona permite que os dados sejam transmitidos sem que o emissor tenha que enviar um sinal de relógio ao receptor. Em vez disso, o remetente e o receptor devem concordar com os parâmetros de tempo de antecedência e bits especiais são adicionados a cada palavra, os quais são usados para sincronizar as unidades de envio e recebimento.

Como o próprio nome já diz, o UART é um protocolo de comunicação assíncrona, sendo assim, ambas as extremidades devem transmitir ao mesmo tempo e em velocidade predefinida para poder ter a mesma temporização de bits. As taxas de baud mais comuns utilizadas em UART atualmente são 4800, 9600, 19,2 K, 57,6 K e 115,2 K. Além de ter a mesma taxa de bauds, ambos os lados de uma conexão UART também têm que usar a mesma estrutura de frames e parâmetros.

<figure style="align:center">
  <img src="assets/frame-uart.png"/>
  <figcaption>Frame UART</figcaption>
</figure>


Frames UART contém bits iniciais e finais, bits de dados e um bit opcional de paridade.

##### Bits iniciais e finais

Devido ao UART ser assíncrono, o transmissor precisa sinalizar que os bits de dados estão chegando. Isso é possível ao utilizar o bit inicial. O bit inicial é uma transição do estado inativo para um estado baixo, imediatamente seguido pelos bits de dados do usuário.

Depois que os bits de dados tiverem terminado, o bit final indica o fim dos dados do usuário. O bit de parada é uma transição de volta para o estado alto ou inativo, ou a permanência no estado alto for um tempo de bit adicional.

##### Bit de paridade
 Um segundo bit final (opcional) pode ser configurado, geralmente para dar ao receptor tempo para se preparar para o próximo frame, mas essa é uma prática relativamente incomum.

##### Bits de dados

Os bits de dados são dados de usuário ou bits "úteis" e vêm imediatamente depois do bit inicial. Pode haver de 5 a 9 bits de dados de usuários, apesar de ser mais comum haver 7 ou 8 bits. Esses bits de dados geralmente são transmitidos com o bit menos significativo primeiro.

## Node MCU [^nodemcu]

<figure style="align:center">
  <img src="assets/pinagem-nodemcu.png" height="350em"/>
  <figcaption>Diagrama de Mapeamento de Pinos do ESP8266</figcaption>
</figure>

- Módulo NodeMcu Lua ESP-12E
- Versão do módulo: V2
- Memória flash: 4 MB
- Tensão de operação:
- Pinos Digitais: 3,3 V
- Pino Analógico: 1,0 V
- Wireless padrão 802.11 b/g/n
- Antena embutida
- Conector micro-usb para programação e alimentação
- Modos de operação: STA/AP/STA+AP
- Suporta 5 conexões TCP/IP
- Portas GPIO: 13
- D0 (GPIO16) só pode ser usado como entrada ou saída, não suporta outras funções (interrupção, PWM, I2C, etc)
- GPIO com funções de PWM, I2C, SPI, etc
- Resolução do PWM: 10 bits (valores de 0 a 1023)
- 01x Conversor analógico digital (ADC)
- Distância entre pinos: 2,54 mm
- Dimensões: 49 x 26 x 7 mm (sem considerar os pinos)

## Biblioteca Assembly em C

## Sensores Digitais vs Analógicos [^sensores]

O termo "sensor" em si significa um mecanismo projetado para medir um parâmetro, a fim de processar ainda mais o resultado da medição. O circuito do sensor gera um sinal de forma conveniente para a transmissão e, em seguida, o sinal é convertido, processado ou armazenado.

##### Analógico

Um sensor analógico gera um sinal analógico na saída, cujo valor de nível é obtido em função do tempo, e esse sinal muda continuamente, o sinal constantemente recebe qualquer um dos muitos valores possíveis.

Portanto, sensores analógicos são adequados para rastrear continuamente a grandeza física, por exemplo tensão terminal do termopar sinaliza uma mudança de temperatura e a tensão no enrolamento secundário do transformador de corrente é em um determinado período proporcional à corrente do circuito controlado. O microfone é um sensor de mudanças de pressão de uma onda sonora, etc.

##### Digital

Os sensores digitais, por sua vez, geram um sinal de saída que pode ser gravado na forma de uma sequência de valores numéricos, geralmente o sinal é binário, ou seja, um nível alto ou baixo (zero). Quando um sinal de sensor digital precisa ser transmitido por um canal analógico, como o rádio, recorra ao uso da modulação.

Os sensores digitais dominam os sistemas de comunicação porque seus sinais de saída são facilmente regenerados no repetidor, mesmo se houver ruído. E o sinal analógico, nesse sentido, será distorcido pelo ruído e os dados serão imprecisos. Ao transmitir informações, os sensores digitais são mais aceitáveis. Eles são adequados para detectar a presença ou ausência de um objeto, por exemplo.


[^rohde-uart]: Compreender UART - [rohde-schwarz.com](https://www.rohde-schwarz.com/br/produtos/teste-e-medicao/osciloscopios/educational-content/compreender-uart_254524.html)

[^freebsd-uart]: Tutorial sobre Comunicações Seriais e UART - [docs.freebsd.org](https://docs.freebsd.org/pt-br/articles/serial-uart/)

[^nodemcu]: NodeMCU ESP8266-12 V2 Especificações - [robocore.net](https://www.robocore.net/wifi/nodemcu-esp8266-12-v2)

[^sensores]: Qual é a diferença entre sensores analógicos e digitais - [i.electricianexp.com](https://i.electricianexp.com/pt/main/praktika/1185-analogovye-i-cifrovye-datchiki.html)
