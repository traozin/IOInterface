#include "libs/display.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Usado para comunicacao UART
#include <unistd.h>	
#include <fcntl.h>
#include <termios.h>


/**
 * Escre um texto no display LCD
 * @param  palavra[] - texto a ser escrito no display
*/
void write_textLCD(char* palavra){
    clearDisplay(); // limpa o display
    int i = 0;

    while(palavra[i] != '\0'){
        write_lcd(palavra[i]); // exibe no display cada caracter
        i++;
    }
}

/**
 * Realiza as configuracoes iniciais da UART
*/
int uart_config(){
    //Abertura do arquivo da UART
	int uart_filestream = -1;
	uart_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY); // Abre em modo escrita/leitura bloqueado
	if (uart_filestream == -1){
		printf("\nFalha na abertura do arquivo!\n");
        return 0;
	}

    struct termios options;
    tcgetattr(uart_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart_filestream, TCIFLUSH);
    tcsetattr(uart_filestream, TCSANOW, &options);

    return 1;
}

/**
 * Envia uma mensagem via UART
 * @param msg - texto a ser enviado
 * @param uart_filestream - arquivo uart
*/
void uart_send(char* msg, int uart_filestream){
	if (uart_filestream != -1){	//Se abriu o arquivo da UART
		int c = write(uart_filestream, msg, strlen(msg));
        if(c == -1){
            printf("\nFalha ao enviar mensagem!\n");
        }
	}	
	else{
		printf("\nFalha na abertura do arquivo!\n");
	}
}

/**
 * Envia uma mensagem via UART
 * @param uart_filestream - arquivo uart
*/
unsigned char* uart_receive(int uart_filestream){
    unsigned char* msg;
    unsigned char mensagem[9]; //define o tamanho da mensagem
	if (uart_filestream != -1){
		int msg_length = read(uart_filestream, (void*)mensagem, 8);
		if (msg_length < 0){
			printf("\nErro na leitura!\n");
		}
		else if (msg_length == 0){
			printf("\nSem mensagem!\n");
            strcpy(msg, mensagem);
		}
		else{
			mensagem[msg_length] = '\0';
		}
	}
	else{
		printf("\nFalha na abertura do arquivo");
	}
    return msg;
}

int main() {
    initDisplay();  // inicializa o display lcd
    char* text = {"Testando"};
    write_textLCD(text);

    if(uart_config() == 0){
        return 0;
    }

    return 0;
}