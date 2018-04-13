#include <18F4620.h>
#fuses HS, NOFCMEN, NOIESO, PUT, NOBROWNOUT, NOWDT
#fuses NOPBADEN, NOMCLR, STVREN, NOLVP, NODEBUG
#use delay(clock=16000000)
#define __OPERACION_SUMA__ 1
#define __OPERACION_RESTA__ 1
#define __OPERACION_MULTIPLICAR__ 1
#define __OPERACION_DIVIDIR__ 1
#define _SUMA_   PIN_B4
#define _RESTA_   PIN_B5
#define _MULTIPLICAR_   PIN_B6
#define _DIVIDIR_   PIN_B7
#define __VALOR_DE_ERROR__ 0X1FFF
#define __DEBUG_SERIAL__

#ifdef __DEBUG_SERIAL__
   #define TX_232        PIN_C6
   #use RS232(BAUD=9600, XMIT=TX_232, BITS=8,PARITY=N, STOP=1)
   #use fast_io(c)
#endif
void recorrido(long resultadoFinal){
    output_a(resultadoFinal);
    output_b(resultadoFinal>>6);
    output_e(resultadoFinal>>10);
}
void comprobacionDeInterruptor(int8 interruptores){
   if(input(_SUMA_)==1)
     interruptores=__OPERACION_SUMA__;
   if(input(_RESTA_)==1)
     interruptores= __OPERACION_RESTA__;
   if(input(_MULTIPLICAR_)==1)
     interruptores= __OPERACION_MULTIPLICAR__;
   if(input(_DIVIDIR_)==1)
     interruptores= __OPERACION_DIVIDIR__;
}
void main (void){
   setup_oscillator(OSC_16MHZ);  
   set_tris_c(0xFF);
   set_tris_d(0xFF);   
   set_tris_b(0xF0);
   set_tris_a(0xC0);
   set_tris_e(0x08);   
   int8 numeroEnC = input_c(), numeroEnD = input_d(), interruptor;
   int16 resultadoOperacion;
/*#ifdef __DEBUG_SERIAL__ 
   printf("Hola Mundo\n");
#endif*/
   while(1){    
    numeroEnC = input_c();
    numeroEnD = input_d();    
   comprobacionDeInterruptor(interruptor);    
   switch (interruptor){
      case 1:
      resultadoOperacion = (int16)numeroEnC + (int16)numeroEnD;
      recorrido(resultadoOperacion);
      interruptor = 0;
      break;
      case 2:
         if(numeroEnD == 0 && numeroEnC == 0){
         resultadoOperacion = __VALOR_DE_ERROR__;
         recorrido(resultadoOperacion);
         interruptor = 0;
         }
         else{
          if(numeroEnD <= numeroEnC){
          resultadoOperacion = (int16)numeroEnC - (int16)numeroEnD;
          recorrido(resultadoOperacion);
          interruptor = 0;
          }
          else{
          resultadoOperacion = (~(int16)numeroEnD)+1;
          resultadoOperacion = resultadoOperacion - (int16)numeroEnD;
          recorrido(resultadoOperacion);
          interruptor = 0;
          }
         }
      break;
      case 3:
      resultadoOperacion = (int16)numeroEnC * (int16)numeroEnD;
      recorrido(resultadoOperacion);
      interruptor = 0;
      break;
      case 4:
      if(numeroEnD == 0 && numeroEnC == 0){
         resultadoOperacion = __VALOR_DE_ERROR__;
         recorrido(resultadoOperacion);
         }
         else{
          if(numeroEnC > numeroEnC){
          resultadoOperacion = (int16)numeroEnC / (int16)numeroEnD;
          if(resultadoOperacion > __VALOR_DE_ERROR__ ){
             resultadoOperacion = __VALOR_DE_ERROR__;          
             recorrido(resultadoOperacion);
             interruptor = 0;
          }
          else
             recorrido(resultadoOperacion);
             interruptor = 0;
          }
         }
      break;
      }
   }
}
