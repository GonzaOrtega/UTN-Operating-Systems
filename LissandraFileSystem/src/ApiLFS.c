/*
 * ApiLFS.c
 *
 *  Created on: 23 abr. 2019
 *      Author: utnso
 */

#include "LissandraFileSystem.h"
#include "funcionesLFS.h"
#include "FileSystem.h"

void consolaLFS(){
	puts("Bienvenido a la consola de LFS. Ingrese un comando:");
	while(1){
		char *linea = readline(">");

		if(!strcmp(linea, "")){
			free(linea);
			continue;
		}
		procesarInput(linea);
		free(linea);
	}
}

void procesarInput(char* linea) {
	int cantidad;
	char **palabras = string_split(linea, " ");
	cantidad = cantidadParametros(palabras);
	if (!strcmp(*palabras, "INSERT")) {
		//consolaInsert(palabras, cantidad);

	} else if (!strcmp(*palabras, "SELECT")) {
		//consolaSelect(palabras,cantidad);
	} else if (!strcmp(*palabras, "CREATE")) {
		consolaCreate(palabras,cantidad);
	} else if (!strcmp(*palabras, "DESCRIBE")) {
		consolaDescribe(palabras,cantidad);
	} else if (!strcmp(*palabras, "DROP")) {
		consolaDrop(palabras,cantidad);
	} else if(!strcmp(*palabras, "exit")){
		printf("Finalizando consola\n");



	} else {
		puts("El comando no existe.");
	}
	liberarPunteroDePunterosAChar(palabras);
	free(palabras);

}


void consolaCreate(char**palabras, int cantidad){
	if(cantidad == 4){

			if(existeTabla(palabras[1])){
				puts("Ya existe una tabla con ese nombre.");
			}
			else{


				crearTablaYParticiones(palabras[1], palabras[3]);
				crearMetadataTabla(palabras[1],palabras[2], palabras[3], palabras[4]);

			}
	}
	else{
		puts("Error en la cantidad de parametros.");
	}
}

void consolaDescribe(char**palabras, int cantidad){
	if(cantidad == 1){

			if(existeTabla(palabras[1])){
				mostrarMetadataTabla(palabras[1]);

			}else{
				printf("La %s no existe", palabras[1]);
			}

	}else if(cantidad==0){
		mostrarMetadataTodasTablas(rutas.Tablas);

	}
	else{
		puts("Error en la cantidad de parametros.");
	}
}


void consolaDrop(char**palabras, int cantidad){
	if(cantidad == 1){

			if(existeTabla(palabras[1])){
				removerTabla(palabras[1]);
				printf("%s eliminada", palabras[1]);
			}

	}
	else{
		puts("Error en la cantidad de parametros.");
	}
}

void consolaInsert(char**palabras, int cantidad){
	if(cantidad == 4){

			if(existeTabla(palabras[1])){
				double timestamp = atof(palabras[4]);
				insertarKey(palabras[1], palabras[2], palabras[3],timestamp);
				log_info(logger, "Insert realizado en memtable");

			} else{
				printf("La %s no existe", palabras[1]);
			}

	}else if(cantidad==3){
		if(existeTabla(palabras[1])){
		double timestamp = getCurrentTime();
		insertarKey(palabras[1], palabras[2], palabras[3], timestamp);
		log_info(logger, "Insert realizado en memtable");

		}else{
			printf("La %s no existe", palabras[1]);
		}

	}
	else{
		puts("Error en la cantidad de parametros.");
	}
}
