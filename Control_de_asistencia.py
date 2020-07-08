import serial       # Para la comunicacion Serial
from datetime import date   # Para obtener la fecha
import unidecode

listaTxt = 'AsIstEEEncia.txt'

try:            # Se conecta al puerto Serial
    ser = serial.Serial('COM7')     # Cambia el puerto si es necesario
    ser.reset_input_buffer()
except:
    input("Revisa el puerto o vuelve a conectar el Pasalista")

def Lector():        # Lee la entrada Serial
    lec = []        # Array para guardar el codigo


    while len(lec)<4:   # Guarda el codigo en el Array
        try:
            ser_bytes = ser.readline()
            decoded_bytes = int(ser_bytes[0:len(ser_bytes)-2].decode("utf-8"))
            lec.append(str(decoded_bytes))
        except:
            ser.write(b'2')
            input("Keyboard Interrupt")
            break
    lec_tup = tuple(lec)    # Lo convierte en una tupla
    return(lec_tup)


def Procesar_txt(): # Procesa el archivo de texto con los codigos (lista.txt)
    try:
        listado = {}    # Diccionario para los codigos y nombres
        vector2 = []    # Array para
        codigos   = []  # Guarda los codigos
        nombres = []    # Guarda los nombres
        cont = 0        # Sirve de indice para navegar por el texto
        with open(listaTxt,encoding='utf-8') as archivo:
	           texto = archivo.readlines()
        # Creamos un vector con los nombres y otro con los codigos
        for i in texto:
            cont = cont + 1
            vector = i[0:-1]    # Guarda en vector una de las lineas de texto (menos el \n)
            if not cont%5 == 0:
                vector2.append(vector)  # Crea el codigo de 4 numeros
            else:
                nombres.append(vector)  # Añade un nombre
                codigos.append(vector2) # Añade un codigo
                cont = 0            # Reinicia el contador
                vector2 = []        # Reinicia el vector

        for i in range(len(nombres)):   # Se crea el dicionario
            listado[tuple(codigos[i])] = nombres[i]

    except:
        ser.write(b'2')
        input("Error con el archivo de la lista de códigos")
    return(listado)

def Anadir_txt(nombre,tupla):   # Añade un nombre a lista.txt
    try:
        with open(listaTxt,encoding='utf-8',mode="a") as archivo:  # Abre lista.txt
            for numero in tupla:                # Añade los numeros del codigo
                archivo.write(f"{int(numero)}\n")
            archivo.write(f"{nombre}\n")        # Añade el nombre
        with open(f"{date.today()}.txt","a") as archivo:    # Añade el nombre a la lista de hoy
            archivo.write(f"{nombre}\n")

    except:
        ser.write(b'2')
        input("Problemas añadiendo el nombre")
    return()

def Enviar(nombre):     # Pone el nombre que envia correcto para la LCD
    i = 2;
    sinTildes = unidecode.unidecode(nombre) # Quita las tildes
    try:
        while not sinTildes[i-2] == ' ':        # Deja el nombre y la primera letra del apellido
            i = i+1
            n = sinTildes[:i] + '.'
        ser.write(n.encode())                   # Envia el nombre
    except:
        ser.write(nombre.encode())                   # Envia el nombre tal cual (por si no tiene apellidos)
    return()

def main():
    Nombres = set({})           # Lista de los nombres que ya han pasado
    lista = Procesar_txt()      # Diccionario
    print(f"Asistencia {date.today()}\n")
    while True:                 # Bucle de funcionamiento
        lec_tup = Lector()      # Se lee la tarjeta
        if lec_tup in lista:    # Si el codigo esta en la lista, escribe el nombre em um txt
            nombre = lista.get(lec_tup)
            if not nombre in Nombres:
                Nombres.add(nombre)
                Enviar(nombre)
                with open(f"{date.today()}.txt","a") as archivo:
                    archivo.write(f"{nombre}\n")
                print(nombre)
        else:                   # Si no, te pide guardarlo
            ser.write(b'1')
            nombre_nuevo = input(f"No se encuentra el código {lec_tup}, introduce Nombre y Apellidos\n")
            Anadir_txt(nombre_nuevo,lec_tup)
            Enviar(nombre_nuevo)
            lista = Procesar_txt()
    return()

try:
    main()
finally:
    ser.close()     # Cierra la conexión serie con el Pasalista
