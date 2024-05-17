# **Simulacion de Trafico de Internet**   

## Trabajo Final de Algoritmos y Estructuras de Datos   

Desarrollo de un programa programa que simule el trafico de datos, al *estilo* del funcionamiento del Internet en C++ con Programacion Orientada a Objetos.    

### Con este programa se pretende cumplir con la siguiente **consigna**:  


- Existen **n maquinas** que cumplen la funcion de **routers** que se encargan de rutear los datos desde una maquina de origen hacia la maquina destino.
  
- Existen otras **k maquinas**, las **terminales**, que son emisoras-receptoras de paginas. Cada una de estas maquinas esta conectada a un unico router que es el encargado de enviar/recibir las paginas hacia/desde el destino final.  

### **Ruteo de los datos**: 

Cada router esta conectado directamente a 1 o mas routers para transmitir los paquetes de un determinado ancho de banda.   

Cada router sabe cuales son las maquinas terminales que tiene conectadas y cuales son los routers vecinos que tiene (es consciente de sus conexiones directas).  

Ademas cada router tienen una **tabla** de destinos que le indica a que router enviar los datos con un determinado destino.  

### **Armado de paquetes**:  

Cuando un Router recibe una pagina para enviar de una de sus terminales, este la divide en n paquetes de igual tamaño y los va enviando por la ruta elegida *de a un paquete por vez.*  
Es decir que una pagina pedida por otra terminal se divide y se envia de a segmentos.  

### **Recepcion de paquetes**:    

1. Cuando un router va recibiendo de otro router paquetes con un determinado destino hay dos opciones:  
   
   - Debe reenviarlo al router vecino correspondienteen la ruta.
     
   - Si el destino final es una maquina terminal conectada directamente.    
       
      > Debe ir almacenando los paquetes recibidos hasta que esten todos los que correspondan a la pagina enviada.
      >
      > > Luego se rearma la pagina y recien ahi se envia a la maquina destino.  

2. Cada router tiene una cola de envios (cola de trafico) para cada router vecino, en donde van encolando los paquetes que tienen que enviarse por ese canal y que luego  envia por cada turno, todos los paquetes quee su ancho de banda le permite.  

   >No se deben colocar todos los paquetes de una pagina consecutivos: deben ser intercalados con los paquetes que provengan de otra maquina, para que se vayan enviando parcialmente.
   >
   >>Esto evita que un envio muy pesado atore al server y los otros paquetes demoren mmucho en ser enviados.

### En resumen, las **Funciones del router** son las siguientes:

  a. Recibir una pagina de una maquina cliente, dividirla en los paquetes que corresponda y enviarla a la cola de trafico de la ruta que corresponda.  

  b.  Recibir paquetes de los routers vecinos y redireccionarlos hacia el router vecino que corresponda si la direccion del paquete no es la propia del router, o bien si la direccion del paquete es la del router en cuestion, debe esperar a recibir todos los paquetes que corresponden a la pagina enviada y una vez sucedido esto, enviar la pagina a la maquina de destino.  

## Tabla de Destinos:  

Las direcciones de las maquinas son del tipo IP pero simplificadas. Tienen dos partes de byte cada una: 

  - La primera indica el router.  

  - La segunda indica la maquina terminal conectada a dicho router.  

Es decir que pueden haber 256 routers con 256 maquinas cada uno.  

Para computar la tabla:

  - Si la direccion del paquete corresponde a la de un router vecino, hay una conexion directa, por lo cual no hay tramite.  

  - Para routers que no son vecinos puede haber varias rutas alternativas, debiendo el router elegir aquella con menos carga de trafico.  

### **Administrador del Sistema**:  

Es el encargado de recomputar las rutas optimas de todos los routers periodicamente.  

Cada router envia el tamaño de la cola de espera de envios hacia cada router vecino y el administrador determina la ruta optima pasando por los routers que tengan menor trafico pendiente en relacion al ancho de banda de la conexion con ese router.  

Cada router envia j paquetes por ese canal, segun el ancho de banda que el canal tenga.  

#### **¿Como se determina el camino optimo?**  

Lo importante es la cantidad de ciclos que un nuevo paquete debe esperar hasta ser enviado. Ademas pierde un ciclo al entrar a un router y volver a salir.  

Una vez que el Administrador determina los caminos optimos, se los informa a cada router. Estos caminos son utilizados hasta que vuelven a recomputarse.  

### Caso Puntual a Simular:  

Se supone una pagina que se dividio en 50 paquetes. Se enviaron 20. Se recomputa el camino optimo y se cambia de ruta. Los 30 paquetes restantes van por otra ruta, que al ser tomada como optima, pueden llegar a destino antes que los primeros 20.  

Constara de *1 ciclo* en que cada uno de los routers haga las tareas de recepcion y reenvio o almacenamiento de paquetes que tienen que hacer, de a uno por vez.  

Cada 2 ciclos, tomara el control el administrador para recomputar los caminos optimos y volvera al computo de ciclos.  

**Consideraciones importantes**:   

1. Se debera utilizar numeros aleatorios para simular la generacion de paginas a ser enviadas, el destino y el tamaño de cada pagina.  

2. La cantidad de routers, terminales por router, las conexiones directas de los rputers y el ancho de banda entre los routers y entre cada terminal y el router asociado debera ser configurable y definido por un archivo que parametrice el sistema.  

