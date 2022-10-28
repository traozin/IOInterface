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
        //entryModeSet();
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
    unsigned char mensagem[33]; //define o tamanho da mensagem
	if (uart_filestream != -1){
		int msg_length = read(uart_filestream, (void*)mensagem, 32);
		if (msg_length < 0){
			printf("\nErro na leitura!\n");
		}
		else{
			mensagem[msg_length] = '\0';
            strcpy(msg, mensagem);
		}
	}
	else{
		printf("\nFalha na abertura do arquivo");
	}
    return msg;
}

int main() {
    initDisplay();  // inicializa o display lcd
    char* text = {"Iniciando conexão..."};
    write_textLCD(text);

    int uart_filestream = uart_config();
    if(uart_filestream == -1){
        return 0;
    }

    char opcao = '1';
    char* msg = {""};

    do{
        printf("========================================\n");
        printf("            Escolha uma opcao\n");
        printf("----------------------------------------\n");
        printf("| 1 | Situação atual do NodeMCU\n");
        printf("| 2 | Valor da entrada analógica\n");
        printf("| 3 | Valor de uma das entradas digitais\n");
        printf("| 4 | Acender LED\n");
        printf("| 0 | Sair\n");
        printf("========================================\n");
        printf("=>  ");

        scanf("%s", &opcao);
        system("cls || clear");
        //msg = uart_receive(uart_filestream); //recebe a mensagem
        //printf(msg);
        switch(opcao){
            case '1':
                break;
            case '2':
                break;
            case '3':
                break;
            case '4':
                break;
            case '0':
                printf("\n\n\tFinalizando...\n");
                break;
            default:
                printf("\n\nOpcao invalida!\n\n");
        }
    } while(opcao);

    return 0;
}