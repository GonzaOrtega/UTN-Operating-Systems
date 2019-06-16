#include "SocketClienteMemoria.h"

Segmento* buscarSegmentoEnFileSystem(char* nombreSegmento) {
	int socketFileSystem = ConectarAServidor(configuracion->PUERTO_FS, configuracion->IP_FS);

	EnviarDatosTipo(socketFileSystem, MEMORIA, (void*) nombreSegmento, strlen(nombreSegmento), DESCRIBE);
	Paquete paquete;
	RecibirPaqueteCliente(socketFileSystem, FILESYSTEM, &paquete);

	if (paquete.header.tipoMensaje == NOTFOUND) {
		return NULL;
	}

	void*cadenaRecibida = malloc(paquete.header.tamanioMensaje);
	char** datos = string_split(cadenaRecibida, " ");
	char* nombreSegmentoRecibido = datos[0];
	t_consistencia consistencia = getConsistenciaByChar(datos[1]);
	int cantParticiones = atoi(datos[2]);
	int tiempoCompactacion = atoi(datos[3]);

	t_metadata_tabla* metadata = malloc(sizeof(t_metadata_tabla));
	metadata->CONSISTENCIA = consistencia;
	metadata->CANT_PARTICIONES = cantParticiones;
	metadata->T_COMPACTACION = tiempoCompactacion;

	Segmento* segmentoRecibido = malloc(sizeof(Segmento));
	segmentoRecibido->nombreTabla = malloc(strlen(nombreSegmentoRecibido));
	strcpy(segmentoRecibido->nombreTabla, nombreSegmentoRecibido);
	segmentoRecibido->metaData = metadata;

	return segmentoRecibido;
}

void enviarRegistroAFileSystem(Pagina* pagina, char* nombreSegmento) {
	int socketFileSystem = ConectarAServidor(configuracion->PUERTO_FS, configuracion->IP_FS);
	t_registro* registro = pagina->registro;
	char* consulta = malloc(
			strlen(nombreSegmento) + sizeof(registro->key) + strlen(registro->value) + sizeof(registro->timestamp) + 3); //mas 3 espacios
	sprintf(consulta, "%s %d %s %f", nombreSegmento, registro->key, registro->value, registro->timestamp);
	EnviarDatosTipo(socketFileSystem, MEMORIA, consulta, strlen(consulta), INSERT);
}

void eliminarSegmentoFileSystem(char* nombreSegmento) {
	int socketFileSystem = ConectarAServidor(configuracion->PUERTO_FS, configuracion->IP_FS);

	char* consulta = malloc(strlen(nombreSegmento) + 1); // +1 por el espacio
	sprintf(consulta, "%s", nombreSegmento);
	EnviarDatosTipo(socketFileSystem, MEMORIA, consulta, strlen(consulta), DROP);
}

int enviarCreateAFileSystem(t_metadata_tabla* metadata, char* nombreTabla)
{
	int socketFileSystem = ConectarAServidor(configuracion->PUERTO_FS, configuracion->IP_FS);
	char* consulta = malloc(strlen(nombreTabla) + 2 + sizeof(int) + sizeof(int) + 3);
	sprintf(consulta, "%s %s %d %d", nombreTabla, getConsistenciaCharByEnum(metadata->CONSISTENCIA),
			metadata->CANT_PARTICIONES, metadata->T_COMPACTACION);
	EnviarDatosTipo(socketFileSystem, MEMORIA, consulta, strlen(consulta), CREATE);
	free(consulta);

	Paquete paquete;
	int succes = 1;
	if (RecibirPaqueteCliente(socketFileSystem, FILESYSTEM, &paquete) > 0) {
		succes = atoi(paquete.mensaje);
	}

	if (succes == 1) {
		return 1; //es un error, la tabla ya existe
	}
	return 0;
}
