#include <stm32f30x.h>
#include <stm32f30x_gpio.h>
#include <stm32f30x_rcc.h>
#include <stm32f30x_usart.h>

int putchar(char c){
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	USART1->TDR = (c & 0xff);
	return 0;
}

int getchar(void){
	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	return USART1->RDR & 0xff;
}

int main() {
  // Enable rcc, AF and gpioa
  // PA8 CLK, PA9 Tx, PA10 Rx

  // Init RCC
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  // Configure Alternate Function
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_7);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_7);

  // Set GPIO
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_StructInit(&GPIO_InitStructure);
  USART_StructInit(&USART_InitStructure);

  // Configure USART Tx
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// Configure USART Rx
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// Configure USART
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	// Enable usart
	USART_Cmd(USART1, ENABLE);

	// Enable RX interrupt, disable TX interrupt
	// USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
	// USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	const char hello[] = "Hello\0";
	while(1){
		int i=0;
		while(hello[i]!='\0'){
			putchar(hello[i]);
			i++;
		}
	}
  return 0;
}