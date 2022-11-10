#ifndef UART_RASP_H
#define UART_RASP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Usado para comunicao uart
#include <unistd.h>	
#include <fcntl.h>
#include <termios.h>
#include <time.h>


/**
 * Realiza as configuracoes iniciais da UART
*/
int uart_config(){
    //Abertura do arquivo da UART
	int uart_filestream = -1;
	uart_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY); // Abre em modo escrita/leitura bloqueado
	if (uart_filestream == -1){
		printf("\nFalha na abertura do arquivo!\n");
        return uart_filestream;
	}

    struct termios options;
    tcgetattr(uart_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart_filestream, TCIFLUSH);
    tcsetattr(uart_filestream, TCSANOW, &options);

    return uart_filestream;
}

/**
 * Envia uma mensagem via UART
 * @param msg - texto a ser enviado
 * @param uart_filestream - arquivo uart
*/
void uart_send(char* msg, int uart_filestream){
	if(write(uart_filestream, msg, strlen(msg)) == -1){
		printf("\nFalha ao enviar mensagem!\n");
	}
}

/**
 * Envia uma mensagem com o tamanho especificado
 * @param uart_filestream - arquivo uart
 * @param tamanho_mensagem - tamanho da mensagem a ser recebida
*/
char* uart_receive(int uart_filestream, int tamanho_mensagem){
	static char mensagem[] = ""; //define o tamanho da mensagem
	int msg_length = -1;
	int tamanho = 0;
	int tamanho_mensagem_aux = tamanho_mensagem;

	while(tamanho < tamanho_mensagem){ // se a mensagem for menor ou igual a zero
		char aux[] = ""; //variavel auxiliar que armazena o tamanho da string
		msg_length = read(uart_filestream, (void*)aux, tamanho_mensagem_aux); // ate 5 bytes
		
		if(msg_length > 0){
			aux[msg_length] = '\0';
			if(strcmp(aux, "1F") == 0){
				char texto[] = "Erro na NodeMCU!";
				write_textLCD(texto);
			}else{
				tamanho = tamanho + msg_length;
				tamanho_mensagem_aux = tamanho_mensagem_aux - msg_length;
				sprintf(mensagem, "%s", aux); // concatena o resultado dentro do loop 
			}
		}
	}
	return mensagem;
}

#endif
