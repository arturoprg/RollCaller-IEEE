# RollCaller-IEEE
RollCaller es un proyecto desarrollado por IEEE Student Branch of Granada con la finalidad de agilizar el proceso de pasar lista en las reuniones de la rama. Para ello se utiliza un sistema controlado por un Arduino nano que lee el código RFID asociado a la Tarjeta Universitaria (TUI) de los miembros.

La versión 2.0 utiliza solo el Arduino, mientras que la versión 3.0 utiliza Arduino y Python para guardar los registros de las tarjetas de forma independiente y mejorando la velocidad.

Uso versión 3.0: Tras conectar el arduino, inicia el programa de python y pasa las tarjetas, se creará un fichero con la fecha del día y la lista de los que han pasado la tarjeta.

Tras cargar RollCaller V3.0.ino en el dispositivo del Pasalista, ejecutamos el programa RollCaller V3.0.py y procedemos a pasar las tarjetas por el RollCaller, se creará un nuevo archivo de texto con la fecha del día donde se guardarán las personas que han pasado su tarjeta.

Si la tarjeta no está registrada, el programa de Python pedirá que introduzcas el nombre y apellidos del miembro, tras lo cuál quedará registrada la tarjeta y se añadirá a la lista de asistencia.

Crea un archivo llamado lista.txt donde se guardarán las tarjetas registradas de la siguiente forma:
Primer número
Segundo número
Tercer número
Cuarto número
Nombre y Apellidos

Para un correcto funcionamiento el archivo debe terminar con un salto de línea. Si el programa de Python tiene problemas leyendo lista.txt, puede que el archivo se haya corrompido, prueba a copiar el contenido del archivo en un nuevo archivo txt con el mismo nombre.

En RollCaller V3.0.py se puede editar el nombre del archivo que registra las tarjetas en la línea 5, y el puerto al cuál está conectado el Arduino en la línea 8.

Página del proyecto: https://edu.ieee.org/es-ugr/proyectos/rollcaller/
