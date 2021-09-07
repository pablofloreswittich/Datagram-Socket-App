#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(void){
    int socket_desc, banderaExit=0;
    struct sockaddr_in server_address, client_address;
    char mensaje[200];
    int client_struct_length = sizeof(client_address);
    int server_struct_length = sizeof(server_address);

    system("clear");    
    printf("\t********BIENVENIDO AL CLIENTE!********\n\n");
    
    // Creamos el socket UDP:
    socket_desc = socket(AF_INET, SOCK_DGRAM, 0);
    
    if(socket_desc < 0){
        printf("Error al crear el socket\n");
        return -1;
    }
    printf("Socket creado correctamente\n");
    
    // Definimos puerto e IP del cliente:
    client_address.sin_family = AF_INET;
    client_address.sin_port = htons(2500);
    client_address.sin_addr.s_addr = htons(INADDR_ANY);
    
    // Bindeamos el puerto y la IP al socket:
    if(bind(socket_desc, (struct sockaddr*)&client_address, sizeof(client_address)) < 0){
        printf("No fue posible bindear al puerto\n");
        return -1;
    }
    printf("Binding exitoso.\n");
    
    printf("Esperando nuevos mensajes...\n\n");
    
    while(banderaExit==0){
        // Se recibe mensaje del servidor:
        if (recvfrom(socket_desc, mensaje, sizeof(mensaje), 0,
             (struct sockaddr*)&server_address, &server_struct_length) < 0){
            printf("No se pudo recibir\n");
            return -1;
        }

        printf("\n\n\t**** Mensaje recibido! ****\n\t- IP Origen:\t%s\n\t- Puerto:\t%i", inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port));

        //Mostrar hora y fecha al recibir segmento
        time_t tiempo = time(0);
        struct tm *tlocal = localtime(&tiempo);
        char output[128];
        strftime(output,128,"\n\t- Fecha:\t%d/%m/%y\n\t- Hora:\t\t%H:%M:%S",tlocal);
        printf("%s\n",output);

        //Mostrar mensaje recibido
        printf("\t- Mensaje:\t%s\n", mensaje);

        if(strcmp(mensaje,"Exit\0") == 0){
            printf("Exit Recibido\n");
            banderaExit=1;
        }

        // Limpiamos el buffer del mensaje:
        memset(mensaje, '\0', sizeof(mensaje));
    }
    
    // Cerramos el socket:
    close(socket_desc);
    
    return 0;
}
