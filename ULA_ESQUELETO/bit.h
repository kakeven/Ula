#ifndef  BIT_H
#define  BIT_H
/*N�o necess�rio editar*/
#define bit_get(reg, bit)  (((reg) & (1<<(bit)))>0) ? 1 : 0  //verifica qual valor do bit na posi��o desejada
#define bit_set(reg, bit)  ((reg) |= (1<<(bit)))           //Coloca o bit para 1 na posi��o desejada
#define bit_clr(reg, bit)  ((reg) &= ~(1<<(bit)))          //Coloca o bit para 0 na posi��o desejada
#define bit_inv(reg, bit)  ((reg) ^= (1<<(bit)))           //Inverte o bit na posi��o desejada

#endif
