#include <cstdlib>
#include <iostream>
#include <sstream>
#include <bitset>
#define INFI  9000
#define MIEMBRO   1
#define NO_MIEMBRO 0
const int cantidadRouters = 14;
const int cantidadMaquinas = 33;

using namespace std;

//-------------------------------------------Lista Template-------------------------------------------------------

template <class T> class Nodo {
private:
    T dato;
    Nodo* next;
public:
    Nodo(void) { next = NULL; };
    Nodo(T a) { dato = a; next = NULL; };
    void setDato(T a) { dato = a; };
    void setNext(Nodo* n) { next = n; };
    T getDato(void) { return dato; };
    Nodo* getNext(void) { return next; };
    bool esVacio(void) { return next == NULL; }
};

template <class T> class Lista {
private: Nodo<T>* czo;
       void borrarD(T d, Nodo<T>* ant);
public:
    Lista(void) { czo = new Nodo<T>(); };
    Lista(Nodo<T>* n) { czo = n; };
    ~Lista(void){};
    void add(T d); //sumar nodos a la lista
    bool esVacia(void);
    T cabeza(void); //retorna el dato del primer nodo
    T* getCabeza(void);
    Lista* resto(void); //retorna el puntero al "resto" de la lista
    int size(void);
    void borrarDato(T d) { borrarD(d, NULL); }//borra el nodo que contiene d
    void borrar(void); //borra la cabeza
    void borrarLast(void);//borra el ultimo
};

template <class T> void Lista<T>::add(T d) 
{
    Nodo<T>* nuevo = new Nodo<T>(d);
    nuevo->setNext(czo);
    czo = nuevo;
}

template <class T> bool Lista<T>::esVacia(void)
{
    return czo->esVacio();
}

template <class T> T Lista<T>::cabeza(void)
{
    if (this->esVacia()) {
        throw std::runtime_error("Error: Lista vacía, no se puede obtener la cabeza");
    }
    return czo->getDato();
}

template <class T> T* Lista<T>::getCabeza(void)
{   
    if (this->esVacia()) {
        throw std::runtime_error("Error: Lista vacía, no se puede obtener la cabeza");
    }
    return czo;

}

template <class T> Lista<T>* Lista<T>::resto(void)
{
    Lista* l = new Lista(czo->getNext());
    return (l);
}

template <class T> void Lista<T>::borrar(void)
{ //borra el nodo cabeza
    if (!this->esVacia()) {
        Nodo<T>* tmp = czo;
        czo = czo->getNext();
        delete tmp;
    }
}

template <class T> void Lista<T>::borrarLast()
{ // borra el ultimo nodo
    if (!this->esVacia()) {
        if ((czo->getNext())->getNext() == NULL) {
            delete czo->getNext();
            czo->setNext(NULL);
        }
        else this->resto()->borrarLast();
    }
}

template <class T> void Lista<T>::borrarD(T d, Nodo<T>* ant)
{
    if (!this->esVacia()) {
        if (d == this->cabeza()) {
            if (ant == NULL) {//al principio
                this->borrar();
            }
            else {//entre medio
                ant->setNext(czo->getNext());
                delete czo;
            }
        }
        else  this->resto()->borrarD(d, czo);

    }
}

//------------------------------------------------- Cola Template-------------------------------------------------

template <class T> class Cola :public Lista<T> {
private:
    int numRouter;
    bool propio;
public:
    Cola(void) { Lista<T>(); };
    ~Cola(void){};
    T tope(void) { return this->last(); };
    bool colavacia(void) { return this->esVacia(); };
    void encolar(T a) { this->add(a); };
    void desencolar(void) { this->borrarLast(); };
    T ultimo(void) { return this->cabeza(); };
};

//-----------------------------------------------Paquete--------------------------------------------------

class Paquete{
private:
    int routerFinal;
    int maqFinal;
    Lista<int> camino;
    int numOrden;
    int numTotalPaquetes;
    bitset<16> IPorigen;
    bitset<16> IPdestino;
    bool terminal;
    int sigRout;
public:
    Paquete(void){};
    Paquete(int routFnal, int maqFnal,int numeroOrden, int numeroTotalPaquetes,bitset<16> IPorigen,bitset<16> IPdest);
    ~Paquete(void){};
    int getRouterFinal(void){return routerFinal;};
    int getMaquinaFinal(void){return maqFinal;};
    Lista<int> getCamino(void){return camino;};
    void setCamino(Lista<int> nuevoCamino);
    int getSigRouter(void);
    void setTerminal(void);
};

