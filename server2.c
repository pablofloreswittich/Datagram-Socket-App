#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

//Agregadas para broadcast
#include <netinet/in.h>
#include <unistd.h>

int main(void){
    //Declaramos variables del socket, puerto y banderas    
    int socket_desc, puerto, banderaExit=0, banderaLong=1, broadcastEnable=1;
    
    //Declaramos variables de tipo estructura socket, para server y client
    struct sockaddr_in server_addr, cliaddr;
    
    //Declaramos mensaje a enviar por UDP
    char message[200];

    // Guardamos tamano de la estructura del cliente
    int client_struct_length = sizeof(cliaddr);

    //Limpiamos consola y mensaje de bienvenida
    system("clear");    
    printf("\t********BIENVENIDO AL SERVIDOR!********\n\n");
    
    // Creamos socket UDP:
    // La funcion socket tiene 3 parametros: 
    // la familia de protocolos TCIP/IP, 
    // sockets orientados a datagramas (usamos UDP),
    // Adopta protocolo de transporte de UDP indicado por SOCK_DGRAM
    socket_desc = socket(AF_INET, SOCK_DGRAM, 0);
    
    // Control de creacion de socket: 0 si fue exitosa, -1 si no fue exitosa    
    if(socket_desc < 0){
        printf("Error al crear el socket\n");
        return -1;
    }
    printf("Socket creado correctamente\n");

    // Definimos opciones de broadcast
    // SO_BROADCAST: si el protcolo lo permite, habilita el envio de broadcast
    // ......
    if (setsockopt(socket_desc, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable))) {
        printf("No se pudo abrir el socket en modo broadcast");
        close(socket_desc);
        return;
    }

    //Obtengo puerto aleatorio entre 50 000 y 50 100
    srand (time(NULL));			//Inicializo el timer
	puerto = rand(); 		    //Obtengo el numero aleatorio	
    puerto=puerto%100;          
	puerto=50000 + puerto;

	printf("Puerto seleccionado: %d\n\n",puerto);
    
    // Definimos puerto e IP del cliente y del servidor:
    // Estos valores se asignan en la estructura definida de cada variable

    //Servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(puerto);
    server_addr.sin_addr.s_addr = inet_addr("192.168.1.2");
    bind(socket_desc,(struct sockaddr_in *)&server_addr,sizeof(server_addr));

    // Cliente
    memset(&cliaddr, 0, sizeof cliaddr);
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port=htons(2500);
    cliaddr.sin_addr.s_addr = inet_addr("255.255.255.255");
    
    //Mientras no se haya enviado el mensaje Exit    
    while(banderaExit==0){    
        // Esperamos mensaje a enviar del usuario
        do{
            // Limpiamos el buffer del mensaje:
            memset(message, '\0', sizeof(message));
            
            // Control de que el mensaje sea menor a 200 caracteres
            if(banderaLong==0){
                printf("\n\n\tERROR! El mensaje debe ser menor a 200 caracteres\n\n");
                memset(message, '\0', sizeof(message));        
            }
            printf("Ingrese el mensaje: ");
            gets(message);
            if(strlen(message)>200){
                banderaLong=0;
            }
            else{
                banderaLong=1;
            }
        }while(banderaLong==0);
        

        // Enviar el mensaje al cliente
        if(sendto(socket_desc, message, strlen(message), 0,
             (struct sockaddr*)&cliaddr, client_struct_length) < 0){
            printf("No fue posible enviar el mensaje\n");
            return -1;
        }
        if(strcmp(message,"Exit\0")==0){
            printf("Exit enviado\n\n");            
            banderaExit=1;
        }else{
            printf("\tMensaje enviado!\n");
        }
    }
    
    // Cerramos el socket:
    close(socket_desc);
    
    return 0;
}