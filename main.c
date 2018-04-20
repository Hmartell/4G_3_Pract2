#include <18F4620.h>
#fuses HS, NOFCMEN, NOIESO, PUT, NOBROWNOUT, NOWDT
#fuses NOPBADEN, NOMCLR, STVREN, NOLVP, NODEBUG
#use delay(clock=16000000)
#use fast_io(a)
#use fast_io(b)
#use fast_io(c)
#use fast_io(d)
#use fast_io(e)
#define __DEBUG_SERIAL__

#ifdef __DEBUG_SERIAL__
   #define TX_232        PIN_C6
   #use RS232(BAUD=9600, XMIT=TX_232, BITS=8,PARITY=N, STOP=1)
   #use fast_io(c)
#endif

int comprobacionDeInterruptor(){
   int interruptores;
   if(input(pin_b4)==1)
      interruptores = 1;
   if(input(pin_b5)==1)
      interruptores = 2;
   if(input(pin_b6)==1)
      interruptores = 3;
   if(input(pin_b7)==1)
      interruptores = 4;     
   return interruptores;
}
void main (void){
   setup_oscillator(OSC_16MHZ);  
   set_tris_c(0xFF);
   set_tris_d(0xFF);   
   set_tris_b(0xF0);
   set_tris_a(0xC0);
   set_tris_e(0x08);   
   long numeroEnC;
   long numeroEnD;
   int interruptor;
   long resultadoOperacion;

   while(1){    
   numeroEnC = (long)input_c();
   numeroEnD = (long)input_d();    
   interruptor = comprobacionDeInterruptor();    
   switch (interruptor){
      case 1:
         resultadoOperacion = numeroEnC + numeroEnD;
         output_a(resultadoOperacion);
         output_b(resultadoOperacion>>6);
         output_e(resultadoOperacion>>10);
         interruptor = 0;
      break;
      case 2:
         resultadoOperacion = numeroEnC - numeroEnD;
         output_a(resultadoOperacion);
         output_b(resultadoOperacion>>6);
         output_e(resultadoOperacion>>10);
         interruptor = 0;
      break;
      case 3:
         resultadoOperacion = numeroEnC * numeroEnD;
         output_a(resultadoOperacion);
         output_b(resultadoOperacion>>6);
         output_e(resultadoOperacion>>10);
         interruptor = 0;
      break;
      case 4:
         if(numeroEnC == 0 && numeroEnD == 0){
         resultadoOperacion = 0x1FFF;
         output_a(resultadoOperacion);
         output_b(resultadoOperacion>>6);
         output_e(resultadoOperacion>>10);
         delay_ms(200);
         }
         else{
         if(numeroEnD == 0){
         resultadoOperacion = 0x1FFF;
         output_a(resultadoOperacion);
         output_b(resultadoOperacion>>6);
         output_e(resultadoOperacion>>10);
         delay_ms(200);
         }
         else{
         resultadoOperacion = numeroEnC / numeroEnD;
         output_a(resultadoOperacion);
         output_b(resultadoOperacion>>6);
         output_e(resultadoOperacion>>10);
         interruptor = 0;
         }
         }
      break;
      }
   }
}
