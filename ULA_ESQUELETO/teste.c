#include <stdio.h>
#include <stdint.h>
#include "ULA.h"

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

void complementador(uint8_t *num){
	uint8_t complemento;
    uint8_t co;
    somador8bits(~*num, 1, 0, &co, &complemento); // ~num + 1
    *num = complemento;
}

void ULA_ADD(int8_t *A, int8_t * B, int8_t * overflow){ 
	//Baseado no livro Willian Stallings 10ed, cap�tulo 10, pag. 287
	int8_t C1 = bit_get(*A, 8);
	int8_t C2 = bit_get(*B, 8);
	while (*B != 0) {
        int8_t carry = *A & *B;  
        *A = *A ^ *B;         
        *B = carry << 1;     
    }
    int8_t C3 = bit_get(*A, 8);
	
	if(C1==C2 && C3!=C1){
		*overflow=1;
	} else{
		*overflow=0;
	}


}

void ULA_ADD16B(int16_t *A, int16_t * B, int8_t * overflow){ 
	//Baseado no livro Willian Stallings 10ed, cap�tulo 10, pag. 287
	int16_t C1 = bit_get(*A, 16);
	int16_t C2 = bit_get(*B, 16);
	while (*B != 0) {
        int16_t carry = *A & *B;  
        *A = *A ^ *B;          
        *B = carry << 1;     
    }
    int16_t C3 = bit_get(*A, 16);
	
	if(C1==C2 && C3!=C1){
		*overflow=1;
	}


}

void ULA_SUB(int8_t *A, int8_t * B, int8_t * overflow){ 
	
	
	complementador(&*B);
	int8_t C1 = bit_get(*A, 8);
	int8_t C2 = bit_get(*B, 8);
	while (*B != 0) {
        int8_t carry = *A & *B;
        *A = *A ^ *B;          
        *B = carry << 1;     
    }
    int8_t C3 = bit_get(*A, 8);
	
	if(C1==C2 && C3!=C1){
		*overflow=1;
	} else{
		*overflow=0;
	}



}

void ULA_MUL(int8_t *A, int8_t *Q, int8_t *M, int8_t *overflow) {
	int16_t produto = 0, inv_produto;
	int16_t multiplicando = (int16_t)(*Q); 
	int16_t multiplicador = (int16_t)(*M); 
	int8_t count = 8;
	int16_t add;
	int8_t um;

	int8_t sign_q = (*Q < 0) ? 1 : 0;
	int8_t sign_m = (*M < 0) ? 1 : 0;

	if (*Q < 0) complementador(&*Q);
	if (*M < 0) complementador(&*M);

	multiplicando = *Q; 
	multiplicador = *M;  

	while (count > 0) {
		if (multiplicador & 1) {
			add = multiplicando;
			ULA_ADD16B(&produto, &add, overflow); 
		}

		multiplicando <<= 1; 
		multiplicador >>= 1;
		um = 1;
		ULA_SUB(&count, &um, overflow); 
	}

	if ((sign_q ^ sign_m)) {

		um = 1;
		inv_produto = ~produto;
		ULA_ADD16B(&inv_produto,&um,overflow);  
		produto = inv_produto;
	}

	*A = (int8_t)((produto >> 8) & 0xFF);
	*Q = (int8_t)(produto & 0xFF);         

	// Verificação de overflow
	if(((sign_m == sign_q)&&(produto<0))||((sign_m != sign_q)&&(produto>0))){
		*overflow = 1;
	} else{
		*overflow = 0;
	}
}
	
void ULA_DIV(int8_t *A, int8_t *Q, int8_t *M, int8_t *overflow) {
	*overflow = 0;
	if (*M == 0) {
		*overflow = 1;
		*Q = 0;
		*A = 0;
		return;
	}

	int8_t signA = (*A < 0) ? -1 : 1;
	int8_t signM = (*M < 0) ? -1 : 1;

	if(*A < 0) complementador(&*A);
	if(*M < 0) complementador(&*M);
	int8_t A_temp = *A;
	int8_t M_temp = *M;
	
	int8_t Q_temp = 0;
	int8_t R_temp = 0;

	for (int i = 0; i < 8; i++) {
		R_temp = (R_temp << 1) | ((A_temp >> (7 - i)) & 1);

		if (R_temp >= M_temp) {
			int8_t temp = M_temp;
			ULA_SUB(&R_temp, &temp, overflow);
			Q_temp |= (1 << (7 - i));
		}
	}

	if (signA != signM) {
		complementador(&Q_temp);
	}
	if (signA < 0) {
		complementador(&R_temp);
	}

	*A = Q_temp;
	*Q = R_temp;
}

int main(){
    int8_t overflow=0;
	int8_t high;
    int8_t num1,num2;
	
	//numero_invertido = complementador(num);
    //printf("Numero: %d\n",numero_invertido);
	num1=10;
	num2=23;
	ULA_ADD(&num1,&num2,&overflow);
	printf("Soma: %d  overflow: %d\n",num1,overflow);
	
	
	
	num1=120;
	num2=10;
	ULA_SUB(&num1,&num2,&overflow);
	printf("Subt: %d\n",num1);

	num1=-120;
	num2=120;
	ULA_MUL(&high,&num1,&num2,&overflow);
	//printf("Multiplicao: %d",multi);
    printf("\tMUL: REG A: %d\tREG Q:%d\tAQ: %d\tOverflow: %d\t",high, num1,(int16_t) (((high & 0x00FF) << 8) | (num1 & 0x00FF)),overflow);

	int8_t A = 7;
    int8_t Q = 0;
    int8_t M = 3;

	ULA_DIV(&A, &Q, &M, &overflow);
    printf("\nResultado: Quociente = %d, Resto = %d, Overflow = %d\n", Q, A, overflow);



}