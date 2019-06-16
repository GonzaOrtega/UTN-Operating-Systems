/*
 ============================================================================
 Name        : Memoria.c
 Author      : Matias Erratchu y Julian Sarrelli
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include "Memoria.h"



int main() {
	logger = log_create("MEM_logs.txt", "MEMORIA Logs", true, LOG_LEVEL_DEBUG);
	log_info(logger, "--Inicializando proceso MEMORIA--");
	cargarConfiguracion();
	int socketFileSystem = HandshakeInicial();
	inicializarMemoria(valueMaximoPaginas, configuracion->TAM_MEMORIA,socketFileSystem);
	log_info(logger, "--Memoria inicializada--");

	pthread_create(&threadId, NULL, leerConsola, NULL);
	pthread_detach(threadId);


	//pruebas varias
//	insertarSegmentoEnMemoria("TABLA1",NULL);
//	insertarSegmentoEnMemoria("TABLA2",NULL);
//	Segmento* tabla1= buscarSegmentoEnMemoria("TABLA1");
//	insertarPaginaEnMemoria(3,"juli" ,tabla1,100);
//	//SELECT_MEMORIA("TABLA1" , 3);

	pthread_create(&intTemporalJournal, NULL, procesoTemporalJournal, NULL);
	pthread_detach(intTemporalJournal);

	pthread_create(&intTemporalGossiping, NULL, procesoTemporalGossiping, NULL);
	pthread_detach(intTemporalGossiping);

	log_info(logger, "Configurando Listening Socket...");
	int listenningSocket = configurarSocketServidor(configuracion->PUERTO_ESCUCHA);
	if (listenningSocket != -1) {
		escuchar(listenningSocket, socketFileSystem);
	}


	close(listenningSocket);
	log_destroy(logger);
	free(configuracion);
	config_destroy(archivo_configuracion);

	return EXIT_SUCCESS;
}

void* leerConsola()
{
	char * consulta;
	while (1) {

		puts("\nIngrese comandos a ejecutar. Para salir presione enter");
		consulta = readline(">");
		if (consulta) {
			add_history(consulta);
		}
		if (consulta == NULL) {
			return NULL;
		}
		procesarConsulta(consulta);

	}
	log_info(logger, "Fin de leectura por consola");
	return NULL;
}

int HandshakeInicial()
{
	int socketFileSystem = ConectarAServidor(configuracion->PUERTO_FS, configuracion->IP_FS);
	log_info(logger, "Memoria conectada a File System");
	EnviarDatosTipo(socketFileSystem, MEMORIA, NULL, 0, CONEXION_INICIAL_FILESYSTEM_MEMORIA);
	Paquete paquete;
	void* datos;
	if (RecibirPaqueteCliente(socketFileSystem, MEMORIA, &paquete) > 0) {
		datos = malloc(paquete.header.tamanioMensaje);
		datos = paquete.mensaje;
		memcpy(&valueMaximoPaginas, datos, sizeof(int));
	}

	log_info(logger, "Handshake inicial realizado. Value Maximo: %d", valueMaximoPaginas);
	return socketFileSystem;
}

void cargarConfiguracion() {
	pathMEMConfig = "/home/utnso/tp-2019-1c-Los-Sisoperadores/Memoria/configMEM.cfg";
	log_info(logger, "Levantando archivo de configuracion del proceso MEMORIA");
	configuracion = (MEMORIA_configuracion*) malloc(sizeof(MEMORIA_configuracion));
	if (configuracion == NULL) {
		exit(0);
	}
	archivo_configuracion = config_create(pathMEMConfig);
	configuracion->PUERTO_ESCUCHA = get_campo_config_string(archivo_configuracion, "PUERTO_ESCUCHA");
	configuracion->IP_FS = get_campo_config_string(archivo_configuracion, "IP_FS");
	configuracion->PUERTO_FS = get_campo_config_int(archivo_configuracion, "PUERTO_FS");
	configuracion->IP_SEEDS = get_campo_config_string(archivo_configuracion, "IP_SEEDS");
	configuracion->PUERTOS_SEEDS = get_campo_config_string(archivo_configuracion, "PUERTOS_SEEDS");
	configuracion->RETARDO_MEMORIA = get_campo_config_int(archivo_configuracion, "RETARDO_MEMORIA");
	configuracion->RETARDO_FS = get_campo_config_int(archivo_configuracion, "RETARDO_FS");
	configuracion->TAM_MEMORIA = get_campo_config_int(archivo_configuracion, "TAM_MEMORIA");
	configuracion->TIEMPO_JOURNAL = get_campo_config_int(archivo_configuracion, "TIEMPO_JOURNAL");
	configuracion->TIEMPO_GOSSIPING = get_campo_config_int(archivo_configuracion, "TIEMPO_GOSSIPING");
	configuracion->MEMORY_NUMBER = get_campo_config_int(archivo_configuracion, "MEMORY_NUMBER");
	log_info(logger, "Archivo de configuracion levantado");

}

void procesoTemporalJournal(){
	while(1){
		usleep(configuracion->TIEMPO_JOURNAL);
		journalMemoria();
	}
}

void procesoTemporalGossiping(){
	while(1){
		usleep(configuracion->TIEMPO_GOSSIPING);
		//falta funcion gossiping
	}

}

