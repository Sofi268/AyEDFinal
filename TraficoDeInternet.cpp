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
    void add(T d); 
    bool esVacia(void);
    T cabeza(void); 
    T* getCabeza(void);
    Lista* resto(void); 
    int size(void);
    void borrar(void); //borra la cabeza
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

template <class T> int Lista<T>::size()
{
    if (this->esvacia()) return 0;
    return 1 + this->resto()->size();
}

template <class T> void Lista<T>::borrar(void)
{ //borra el nodo cabeza
    if (!this->esVacia()) {
        Nodo<T>* tmp = czo;
        czo = czo->getNext();
        delete tmp;
    }
}

//------------------------------------------------- Cola Template-------------------------------------------------

template <class T> class Cola :public Lista<T> {
private:
    int numRouter;
public:
    Cola(void) { Lista<T>(); };
    ~Cola(void){};
    int getNumRout(){return numRouter;};
    T tope(void) { return this->last(); };
    bool colavacia(void) { return this->esVacia(); };
    void encolar(T a) { this->add(a); };
    void desencolar(void) { this->borrarLast(); };
    T ultimo(void) { return this->cabeza(); };
};

//-----------------------------------------------Paquete--------------------------------------------------

class Paquete{
private:
    int routerFinal, routerActual, routerOrigen;
    int maqFinal;
    Lista<int> camino;
    int numOrden;
    int numTotalPaquetes;
    bitset<16> IPorigen;
    bitset<16> IPdestino;
    bool terminal;
    int sigRout;
    int numOrdenCola;

public:
    Paquete(void){};
    Paquete(int routFnal, int maqFnal,int routerActual, int numeroOrden, int numeroTotalPaquetes,bitset<16> IPorigen,bitset<16> IPdest);
    ~Paquete(void){};

    int getRouterFinal(void){return routerFinal;};
    int getMaquinaFinal(void){return maqFinal;};
    int getSigRouter(void);
    int getCantiTotalPaq(){return numTotalPaquetes;};
    Lista<int> getCamino(void){return camino;};
    int getRoutActual(void){return routerActual;};
    int getOrdenCla(void){return numOrdenCola;};
    int getRouterOrigen(void){return routerOrigen;};
    bitset <16> getIPorigen(void){return IPorigen;};

    void setNuevoCamino(Lista<int> nuevoCamino);
    void setTerminal(void);
    void setRoutActual(int routAct);
    void setOrdenCola(int nuevoNum);

    void borrarElemCamino(void);

};

Paquete::Paquete(int routFnal, int maqFnal,int routActual,int numeroOrden, int numeroTotalPaquetes,bitset<16> IPorig, bitset<16> IPdest)
{
    routerFinal = routFnal;
    routerOrigen = routActual;
    maqFinal = maqFnal;
    routerActual = routActual;
    numOrden = numeroOrden;
    numTotalPaquetes = numeroTotalPaquetes;
    IPorigen = IPorig;
    IPdestino = IPdest;
    terminal = false;
    numOrdenCola = 0;
}

int Paquete::getSigRouter()
{   
    int sigRout = camino.cabeza();
    return sigRout;
}

void Paquete::setNuevoCamino(Lista<int> nuevoCamino)
{   
    camino = nuevoCamino;
}

void Paquete::setTerminal(){
    terminal = true;
}

void Paquete::setRoutActual(int routAct)
{
    routerActual = routAct;
}

void Paquete::setOrdenCola(int nuevoNum)
{
    numOrdenCola = nuevoNum;
}
void Paquete::borrarElemCamino()
{
    camino.borrar();
}

//----------------------------------------Pagina----------------------------------------------------------------

class Pagina {
private:
    int tamanioPagina;
    int maqFinal;
    int routerFinal;
    int routOrigen;
    bitset<16> IPorigen;
    bitset<16> IPdestino;

public:
    Pagina(void){};
    Pagina(int maquinaFinal, int routFinal,int routerOrigen, bitset<16> direcOrigen, int tamanio);
    ~Pagina(void){};

    int getTamanoPagina(void){return tamanioPagina;};
    bitset<16> getIPorigen(void){return IPorigen;};
    bitset<16> getIPdestino(void){return IPdestino;};
    int getNumMaqFinal(void){return maqFinal;};
    int getRoutFinal(void){return routerFinal;}
    int getRoutOrigen(){return routOrigen;};

    void setIPdestino(int maq, int rout);
};

