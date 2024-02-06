## **Simulacion de Trafico de Internet** 

### Trabajo Final Algoritmos y Estructuras de Datos 

Desarrollo de un programa programa que simule el trafico de datos, al *estilo* del funcionamiento del Internet en C++ con Programacion Orientada a Objetos.  

Con este programa se pretende cumplir con la siguiente **consigna**:


- Existen **n maquinas** que cumplen la funcion de **routers** que se encargan de rutear los datos desde una maquina de origen hacia la maquina destino.
- Existen otras **k maquinas**, las **terminales**, que son emisoras-receptoras de paginas. Cada una de estas maquinas esta conectada a un unico router que es el encargado de enviar/recibir las paginas hacia/desde el destino final.

Ruteo de los datos: 

Cada router esta conectado a 1 o mas routers para transmitir los paquetes. 
Cada router sabe cuales son las maquinas terminales que tiene conectadas y cuales son los routers vecinos que tiene ( es consciente de sus conexiones directas).
Ademas cada router tienen una tabla que le indica a que router enciar los datos con un determinado destino.

Consideraciones de recepcion de paquetes:

1. Cuando un router va recibiendo de otro router paquetes con un determinado destino hay dos opciones:

  - Debe reenviarlo al router vecino correspondienteen la ruta.
  - Si el destino final es una maquina terminal conectada directamente.
    > Debe ir almacenando los paquetes recibidos hasta que esten todos los que correspondan a la pagina enviada.
    > Luego se rearma la pagina y recien ahi se envia a la maquina destino.


