#include "display.h"
#include "uart_rasp.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


char* msg = "";
pthread_mutex_t mutex; // variavel de controle para msg
pthread_t id_thread;

int main() {
    initDisplay();  // inicializa o display lcd
    char text[] = "Problema 2 - SD";
    write_textLCD(text);

    int uart_filestream = uart_config();
    if(uart_filestream == -1){
        printf("\nFalha na abertura do arquivo UART!\n");
        return 0;
    }

    //cria uma thread que recebe as mensagens
    //pthread_create(&id_thread, NULL, receive_msg, &uart_filestream);

    char sensor = '1';
    char opcao = '/';
    do{
        printf("========================================\n");
        printf("            Escolha uma opcao           \n");
        printf("----------------------------------------\n");
        printf("| 1 | Situação atual do NodeMCU\n"); // 0x03
        printf("| 2 | Valor da entrada analógica\n"); // 0x04
        printf("| 3 | Valor das entradas digitais\n"); // 0x05
        printf("| 4 | Acender/Apagar LED\n"); // 0x06
        printf("| 0 | Sair\n");
        printf("========================================\n");
        printf("=>  ");
        scanf("%s", &opcao);
        system("cls || clear");

        switch(opcao){
            case '1':
                uart_send("30", uart_filestream);
		if(uart_receive(uart_filestream) == "00"){
			write_textLCD("NodeMCU OK!");
		}
                break;
            case '2': // sensor analogico
		msg = "Sensor analogico: ";
                uart_send("40", uart_filestream);
                break;
            case '3': // sensor digital
                /*printf("\nQual sensor digital deseja selecionar? [1-8] \n =>  ");
                scanf("%s", &sensor);
		if(sensor >= '1' && sensor <= '8'){
			uart_send("51", uart_filestream);
			uart_receive(uart_filestream);
			msg = "Sensor digital: ";
		}else{
			printf("\nOpção inválida!\n");
		}*/
                break;
            case '4':
                uart_send("60", uart_filestream);
		if(uart_receive(uart_filestream)[0] == '1'){
                        write_textLCD("LED ligado!");
                }else{
			write_textLCD("LED desligado!");
		}
                break;
            case '0':
                printf("\n\n\tFinalizando...\n");
                break;
            default:
                printf("\n\n\tOpcao invalida!\n\n");
        }
    } while(opcao != '0');

    return 0;
}

/**
 * Recebe a mensagem e escreve no display continuamente
 * @param uart_filestream - arquivo UART
*/
void * receive_msg(void * uart){
    int uart_filestream = *((int *) uart);

    while(1){
        if(msg != ""){
            char* texto = uart_receive(uart_filestream);
            if(texto != ""){
                char* n_msg = strcat(msg, texto);
                write_textLCD(n_msg);
            }
        }
    }
}

/**
 * Altera o valor da mensagem utilizando mutex para evitar conflitos
 * @param nova_msg - nova mensagem a ser atribuida
*/
void alteramsg(char* nova_msg){
    pthread_mutex_lock(&mutex); // segura o recurso
    msg = nova_msg;
    pthread_mutex_unlock(&mutex); // libera o recurso

}
