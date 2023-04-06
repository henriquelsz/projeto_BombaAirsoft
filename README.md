# Projeto BombaAirsoft

## Este repositório é sobre um simulador de bomba para ser utilizado em jogos de Airsoft com intuito recreativo.

### Funcionamento:
A bomba é acionada, o cronometro da bomba é disparado, o participante terá até fim do tempo do cronometro para acertar uma senha de 4 dígitos para desarmá-la, caso contrário, um buzzer é ativado – simulando a explosão da bomba. O cronometro será mostrado por um DISPLAY e a senha poderá ser digitada via um teclado matricial 4x4.

#### Exemplo de caso de uso:
Realizar uma competição para um time B plantar a bomba e o time A tentar desarmá-la. 
O time A irá digitar uma senha de 4 dígitos, se for a correta o LED verde será aceso, indicando a vitória do time A.
Se a senha estiver incorreta, o LED Vermelho será aceso e um Buzzer ativado, indicando a explosão da bomba

### Componentes:
- Arduino Mega 2560;
- Display LCD 20x4;
- Teclado Matricial 4x4;
- Buzzer;
- Placa Protoboard;
- LED`s;