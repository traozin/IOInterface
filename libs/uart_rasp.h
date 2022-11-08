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
 * Envia uma mensagem via UART
 * @param uart_filestream - arquivo uart
*/
char* uart_receive(int uart_filestream){
	static char mensagem[] = ""; //define o tamanho da mensagem
	int msg_length = -1;
	sleep(2);

	while(msg_length == -1 || msg_length == 0){
		msg_length = read(uart_filestream, (void*)mensagem, 5);
		if(msg_length > 0){
			mensagem[msg_length] = '\0';
			if(strcmp(mensagem, "1F") == 0){
				char texto[] = "NodeMCU com problema!";
				write_textLCD(texto);
			}
			printf("\n\n\n %s \n\n\n", mensagem);
		}
	}
	return mensagem;
}
#endif
