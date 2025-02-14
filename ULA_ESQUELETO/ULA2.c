#include "ULA.h"

//Somador Baseado no livro Willian Stallings 10ed, cap�tulo 10, pag. 334
static void somadorCompleto (uint8_t A, uint8_t B, uint8_t Cin, uint8_t * co, uint8_t * s){
	*co = (A&B) | (A&Cin) | (B&Cin);
	//*s = A ^ B ^ Cin;
	*s = (~A & ~B & Cin) | (~A & B & ~Cin) | (A & B & Cin) | (A & ~B & ~Cin);	
}

//O somador bin�rio da ULA trata os dois números como inteiros sem sinal.
//Entradas: Registrados A, B e cin(CarryIn), Saída: s(soma) e co(CarryOut) 
static void somador8bits(uint8_t A, uint8_t B, uint8_t cin, uint8_t * co, uint8_t * s){
	*co = cin;
	uint8_t soma;
	for (int i=0;i<8;i++){
		somadorCompleto(bit_get(A,0),bit_get(B,0), *co, co,&soma);
		A = A>>1;//deslocando os bits para direita a cada intera��o
		B = B>>1;

		if(soma==1){
			bit_set(*s,i);
		}else{
			bit_clr(*s,i);
		}
		
	}
}

//Realiza o complemento de 2 do Registrador A e grava em A





static void complementador(int8_t *A){

	//somador8bits();
	//Baseado no livro Willian Stallings 10ed, cap�tulo 10, pag. 285
}

//Adi��o de A e B e grava resultado em A
void ULA_ADD(int8_t *A, int8_t * B, int8_t * overflow){ 
	//Baseado no livro Willian Stallings 10ed, cap�tulo 10, pag. 287
}

//Subtra��o de A e B e grava resultado em A
void ULA_SUB(int8_t *A, int8_t * B, int8_t * overflow){ 
	//Baseado no livro Willian Stallings 10ed, cap�tulo 10, pag. 287
}

//Multiplica��o de Q(8bits) com M(8bits) gera resultado de 16bits que est� em A(8bits) e Q(8bits)
void ULA_MUL(int8_t * A, int8_t *Q, int8_t * M,  int8_t * overflow){
	//Baseado no livro Willian Stallings 10ed, cap�tulo 10, pag. 293	
}

//Divis�o com sinal de Q(Dividendo de 8bits) por M(Divisor de 8bits) com Quociente em Q(8bits) e Resto em A(8bits)
void ULA_DIV(int8_t *A, int8_t *Q, int8_t * M, int8_t * overflow){
	//Baseado no livro Willian Stallings 10ed, cap�tulo 10, pag. 296	
}

//Descomente as fun��es caso deseja implementar as fun��es de Ponto Flutuante
//Baseado no livro Willian Stallings 10ed, cap�tulo 10, pag. 298
//void ULA_ADD_PF(float *A, float * B, int8_t * overflow, int8_t * underflow){}
//void ULA_SUB_PF(float *A, float * B, int8_t * overflow, int8_t * underflow){}
//void ULA_MUL_PF(float *A, float * B, int8_t * overflow, int8_t * underflow){}
//void ULA_DIV_PF(float *A, float * B, int8_t * overflow, int8_t * underflow){}

//teste
//AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
//CCCCCCCCCCCCCCCCCCCCCC algo