## **Solucion propuesta**:  

### Clases Principales:  
   
1. *Router*: la clase principal. Diseñada para representar objetos que manipulen paquetes y esten en todas sus etapas tanto en la generacion como la recepcion, el envio y la convergencia al formar una pagina. Distribuyen los paquetes de la forma mas efectiva posible haciendo uso de clases suplementarias para agilizar y optimizar su ejecucion.  
   
2. *Terminal*: SRepresenta maquinas Terminales. Se encarga de la generacion, envio y recepcion de paginas hacia y desde el router asociado a ella.  
   
3. *Pagina*: representa el objeto final que se quiere transportar mediante los routers.  

4. *Paquete*: medio de transporte de las paginas a traves de su division en estos segmentos.

5. *Administrador*: clase encargada de la continua actualizacion de los caminos optimos para dirigir a los paquetes en el menor tiempo hacia su destino final.

6. *Camnio*: establece el camino optimo para que cada paquete llegue a su destino en el menor tiempo.


### Clases Extra:  

Se nos proveyo el codigo de estas estructuras de datos para que sirvan como base del programa.

- *Lista*: estructura de datos que almacena diferentes datos en nodos y los une por medio de punteros.  
  
- *Cola*: hereda de lista pero la transforma en una estructura FIFO.  

### Etapas:

1) Creacion de los Routers segun la cantidad establecida. Se le pasa como argumento un numero ID y también una lista que contiene todos los paquetes en trafico en todos los routers (se agregan desde que se crean los paquetes de la pagina y se eliminan de la lista recien cuando llegan al router destino).

2) Se crean las Terminales segun la cantidad establecida. Se elige el router asociado por medio de una distribucion uniforme con los routers creados previamente. Si el router tiene mas del limite de maquinas asociadas, se repite el proceso hasta encontrar un router apto y luego se lo asocia a la maquina final. La terminal guarda un puntero a su router asociado y el router guarda un puntero a la terminal en su lista de terminales.

3) Se crean las conexiones entre los routers. Se empieza a recorrer la lista de routers y se selecciona una cantidad de vecinos para cada uno. Luego se procede a seleccionar un posible vecino teniendo en cuenta que la cantidad de vecinos de este no supere el maximo de vecinos posibles. Al ser declarado apto el vecino, se setean como vecinos mutuamente y se actualiza la Matriz de Adyacencia con un 1 en la columna y fila de ambos. Esto se hace con el fin de indicar que hay un camino posible entre ambos y en principio todos valen 1.

4) Se crean las paginas segun la cantidad determinada aleatoriamente. Luego se eligen las terminales origen y destino tambien aleatoriamente asegurandose de que sean diferentes. Se obtienen sus routers asociados y se utiliza la maquina origen para enviar a su router la pagina que previamente crea con un tamanio aleatorio.

5) Una vez que la pagina es enviada al router, este genera los paquetes y los guarda en una lista para enviar nuevamente al main.

6) El main le pide al router la lista y una vez que la tiene, con un objeto camino previamente instanciado le setea el camino mas corto a los paquetes utilizando el algoritmo de Dijkstra.

7) Cuando los paquetes estan listos, desde el main se le envia al router la lista de los paquetes con los caminos y este los envia directamente a el router vecino que corresponda. Este envio directo solo se hace desde el Router origen. Y se repite el proceso segun el numero de paginas creadas.

8) El vecino, al recibir el paquete debe determinar si es final(dirigido a una de sus maquinas terminales) o para un router vecino. Si es un paquete final, este lo guarda en una lista de la pagina correspondiente en la lista de cada terminal (ya que cada terminal puede tener varias paginas y por eso se guarda en una triple lista: lista de los paquetes de las paginas de las terminales) y una vez que esten todos los paquetes de esa pagina, los ordena, reconstruye la pagina y la envia a la terminal destino. Cuando el paquete es destinado a un vecino, este puede ir a la cola de trafico o la cola de espera, segun el ultimo paquete en la cola de trafico ya que no puede haber dos paquetes seguidos de la misma pagina. 

9) Luego se inician los ciclos (ciclo en el que un router envia a sus vecinos tantos paquetes de su respectiva cola de trafico como su ancho de banda le permita) de todos los routers. 

10) Cada dos ciclos, el administrador envia los paquetes en trafico al objeto camino para que reprograme sus caminos si es que hay una ruta con menos trafico. Para hacer esto primero se tiene que actualizar la Matriz de Adyacencia en el camino y esto se hace a traves de recorrer todos los routers y preguntar el turno(cuantos ciclos de demora hay si un paquete con direccion a ese router entrara ahora) de sus vecinos y plasmarlo en la matriz.

11) Se repite el proceso de los ciclos hasta que lleguen todos los paquetes o se alcance la cantidad establecida de ciclos.


### Diagrama de Clases:

### Diagramas de Flujo para algunos Metodos: 

recibirPaqueteVecino():

![image](https://github.com/Sofi268/AyEDFinal/assets/116833245/dcba6a17-33a0-4df1-b401-dbaa67c55444)

busquedaLista(): 

![image](https://github.com/Sofi268/AyEDFinal/assets/116833245/efe5a7ee-1e00-4a4d-a0b2-ffc306583dfb)


