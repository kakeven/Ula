#include <stdio.h>
#include <stdint.h>
#include "ULA.h"



/*int8_t soma(int8_t a, int8_t b) {
    while (b != 0) {
        int8_t carry = a & b;  // Calcula o "vai um"
        a = a ^ b;          // Soma sem considerar o carry
        b = carry << 1;     // Move o carry para a esquerda
    }
    return a;
}*/

static void somadorCompleto (uint8_t A, uint8_t B, uint8_t Cin, uint8_t * co, uint8_t * s){
	*co = (A&B) | (A&Cin) | (B&Cin);
	//*s = A ^ B ^ Cin;
	*s = (~A & ~B & Cin) | (~A & B & ~Cin) | (A & B & Cin) | (A & ~B & ~Cin);	
}

static void somador8bits(uint8_t A, uint8_t B, uint8_t cin, uint8_t * co, uint8_t * s){
	*co = cin;
	uint8_t soma;
	for (int i=0;i<8;i++){
		somadorCompleto(bit_get(A,0),bit_get(B,0), *co, co,&soma);
		A = A>>1;//deslocando os bits para direita a cada intera��o
		B = B>>1;
		if(soma==1)bit_set(*s,i);
		else bit_clr(*s,i);
	}
}




int8_t completo2(int8_t num){
	int8_t s;
	somador8bits(num,0,0,0,&s);   
    return s ;


}




int main(){
    int8_t num = 128;
    int8_t numero_invertido;
    
	
	numero_invertido = completo2(num);
    printf("Numero: %d",numero_invertido);

}