Paquete::Paquete(int routFnal, int maqFnal,int numeroOrden, int numeroTotalPaquetes,bitset<16> IPorig, bitset<16> IPdest)
{
    routerFinal = routFnal;
    maqFinal = maqFnal;
    numOrden = numeroOrden;
    numTotalPaquetes = numeroTotalPaquetes;
    IPorigen = IPorig;
    IPdestino = IPdest;
    terminal = false;
}
void Paquete::setCamino(Lista<int> nuevoCamino)
{
    camino = nuevoCamino;
}

int Paquete::getSigRouter()
{
    //Devuelve el siguiente router de su camino
}

void Paquete::setTerminal(){
    terminal = true;
}

//----------------------------------------Pagina----------------------------------------------------------------

class Pagina {
private:
    int tamanioPagina;
    int maqFinal;
    int routerFinal;
    bitset<16> IPorigen;
    bitset<16> IPdestino;
public:
    Pagina(void){};
    Pagina(int maquinaFinal, int routFinal, bitset<16> direcOrigen, int tamanio);
    ~Pagina(void){};
    int getTamanoPagina(void){return tamanioPagina;};
    bitset<16> getIPorigen(void){return IPorigen;};
    bitset<16> getIPdestino(void){return IPdestino;};
    int getNumMaqFinal(void){return maqFinal;};
    int getRoutFinal(void){return routerFinal;}
    void setIPdestino(int maq, int rout);
    Paquete* generarPaquete(void);
};

Pagina::Pagina(int maquinaFinal, int routFinal, bitset<16> direcOrigen, int tamanio)
{   
    maqFinal = maquinaFinal;
    routerFinal = routFinal;
    IPorigen = direcOrigen;
    tamanioPagina = tamanio;
    setIPdestino(maqFinal,routerFinal);
}

Paquete* Pagina::generarPaquete(void)
{   
    int numOrden,numTotalPaquetes;
    //Dar valores correctos a las var numOrden,numTotalPaquetes

    Paquete* paquete = new Paquete(routerFinal, maqFinal, numOrden,numTotalPaquetes,IPorigen, IPdestino);
    return paquete;
}

void Pagina::setIPdestino(int maq, int rout)
{
    bitset<16> a(maq);
    bitset<16> b(rout);
    (a<<=8);
    IPdestino = (a&b);
}

//-----------------------------------------Maquina Template---------------------------------------------------------------

template <class T> class Maquina{
private:
    int numMaquina;
    int numRouter;
    bitset <16> direcMaquina;
    Pagina paginaEnviar;
    Pagina paginaRecibida;
public:
    Maquina(void){};
    Maquina(int maq, int rout);
    ~Maquina(void){};
    int getMaquina(void){return numMaquina;};
    int getRouter(void){return numRouter;};
    bitset <16> getDirec(void){return direcMaquina;};
    void setDirecMaquina(int maq, int rout);
    void setPaginaEnviar(Pagina pag);
    void setPaginaRecibida(Pagina p);
    Pagina* generarPaginaAleatoria(Lista<T>* rout,int cantidadRout);
    int generarTamanioAleatorio(void);
    int generarDirecciondeEnvio(Lista<T>* rout,int cantidadRout);
    int generarMaquina(int randRout,Lista<T> *rout);
};

template<class T> Maquina<T>::Maquina(int maq, int rout)
{
    numMaquina = maq;
    numRouter = rout;
    setDirecMaquina(maq, rout);
}

template<class T> void Maquina<T>::setDirecMaquina(int maq, int rout)
{
    bitset<16> direc;
    bitset<16> a(maq);
    bitset<16> b(rout);
    (a<<=8);
    direcMaquina = (a&b);
}

template<class T> void Maquina<T>::setPaginaEnviar(Pagina pag)
{   
    paginaEnviar = pag;
}

template<class T> void Maquina<T>::setPaginaRecibida(Pagina pag)
{   //Completar el metodo
    paginaRecibida = pag;
}

template <class T> Pagina* Maquina<T>::generarPaginaAleatoria(Lista<T>* rout,int cantidadRout)
{   
    int tamanioPag = generarTamanioAleatorio();
    int numMaqFinal, numRoutFinal = generarDirecciondeEnvio(rout, cantidadRout);
    Pagina* nueva = new Pagina(numMaqFinal, numRoutFinal, direcMaquina, tamanioPag);
    Pagina pag = *nueva;
    setPaginaEnviar(pag);
    return nueva;
}

