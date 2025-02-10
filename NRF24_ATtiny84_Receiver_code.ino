#include <nRF24L01.h>
#include <RF24.h>

#define pin_giro_izquierda 0                                  //luz de giro izquierda
#define pin_giro_derecha 1                                    //luz de giro derecha
#define pin_boton_variador 2                                    //luz superior intermitente
#define frecuencia_luz_intermitente 800                       //frecuencia de intermitencia de la luz superior
#define frecuencia_luz_de_giro 500                            //frecuencia de intermitencia en la luz de giro

byte lectura_emision,memoria_luz,contador;

RF24 radio(2, 3);                                             // definición de pines de CE, CSN  para el modulo NRF24L01
const byte direcciones[][6] = {"00001", "00002"};

void setup() 
{ 
  //Serial.begin(9600);                                         // No valido para attiny84/85, OJO!
  pinMode(pin_giro_derecha,OUTPUT);
  pinMode(pin_giro_izquierda,OUTPUT);  
  pinMode(pin_boton_variador,OUTPUT);  
  
  radio.begin();                                                // inicio la comunicación
  radio.openWritingPipe(direcciones[0]);                        // Habilito el canal de comunicación 00001 para escritura.
  radio.openReadingPipe(1, direcciones[1]);                     // Habilito el canal de comunicación 00002 para lectura.
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);  
}

void loop() {
  // lectura de información enviada desde el transmisor //
  //capaz que no hace falta, pero por las dudas... delay(10); // delay de 10 mS por las dudas
  radio.startListening();                                     // sentencio el inicio de la lectura RF
  if (radio.available())                                      // verifico si se tiene alguna informacion en la comunicaicon RF
      {                                    
      radio.read(lectura_emision, sizeof(lectura_emision));   // leo la info que pueda llegar a captar por RF
      
      if (lectura_emision == 1)                               //si desde el comando se manda la orden de dobla a la derecha
      {giro_derecha();}      

      if (lectura_emision == 2)                               //si desde el comando se manda la orden de dobla a la izquierda
      {giro_izquierda();}

      if (lectura_emision == 3)                               //el comando avisa que se apretó el boton variable
      {
       contador++;
       boton_variable();
      }
      else                                                    //se recibe 0
      {
      lectura_emision=0;                                      //si no recibo nada digo que la recepción que mandaré
      }                                                       //al emisor vale 0
      }
   radio.stopListening();                                     //dejo de escuchar y empiezo a transmitir la señal de confirmacion de recepcion
   radio.write(lectura_emision, sizeof(lectura_emision));     //con esto me comunico al transmisor      
}
