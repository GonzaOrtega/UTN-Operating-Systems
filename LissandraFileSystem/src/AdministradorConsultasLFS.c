#include "AdministradorConsultasLFS.h"

int funcionCREATE(char* nombreTabla, char* cantParticiones, char* consistenciaChar, char* tiempoCompactacion) {
	if (existeTabla(nombreTabla)) {
		printf("Ya existe una tabla con el nombre %s en el FileSystem\n", nombreTabla);
		return 1;
	}
	crearTablaYParticiones(nombreTabla, cantParticiones);
	crearMetadataTabla(nombreTabla, consistenciaChar, cantParticiones, tiempoCompactacion);
	pthread_t threadCompactacion;
	pthread_create(&threadCompactacion, NULL, (void*) iniciarThreadCompactacion, nombreTabla);
	pthread_detach(threadCompactacion);
	return 0;
}

int funcionDROP(char* nombreTabla) {
	if (existeTabla(nombreTabla)) {
		removerTabla(nombreTabla);
		log_trace("%s eliminada\n\n", nombreTabla);
		return 0;
	} else {
		puts("La tabla que se quiere eliminar no existe");
	}
	return 1;
}

t_metadata_tabla funcionDESCRIBE(char* nombreTabla) {
	t_metadata_tabla metadata = obtenerMetadata(nombreTabla);
	mostrarMetadataTabla(metadata, nombreTabla);
	return metadata;
}

void funcionDESCRIBE_ALL() {
	mostrarMetadataTodasTablas(rutas.Tablas);
}

int funcionINSERT(double timeStamp, char* nombreTabla, char* key, char* value) {

	if (config->TAMANIO_VALUE < strlen(value)) {
		log_error(loggerError, "Tamanio maximo de value excedido");
		return 1;
	}
	if (existeTabla(nombreTabla)) {
		insertarKey(nombreTabla, key, value, timeStamp);
		log_trace(loggerInfo, "Insert de %s;%s en %s realizado en memtable", key, value, nombreTabla);
		return 0;
	} else {
		log_error(loggerError, "La tabla %s no existe",nombreTabla);
		return 1;
	}
}

t_registro* funcionSELECT(char*nombreTabla, int keyActual) {
	if (existeTabla(nombreTabla)) {
		t_registro* registro = getRegistroByKeyAndNombreTabla(nombreTabla, keyActual);

		if (registro != NULL) {
			printf("\nRegistro con mayor timestamp: %f;%d;%s\n", registro->timestamp, registro->key, registro->value);

			log_info(loggerInfo, "Select a key %d", registro->key);

			return registro;

		} else {
			puts("No hay registros para mostrar");
			return NULL;
		}
	} else {
		log_error(loggerError,"La tabla sobre la que se quiere hacer SELECT no existe en LFS\n");
		return NULL;
	}
}

void procesoDump() {

	while (1) {
		pthread_mutex_lock(&mutexDump);

		usleep(config->TIEMPO_DUMP * 1000);
		//log_info(loggerInfo, "Iniciando proceso Dump");
		crearYEscribirArchivosTemporales(rutas.Tablas);

		pthread_mutex_unlock(&mutexDump);
	}
}
