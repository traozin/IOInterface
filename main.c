#include "libs/display.h"
#include "libs/uart_rasp.h"

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
    char text[] = "SD ---- 2";
    write_textLCD(text);

    int uart_filestream = uart_config();
    if(uart_filestream == -1){
        printf("\nFalha na abertura do arquivo UART!\n");
        return 0;
    }

    //cria uma thread que recebe as mensagens
    //pthread_create(&id_thread, NULL, receive_msg, &uart_filestream);

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
                //msg = "Situacao da NodeMCU: ";
                uart_send("30", uart_filestream);
                //sleep(3);
                break;
            case '2': // sensor analogico
                uart_send("40", uart_filestream);
                break;
            case '3': // sensor digital
                /*char sensor[] = "";
                printf("Qual sensor digital deseja selecionar? \n =>  ");
                scanf("%s", &sensor);
                int sensor = atoi(sensor);*/
                uart_send("50", uart_filestream);
                break;
            case '4':
                msg = "";
                uart_send("60", uart_filestream);
                write_textLCD("LED acionado!");
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
void * receive_msg(void * uart_filestream){
    int uart_filestream = *((int *) uart_filestream);
    
    while(1){
        if(msg != ""){            
            sleep(1); // aguarda 1 segundo para receber cada msg
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