template<class T> int Maquina<T>::generarTamanioAleatorio(void)
{
    int tamanio = rand()% 100;
    return tamanio;
}

template<class T> int Maquina<T>::generarDirecciondeEnvio(Lista<T>* rout, int cantidadRout)
{
    int randMaq,randRout;
    randRout = rand()% cantidadRout;
    randMaq = generarMaquina(randRout, rout);
    return randMaq,randRout;
}

template<class T> int Maquina<T>::generarMaquina(int randRout,Lista<T> *rout)
{   
    if(rout->numRout==randRout){
        int randMaq = rand()%(rout->cantidadMaquinas); 
        return randMaq;
    }
    else return generarMaquina(randRout, rout->getNext());
}


//---------------------------------------------------------Router-----------------------------------------------

class Router{
private:
    int IDRout;
    int cantidadMaquinas;
    int cantidadVecinos;
    int anchoBanda;
    Lista<Maquina<Router>> maqAsociadas;
    Lista<Router> routVecinos;
    Lista<Cola<Paquete>> colasVecinos;
    Lista<Paquete> listaTerminal;
    Lista<int> veciNum;

public:
    Router();
    ~Router(){};
    int getIDRouter(void){return IDRout;};
    int getCantidadMaq(void){return cantidadMaquinas;};
    int getCantidadVeci(void){return cantidadVecinos;};
    Lista<int> getVecinos(){ return veciNum;};
    int generarAnchoBanda();
    void setIDRouter(int num){IDRout = num;}
    void setNuevaMaq(void){cantidadMaquinas++;};
    void setNuevoVecino(void){cantidadVecinos++;};
    void agregarVecino(Router vecino);
    void recibirPaquete(Paquete paq);
    void ordenarPaqueteFinal();
    void enviarPaquete();
    void enviarPaqueteVecino();
    void enviarPaqueteFinal();
    void borrarPaquete(Paquete paq);
};

Router::Router()
{
    IDRout = -1;
    cantidadMaquinas = 0;
    cantidadVecinos = 0;
    anchoBanda = generarAnchoBanda();
}

void Router::recibirPaquete(Paquete paq)
{
    //final o vecino
}

void Router::ordenarPaqueteFinal()
{
    //Ordenar la listaTerminal cuando este completa
}

void Router::enviarPaquete()
{
    //final o vecino
}

void Router::enviarPaqueteVecino()
{
    //Enviar y borrar despues
}

void Router::enviarPaqueteFinal()
{
    //Enviar paquete a la maquina final
}

int Router::generarAnchoBanda()
{
    //Generar aleatoriamente un ancho de banda para el router
}

void Router::agregarVecino(Router vecino)
{
    routVecinos.add(vecino);
    setNuevoVecino();
    int numVeci = vecino.getIDRouter();
    veciNum.add(numVeci);
}

void Router::borrarPaquete(Paquete paq)
{
    //Borrar de la cola correspondiente
}

//----------------------------------------------Admin de Sistema---------------------------------------------
class AdminSistema{

};

//----------------------------------------------Grafo: Dijkstra----------------------------

int *dijkstra(int C[][cantidadRouters],int s, int t, int Pre[])
{
    static int D[cantidadRouters];
    int S[cantidadRouters];
    int actual, i, k, b;
    int menordist, nuevadist;

    for(i=0;i<cantidadRouters;i++){
                            S[i]=NO_MIEMBRO;
                            D[i]=INFI;
                            Pre[i]= -1;
    }
    S[s]=MIEMBRO; D[s]=0; actual=s; b=1; k=0;
    while((actual!=t)&&(b==1)){
                    b=0;
                    menordist=INFI;
                    for(i=0;i<cantidadRouters;i++){             
                            if(S[i]==NO_MIEMBRO){
                                    nuevadist=D[actual]+C[actual][i];
                                    if(nuevadist<D[i]){
                                        D[i]=nuevadist;
                                        Pre[i]=actual; b=1;
                                    }
                                    if(D[i]<menordist){
                                        menordist=D[i];
                                        k=i;  b=1;
                                    }
                            }
                    }                               
                    actual=k; 
                    S[actual]=MIEMBRO;
                    printf("\n\n         D     S     Pre");
                    for(i=0;i<cantidadRouters;i++){
                            printf("\n[%2i] %5i %5i %5i     ",i,D[i], S[i],Pre[i]);
                    } 
    }
    return D;
}

void camino(int P[], int s, int t)
{  if (t==s) cout<< s<<"  ";
   else{
        camino(P,s,P[t]);
        cout<<t<<"  ";
   } 
}

