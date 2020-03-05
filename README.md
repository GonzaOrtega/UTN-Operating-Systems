# Trabajo Practico de Sistemas Operatios 1C 2019

# Objetivo

El objetivo del trabajo práctico consiste en desarrollar una solución que permita la simulación de
ciertos aspectos de una base de datos distribuida, es decir donde los distintos recursos del sistema
puedan realizar una carga de trabajo ejecutando desde distintos puntos.
Dichos componentes incluidos dentro de la arquitectura del sistema deberán trabajar en conjunto
para la planificación y ejecución de distintos scripts. Estos scripts estarán conformados por
múltiples Requests (operaciones como leer valores, escribirlos, crear tablas, etc), los cuales se
asemejan a las principales operaciones de una base de datos.
Los componentes del sistema serán:
  - un kernel
  - un pool de memorias
  - un file system.

Los datos con los que se trabajará estarán organizados en estructuras de tablas , siendo los registros
de estas del tipo Key - Value.

Los Request principales que le podremos solicitar al sistema son:

  SELECT: Permite la obtención del valor de una key dentro de una tabla.
  
  INSERT: Permite la creación y/o actualización de una key dentro de una tabla.
  
  CREATE: Permite la creación de una nueva tabla dentro del file system.
  
  DESCRIBE: Permite obtener la Metadata de una tabla en particular o de todas las tablas que el filesystem tenga.
  
  DROP: Permite la eliminación de una tabla del file system.

## ¿Como usar?

* Clonar el repo
* Correr el bash ejecutando el comando `bash deploy.sh`
* Correr los modulos (LFS, Memoria y Kernel) en ese orden, y listo.
