#include <nRF24L01.h>
#include <RF24.h>

#define boton_izquierda 0           //defino el puerto analógico del attiny84 para el giro a la izquierda
#define boton_derecha 1             //defino el puerto analógico del attiny84 para el giro a la derecha
#define boton_variacion 2           //defino el puerto analógico del attiny84 para generar la variacion de frecuencia titilado 
#define V_umbral 500                //defino la lectura analógica umbral (0-1023) sore el cual actua el boton
#define tiempo_anti_rebote 500      //defino el tiempo (ms) de antirebote para que no haya superposicion de sentencias

uint32_t receiver[1];
byte emision,recepcion,habilitar_emision,lectura_de_confirmacion;

RF24 radio(2, 3);                                   // defino los pines CE, CSN del módulo
const byte direcciones[][6] = {"00001", "00002"};   //direcciones de comunicación

void setup() 
  {
  radio.begin();                                    //doy comienzo a la comunicación por RF
  radio.openReadingPipe(1, direcciones[0]);         //habilitando comunicación por vía 00001 para lectura
  radio.openWritingPipe(direcciones[1]);            //habilitando comunicación por vía 00002 para escritura
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS); 
  }
  
void loop() 
 { 
  //lectura de la señal de radiofrecuencia  
  radio.startListening();      // sentencio el inicio de la lectura RF para poder saber si el mje llego o no
  if(radio.available())      //si recibo una señal habilito el loop 
  {
  radio.read(lectura_confirmacion, sizeof(lectura_confirmacion));

  if (lectura_de_confirmacion==1 || lectura_de_confirmacion==2 || lectura_de_confirmacion==3)            //si se recibe una señal de 1,2 o 3 significa que receptor recibio alguna de las sentencias
  {                                                                                                      //entonces no sigo emitiendo señal
   habilitar_emision=0;                                                                                  //deshabilito la emision de informacion por radiofrecuencia
  }
  }
  
  else
  {
  habilitar_emision=1;         //habilito la emision de informacion por parte del módulo de rádio frecuencia emisor ya que no se recibe informacion o se recibe un 0
  }
  
  radio.stopListening();       // dejo de recibir informacion desde la comunicación RF 
  
  //lectura de entrada analogicas para setear las luces de giro, siempre que la emision de informacion este disponible
  if(habilitar_emision==1)                                                    //si NO se recibió confirmación de recepcion de 1 2 o 3
  { 
  if (analogRead(boton_izquierda)<V_umbral) //boton entrada giro a la derecha
  {
  emision=1;
  }
  else if (analogRead(boton_derecha)<V_umbral) //boton entrada giro a la izquierda
  {
  emision=2;
  }  
  else if (analogRead(boton_variacion)<V_umbral) //boton entrada para alternar luces
  {
  emision=3;
  } 
  else
  {
  emision=0;  //si no se detecta ninguna señal analógica 
  }
  
  // puede ser que no haga falta, pero por caulq cosa se puede usar...   delay(10); // doy un delay de 10 mS por las dudas
  radio.write(emision, sizeof(emision));                   // envio info dependiendo de q boton se apriete o envio 0 si es que 
  }                                                        //se recibió una confirmación por parte del receptor
  else
  {
  delay(tiempo_anti_rebote);
  emision=0;  
  radio.write(emision, sizeof(emision));
  }
  
  }