Router* buscar(int numRout, Lista<Router> routers){
    Router* buscado; 
    for(int i = 0; i < routers.size(); i++){
        if(numRout == routers.cabeza().getIDRouter()){
            buscado = routers.getCabeza();
        }
        else buscar(numRout, *routers.resto());
    }
    return buscado;
}

//----------------------------------------------- Main-----------------------------------------------------

int main(){
    int posible = 0;
    int division = 4; //Cantidad por cada division 
    int anchoBanda = 5;
    Lista<Router> rout;
    Lista<Maquina<Router>> maquinas;
    AdminSistema administrador;
    Lista<Paquete> paquetes;
    int pagsCanti = rand() % 50;
    int baseCantidadVeci = 4;
    int limInferior = 0;
    Lista<Router> aux;
    Lista<int> aux2;
    int matrizAdy[cantidadRouters][cantidadRouters];
    int *pdist, i, j, P[cantidadRouters],s,t;
    
    //Creacion de los routers

    for(int i = 0; i < cantidadRouters; i++){
        Router* router = new Router();
        router->setIDRouter(++i);
        rout.add(*router);
    }
    
    //Creacion de maquinas

    for(int i = 0; i < cantidadMaquinas; i++){
        int numMaq,numRout;
        int idRout;
        int idCantMaq;
        bool apto = false;
        aux = rout;
        //Agregar metodo buscar
        while(!apto){ //Bucle para seleccionar router aleatorio con cantidad de maquinas disponibles            
            numRout = rand() % cantidadRouters; //Eleccion de Router Aleatorio
            for(int j = 0; j < cantidadRouters; j++){ //Recorre la lista de Routers hasta encontrar el router aleatorio obtenido
                idRout = aux.cabeza().getIDRouter();
                if(numRout == idRout){ //Caso en que se encuentra el router correspondiente
                    idCantMaq = aux.cabeza().getCantidadMaq();//Obtener la cantidad de Maquinas asociada al router aleatorio obtenido
                    if(idCantMaq < cantidadRouters){  //Comprobar si la cantidad de maquinas es menor a la maxima posible
                        apto = true;
                        aux.cabeza().setNuevaMaq(); //Actualizo la cantidad de maquinas del router
                        break;
                    }
                }
                aux = *aux.resto(); //No es el router o la cantidad de maquinas del router elegido es maxima, asi que continua con el siguiente
            }
        }
        numRout = idRout;
        numMaq = ++idCantMaq;
        Maquina<Router>* maquina= new Maquina<Router>(numMaq,numRout);
        maquinas.add(*maquina);
    }

    //Creacion de conexiones entre los Routers
    //Dos bucles: el primero para elegir los routers vecinos y guardarlos, y el segundo para guardar en los vecinos ese router
    
    for(int i = 0; i<cantidadRouters; i++){ //i representa el numero del router al que se le esta designando los vecinos
        Router* actual;
        Router* vecino;
        int cantVeciAleat = (rand() % baseCantidadVeci) + limInferior;
        aux = rout;
        for(int j = 0; j<cantVeciAleat; j++){
            int numRoutVeci = rand() % (cantidadRouters - 1);

            actual = buscar(i , rout);
            actual->agregarVecino(*vecino);

            vecino = buscar(numRoutVeci , rout);
            vecino->agregarVecino(*actual);
        }
        limInferior++;
    }
    
    //Crear Matriz de Adyacencia entre ellos

    aux = rout;

    for(int i = 0; i<cantidadRouters; i++){  //Agrego las relaciones directas a la matriz
        aux2 = aux.cabeza().getVecinos();
        for(int j = 0; j < aux.cabeza().getVecinos().size(); j++){  
            int k = aux2.cabeza();
            matrizAdy[i][k] = 1;
        }
        aux = *aux.resto();
    }

    for(int i = 0; i < cantidadRouters; i++){  //Relleno el resto de relaciones
        for(int j = 0; j < cantidadRouters; j++){
            if(matrizAdy[i][j]!=1) matrizAdy[i][j] = INFI;
        }
    }


    //Crear paginas
    
    for(int i=0; i<pagsCanti; i++){
        Pagina* pag = new Pagina();
        int cantidadPaquetes = rand() % 20;
        for(int i = 0; i<cantidadPaquetes; i++){
            Paquete paq = *pag->generarPaquete();
            paquetes.add(paq);
        }
        
    }

    //Crear admin del sistema 

    //Reconfigurar rutas de los paquetes

}
