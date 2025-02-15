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
	uint8_t complemento;
    uint8_t co;
    somador8bits(~num, 1, 0, &co, &complemento); // ~num + 1
    return complemento;

}

void ULA_ADD8B(int8_t *A, int8_t * B, int8_t * overflow){ 
	//Baseado no livro Willian Stallings 10ed, cap�tulo 10, pag. 287
	int8_t C1 = bit_get(*A, 8);
	int8_t C2 = bit_get(*B, 8);
	while (*B != 0) {
        int8_t carry = *A & *B;  // Calcula o vai um
        *A = *A ^ *B;          // Soma sem considerar o carry
        *B = carry << 1;     // Move o carry para a esquerda
    }
    int8_t C3 = bit_get(*A, 8);
	
	if(C1==C2 && C3!=C1){
		*overflow=1;
	}


}


void ULA_ADD16B(int16_t *A, int16_t * B, int8_t * overflow){ 
	//Baseado no livro Willian Stallings 10ed, cap�tulo 10, pag. 287
	int16_t C1 = bit_get(*A, 16);
	int16_t C2 = bit_get(*B, 16);
	while (*B != 0) {
        int16_t carry = *A & *B;  // Calcula o vai um
        *A = *A ^ *B;          // Soma sem considerar o carry
        *B = carry << 1;     // Move o carry para a esquerda
    }
    int16_t C3 = bit_get(*A, 16);
	
	if(C1==C2 && C3!=C1){
		*overflow=1;
	}


}







void ULA_SUB(int8_t *A, int8_t * B, int8_t * overflow){ 
	
	
	*B=completo2(*B);
	while (*B != 0) {
        int8_t carry = *A & *B;  // Calcula o vai um
        *A = *A ^ *B;          // Soma sem considerar o carry
        *B = carry << 1;     // Move o carry para a esquerda
    }
    


}












// void ULA_MUL(int8_t * A, int8_t *Q, int8_t * M,  int8_t * overflow){
// 	//Baseado no livro Willian Stallings 10ed, cap�tulo 10, pag. 293	
//     int16_t product = 0; // Inicializa o produto
//     int16_t mcand = M;
//     int8_t contador = 8; // Contador para 8 bits
    
    
    
//     while (contador > 0) {
//         int8_t um=1;
//         if (*Q & 1) { // Se o bit menos significativo do multiplicador for 1
//             int16_t multiplicadoq1we2d=mcand;
//             ULA_ADD16B(&product,&multiplicadoq1we2d,&overflow); // Soma o multiplicando ao produto
//         }

//         mcand <<= 1; // Desloca o multiplicando para a esquerda
//         *Q >>= 1;   // Desloca o multiplicador para a direita
        
//         ULA_SUB(&count,&um,&overflow);
//     }

//     return product;
// }







void binaryMultiplication( int8_t high, int8_tlow, int8_t multiplicand) {
    int16_t product = 0; // Inicializa o produto
    int16_t mcand =low; // Armazena o multiplicando como int16_t para evitar estouro de bits
    int8_t count = 8; // Contador para 8 bits

    while (count > 0) {
        if (*multiplicand & 1) { // Se o bit menos significativo do multiplicador for 1
            product += mcand; // Soma o multiplicando ao produto
        }

        mcand <<= 1;  // Desloca o multiplicando para a esquerda
        *multiplicand >>= 1;  // Desloca o multiplicador para a direita (mantendo sinal)
        count--;
    }

    *high = (product >> 8) & 0xFF; // Parte alta do resultado
    *low = product & 0xFF;         // Parte baixa do resultado

}



int main(){
    int8_t overflow=0;
	int8_t high;
    int8_t num1,num2;
    int16_t multi;
	
	//numero_invertido = completo2(num);
    //printf("Numero: %d\n",numero_invertido);
	num1=100;
	num2=203;
	ULA_ADD16B(&num1,&num2,&overflow);
	printf("Soma: %d  overflow: %d\n",num1,overflow);
	
	
	
	num1=120;
	num2=120;
	ULA_SUB(&num1,&num2,&overflow);
	printf("Subt: %d\n",num1);

	num1=120;
	num2=120;
	binaryMultiplication(&high,&num1,&num2,&overflow);
	//printf("Multiplicao: %d",multi);
    printf("\tMUL: REG A: %d\tREG Q:%d\tAQ: %d\tOverflow: %d\t",high, num1,(int16_t) (((high & 0x00FF) << 8) | (num1 & 0x00FF)),overflow);




}