Pagina::Pagina(int maquinaFinal, int routFinal,int routerOrigen, bitset<16> direcOrigen, int tamanio)
{   
    maqFinal = maquinaFinal;
    routerFinal = routFinal;
    routOrigen = routerOrigen;
    IPorigen = direcOrigen;
    tamanioPagina = tamanio;
    setIPdestino(maqFinal,routerFinal);
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
    T* routerAsociado;
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
    T* getRouterAsociado(){return routerAsociado;};

    void setDirecMaquina(int maq, int rout);
    void setPaginaRecibida(Lista<Paquete> paquetes);
    void setRouterAsociado(T* router);

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

template<class T> void Maquina<T>::setPaginaRecibida(Lista<Paquete> paquetes)
{   
    Paquete paq = paquetes.cabeza();
    Pagina* pag = new Pagina(numMaquina, numRouter,paq.getRouterOrigen(), paq.getIPorigen(), paq.getCantiTotalPaq());
    paginaRecibida = pag;
}

template<class T> void Maquina<T>::setRouterAsociado(T* router)
{
    routerAsociado = router;
}

template <class T> Pagina* Maquina<T>::generarPaginaAleatoria(Lista<T>* rout,int cantidadRout)
{   
    int tamanioPag = generarTamanioAleatorio();
    int numMaqFinal, numRoutFinal = generarDirecciondeEnvio(rout, cantidadRout);
    int routOrigen = numRouter;
    Pagina* nueva = new Pagina(numMaqFinal, numRoutFinal,routOrigen, direcMaquina, tamanioPag);
    Pagina pag = *nueva;
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
    int cantidadListTermi;
    Lista<Maquina<Router>> maqAsociadas;
    Lista<Router> routVecinos;
    Lista<Cola<Paquete>> colasVecinos;
    Lista<Lista<Paquete>> listasTerminales; 
    Lista<int> veciNum; //Lista de enteros con los numeros ID de los routers vecinos

public:
    Router();
    ~Router(){};

    int getIDRouter(void){return IDRout;};
    int getCantidadMaq(void){return cantidadMaquinas;};
    int getCantidadVeci(void){return cantidadVecinos;};
    Lista<int> getVecinos(){ return veciNum;};

    void setIDRouter(int num){IDRout = num;}
    void setNuevaMaq(void){cantidadMaquinas++;};
    void setNuevoVecino(void){cantidadVecinos++;};

    int generarAnchoBanda();
    void agregarVecino(Router vecino);
    void recibirPagina(Pagina pag);
    Paquete generarPaquete(int routerFinal, int maqFinal,int routActual, int numOrden, int numTot, bitset<16> IPorigen, bitset<16> IPdestino);
    void encolarPaquete(Paquete paq);
    void recibirPaquete(Paquete paq);
    void recibirPaqueteTerminal(Paquete paq);
    void recibirPaqueteVecino(Paquete paq);
    void ordenarPaqueteTerminal(Lista<Paquete> paquetes);
    void enviarPaquetes();
    void enviarPaqueteVecino(Lista<Paquete> paquetes);
    void enviarPaqueteTerminal(Lista<Paquete> paquetes);
    void borrarPaquete(Paquete paq);
    int turno(void);
};

Router::Router()
{
    //Setea valores por defecto
    IDRout = -1;
    cantidadMaquinas = 0;
    cantidadVecinos = 0;
    cantidadListTermi = 0;
    anchoBanda = generarAnchoBanda();
}

int Router::generarAnchoBanda()
{
    //Genera un Ancho de Banda aleatorio entre 1 y 5
    anchoBanda = rand() % 5;
}

void Router::agregarVecino(Router vecino)
{
    //Aniado el router a la Lista de vecinos
    routVecinos.add(vecino);
    setNuevoVecino();

    //Aniado el numero de router a la lista de enteros con los numeros de los routers vecinos
    int numVeci = vecino.getIDRouter();
    veciNum.add(numVeci);
}

void Router::recibirPagina(Pagina pag)
{   
    //Setear los atributos a cada paquete
    int numTotalPaquetes = pag.getTamanoPagina();
    int routerFinal = pag.getRoutFinal();
    int maqFinal = pag.getNumMaqFinal();
    int routActual = pag.getRoutOrigen();
    bitset<16> IPorigen = pag.getIPorigen();
    bitset<16> IPdestino = pag.getIPdestino();
    int numOrden = 0;
    
    for(int i = 0; i<numTotalPaquetes; i++){
        
        //Generar paquetes
        Paquete paquete = generarPaquete(routerFinal, maqFinal,routActual, numOrden, numTotalPaquetes, IPorigen, IPdestino);
        
        //Setear camino del paquete
        
        //Encolar paquetes
        encolarPaquete(paquete);
        numOrden++;
    }
}

Paquete Router::generarPaquete(int routerFinal, int maqFinal,int routActual, int numOrden, int numTot, bitset<16> IPorigen, bitset<16> IPdestino)
{   
    Paquete* paquete = new Paquete(routerFinal, maqFinal, routActual, numOrden,numTot,IPorigen, IPdestino);
    return *paquete;
}

void Router::encolarPaquete(Paquete paq) //Recorrer la lista de colas hasta encontrar el router que corresponde
{   
    //Set orden de cola
    int sigRout = paq.getSigRouter();
    Lista<Cola<Paquete>> aux = colasVecinos;

    for (int i= 0; i<cantidadVecinos; i++){
        int numActual = aux.getCabeza()->getNumRout();
        if(numActual==sigRout){
            aux.getCabeza()->add(paq);
            break;
        }
        aux = *aux.resto();
    } 
}

void Router::recibirPaquete(Paquete paq)
{
    paq.setRoutActual(IDRout);
    paq.borrarElemCamino();

    //Determino si el paquete es terminal o vecino
    if(paq.getCamino().size()==0){
        recibirPaqueteTerminal(paq);
    }
    else{
        recibirPaqueteVecino(paq);
    }
}

void Router::recibirPaqueteTerminal(Paquete paq)
{
    //Aniado el paquete a la Lista Terminal correspondiente
    Lista<Lista<Paquete>> listTerminalesAux = listasTerminales;
    Lista<Paquete>* lisAux;
    int numMaq;
    //if(paq.getMaquinaFinal())
    for(int i = 0; i < ; i++){
        numMaq = 
        if(paq.getMaquinaFinal()==numMaq)
    }
    listaTerminal.add(paq);
    int tamanioPag = paq.getCantiTotalPaq();
    int tamanioActual = listaTerminal.size();

    //Compruebo si llegaron todos los paquetes
    //En caso de que si, los ordeno
    if(tamanioPag == tamanioActual) ordenarPaqueteTerminal(listaTerminal);
}

void Router::recibirPaqueteVecino(Paquete paq)
{
    //Setear paq.setOrdenCola;
}

void Router::ordenarPaqueteTerminal(Lista<Paquete> paquetes)
{
    //Ordenar la listaTerminal cuando este completa
    // enviarPaqueteTerminal(paquetes);
}

void Router::enviarPaqueteVecino(Lista<Paquete> paquetes)
{
    //Recorrer lista de Maquinas asociadas hasta encontrar la correspondiente
    Lista<Maquina<Router>> maqsAux= maqAsociadas;
    int numMaqFNal = paquetes.cabeza().getMaquinaFinal();
    int maq;
    for(int i = 0; i < cantidadMaquinas; i++){
        maq = maqsAux.cabeza().getMaquina();
        //Una vez hallada, se envia la lista de paquetes previamente ordenados
        if(maq==numMaqFNal){
            maqsAux.getCabeza()->setPaginaRecibida(paquetes);
            break;
        }
        maqsAux = *maqsAux.resto();
    }
}

void Router::enviarPaqueteTerminal(Lista<Paquete> paquetes)
{
    //Enviar paquete a la maquina final
    Maquina<Router>* maq;
    //borrarPaquete()

}

void Router::borrarPaquete(Paquete paq)
{
    //Borrar de la cola correspondiente
}

int Router::turno()
{
    //Cuanto le falta al paquete para que sea el turno de su cola para enviarse
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

//----------------------------------Metodos extra-----------------------------------------------

Router* buscar(int numRout, Lista<Router> routers)
{
    Router* buscado; 
    for(int i = 0; i < routers.size(); i++){
        if(numRout == routers.cabeza().getIDRouter()){
            buscado = &routers.cabeza();
        }
        else buscar(numRout, *routers.resto());
    }
    return buscado;
}

void enviarPagina(Pagina pag, Router* routAsociado)
{
    routAsociado->recibirPagina(pag);
}

//----------------------------------------------- Main-----------------------------------------------------

int main(){
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
        Router* routerAsociado = aux.getCabeza();
        maquina->setRouterAsociado(routerAsociado); //Asocio el router a la maquina
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

    //Mejorar
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
    Lista<Maquina<Router>> aux3 = maquinas;
    Lista<Router> *prout = &rout;

    for(int i=0; i<pagsCanti; i++){
        //Seleccion de maquina generadora aleatoria
        int numMaquinaAleat = rand() % cantidadMaquinas;
        for(int i = 0; i < cantidadMaquinas; i++){
            int numMaqActual = aux3.cabeza().getMaquina();
            if(numMaqActual== numMaquinaAleat){
                Maquina<Router>* maquina = aux3.getCabeza();
                Pagina* pagina = maquina->generarPaginaAleatoria(prout, cantidadRouters);
                Router* routAsociado = maquina->getRouterAsociado();
                //Envio de pagina
                enviarPagina(*pagina, routAsociado);
                break;
            }
            aux3 = *aux3.resto();
        }
    }

    //Designar mejores rutas para los paquetes
    

    //Crear admin del sistema 

    //Reconfigurar rutas de los paquetes
    //Bucle de envios hasta que no haya mas paquetes en ningun router

}
