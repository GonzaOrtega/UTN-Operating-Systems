#ifndef MEMORIA_H_
#define MEMORIA_H_

#include <Libraries.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <commons/config.h>
#include "MemoriaPrincipal.h"
#include "AdministradorDeConsultas.h"
#include "Sockets/Conexiones.h"
#include "Sockets/Serializacion.h"
#include "Parser.h"
#include "SocketServidorMemoria.h"

typedef struct MEMORIA_configuracion{
	char* PUERTO_ESCUCHA;
	char* IP_FS;
	int PUERTO_FS;
	char* IP_SEEDS;
	char* PUERTOS_SEEDS;
	int RETARDO_MEMORIA;
	int RETARDO_FS;
	int TAM_MEMORIA;
	int TIEMPO_JOURNAL;
	int TIEMPO_GOSSIPING;
	int MEMORY_NUMBER;
}MEMORIA_configuracion;

t_log* logger;
char* pathMEMConfig;
MEMORIA_configuracion* configuracion;
pthread_t threadId;
pthread_t intTemporalJournal;
pthread_t intTemporalGossiping;
t_config*  archivo_configuracion;
int valueMaximoPaginas;

void cargarConfiguracion();
void* leerConsola();
int HandshakeInicial();
void procesoTemporalJournal();
void procesoTemporalGossiping();
#endif /*MEMORIA_H_ */
