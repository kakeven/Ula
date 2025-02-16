#include "ULA.h"

//Somador Baseado no livro Willian Stallings 10ed, cap�tulo 10, pag. 334
static void somadorCompleto (uint8_t A, uint8_t B, uint8_t Cin, uint8_t * co, uint8_t * s){
	*co = (A&B) | (A&Cin) | (B&Cin);
	//*s = A ^ B ^ Cin;
	*s = (~A & ~B & Cin) | (~A & B & ~Cin) | (A & B & Cin) | (A & ~B & ~Cin);	
}

//O somador bin�rio da ULA trata os dois n�meros como inteiros sem sinal.
//Entradas: Registrados A, B e cin(CarryIn), Sa�da: s(soma) e co(CarryOut)
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

//Realiza o complemento de 2 do Registrador A e grava em A
static void complementador(uint8_t *A){
	//Baseado no livro Willian Stallings 10ed, cap�tulo 10, pag. 285
	uint8_t complemento;
    uint8_t co;
    somador8bits(~*A, 1, 0, &co, &complemento); 
    *A = complemento;
}

//Adi��o de A e B e grava resultado em A
void ULA_ADD(int8_t *A, int8_t * B, int8_t * overflow){ 
	///Baseado no livro Willian Stallings 10ed, cap�tulo 10, pag. 287
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

//Subtra��o de A e B e grava resultado em A
void ULA_SUB(int8_t *A, int8_t * B, int8_t * overflow){ 
	//Baseado no livro Willian Stallings 10ed, cap�tulo 10, pag. 287
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

//Multiplica��o de Q(8bits) com M(8bits) gera resultado de 16bits que est� em A(8bits) e Q(8bits)
void ULA_MUL(int8_t * A, int8_t *Q, int8_t * M,  int8_t * overflow){
	//Baseado no livro Willian Stallings 10ed, cap�tulo 10, pag. 293	
	int16_t produto = 0, inv_produto, um2;
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

		um2 = 1;
		inv_produto = ~produto;
		ULA_ADD16B(&inv_produto,&um2,overflow);  
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

//Divis�o com sinal de Q(Dividendo de 8bits) por M(Divisor de 8bits) com Quociente em Q(8bits) e Resto em A(8bits)
void ULA_DIV(int8_t *A, int8_t *Q, int8_t *M, int8_t *overflow) {
	*overflow = 0;
		if (*M == 0) {
			*overflow = 1;
			*Q = 0;
			*A = 0;
			return;
		}
	
		int8_t signQ = (*Q < 0) ? -1 : 1;
		int8_t signM = (*M < 0) ? -1 : 1;
	
		if(*Q < 0) complementador(&*Q);
		if(*M < 0) complementador(&*M);
		int8_t Q_temp = *Q;
		int8_t M_temp = *M;
		
		int8_t A_temp = 0;
		int8_t R_temp = 0;
	
		for (int i = 0; i < 8; i++) {
			R_temp = (R_temp << 1) | ((Q_temp >> (7 - i)) & 1);
	
			if (R_temp >= M_temp) {
				int8_t temp = M_temp;
				ULA_SUB(&R_temp, &temp, overflow);
				A_temp |= (1 << (7 - i));
			}
		}
	
		if (signQ != signM) {
			complementador(&A_temp);
		}
		if (signQ < 0) {
			complementador(&R_temp);
		}
	
		*Q = A_temp;
		*A = R_temp;

		if(((signQ == signM)&&(*Q<0))||((signQ != signM)&&(*Q>0))){
			*overflow = 1;
		} else{
			*overflow = 0;
		}
}


//Descomente as fun��es caso deseja implementar as fun��es de Ponto Flutuante
//Baseado no livro Willian Stallings 10ed, cap�tulo 10, pag. 298
//void ULA_ADD_PF(float *A, float * B, int8_t * overflow, int8_t * underflow){}
//void ULA_SUB_PF(float *A, float * B, int8_t * overflow, int8_t * underflow){}
//void ULA_MUL_PF(float *A, float * B, int8_t * overflow, int8_t * underflow){}
//void ULA_DIV_PF(float *A, float * B, int8_t * overflow, int8_t * underflow){}
