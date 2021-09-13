# TPE1SO
TPE1 Sistemas Operativos Grupo 10.

Integrantes: Mauro Sambartolomeo, Juan Negro, Agustín Morantes.

## Instrucciones de compilación
Para compilar se debe ejecutar el comando ```make``` dentro de la raíz del repositorio.
Esto generará los ejecutables ```application.out``` ```view.out``` y ```worker.out``` en la raíz.

## Instrucciones de uso
Para ejecutar el programa se debe ejecutar el ejecutable ./application.out de la siguiente forma:
```
./application.out (-j WORKER_COUNT) [FILES]
```
El flag opcional ```-j``` sirve para especificar la cantidad de procesos workers.

También opcionalmente puede utilizar el proceso View para ver el output de los workers, de la siguiente forma:
```
./application.out ... | ./view.out
```
Opcionalmente si se quiere llamar al proceso View desde otro shell, se le puede pasar como parámetro el nombre de la shared memory que utiliza el proceso application, por ejemplo:
```
./view.out /sharedMemName
```

En caso de querer probar el worker manualmente, se lo puede llamar con el comando ```./worker.out``` y por entrada estándar se le pueden pasar los paths de los archivos a procesar separados con un newline. Para terminar su ejecución puede enviarle un EOF con ```CTRL+D```.
