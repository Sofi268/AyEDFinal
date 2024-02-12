#include <cstdlib>
#include <iostream>
#include <sstream>
#include <bitset>
#include <ctime>
#define INFI  9000
#define MIEMBRO   1
#define NO_MIEMBRO 0
const int cantidadRouters = 14;
const int cantidadTerminales = 33;

using namespace std;

//-------------------------------------------Lista Template-------------------------------------------------------
template <class T> class Nodo {
private:
    T dato;
    Nodo* next;
public:
    Nodo() { next = NULL; };
    Nodo(T a) { dato = a; next = NULL; };
    void set_dato(T a) { dato = a; };
    void setNext(Nodo* n) { next = n; };
    T getDato() { return dato; };
    Nodo* getNext() { return next; };
    bool es_vacio() { return next == NULL; }
};

template <class T> class Lista {
private: Nodo<T>* czo;
       void borrarD(T d, Nodo<T>* ant);
public:
    Lista() { czo = new Nodo<T>(); };
    Lista(Nodo<T>* n) { czo = n; };
    //~Lista(void);
    void add(T d); //sumar nodos a la lista
    bool esVacia(void);
    T cabeza(void); //retorna el dato del primer nodo
    Lista* resto(void); //retorna el puntero al "resto" de la lista
    int size();
    bool esta(T d);// detecta si d esta en la lista
    void borrarDato(T d) { borrarD(d, NULL); }//borra el nodo que contiene d
    void borrar(void); //borra la cabeza
    void borrarLast();//borra el ultimo
    void concat(Lista<T>* l1);// le transfiere los datos de l1 a this
    void tomar(int n);//deja "vivos" los n primeros nodos y borra el resto
    Lista<T>* copy(void);// hace una copia de la lista
    T last(); //retorna el dato del ultimo nodo
    void addLast(T d);
};

template <class T> void Lista<T>::add(T d) 
{
    Nodo<T>* nuevo = new Nodo<T>(d);
    nuevo->setNext(czo);
    czo = nuevo;
}

template <class T> bool Lista<T>::esVacia(void)
{
    return czo->es_vacio();
}

template <class T> T Lista<T>::cabeza(void)
{
    if (this->esVacia()) {
        throw std::runtime_error("Error: Esta lista vacía, no se puede obtener la cabeza");
    }
    return czo->getDato();
}

template <class T> Lista<T>* Lista<T>::resto(void)
{
    Lista* l = new Lista(czo->getNext());
    return (l);
}

template <class T> int Lista<T>::size()
{
    if (this->esVacia()) return 0;
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

template <class T> void Lista<T>::concat(Lista<T>* l1)
{// le transfiere los datos de l1 a this
    if (!(l1->esVacia())) {
        this->concat(l1->resto());
        this->add(l1->cabeza());
    }
}

template <class T> void Lista<T>::tomar(int n)
{ //deja "vivos" los n primeros nodos y borra el resto
    if (this->size() > n) {
        this->borrarLast();
        this->tomar(n);
    }
}

template <class T> Lista<T>* Lista<T>::copy(void)
{
    Lista<T>* aux = new Lista();
    aux->concat(this);
    return aux;
}

template <class T> bool Lista<T>::esta(T d)
{// busca d en la lista
    if (this->esVacia()) return false;
    if (this->cabeza() == d) return true;
    
    return this->resto()->esta(d);    
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

template <class T> T Lista<T>::last()
{
    if (!this->esVacia()) {
        if (this->resto()->esVacia())return this->cabeza();
        return this->resto()->last();
    } 
    else{
        return NULL;
    }
}
template <class T> void Lista<T>::addLast(T d)
{
    if (this->esVacia()) { // Si la lista está vacía, agregar al principio
        this->add(d);
    } else {
        Nodo<T>* nuevo = new Nodo<T>(d);
        Nodo<T>* temp = czo;
        while (temp->getNext() != NULL) {
            temp = temp->getNext();
        }
        temp->setNext(nuevo);
    }
}

//------------------------------------------------- Cola Template-------------------------------------------------

template <class T> class Cola :public Lista<T> {
private:
    int numRouter;
    int idAnterior; //Pagina a la que pertenece el ultimo elemento de la cola
    int numOrden; //Cantidad de elementos que tiene la cola
    int anchoBanda;
public:
    Cola(void) { Lista<T>(); };
    Cola(int numRout, int idAnt, int numOrden, int anchoBanda);
    ~Cola(void){};

    //Getters
    int getNumRout(void){return numRouter;};
    int getIdAnt(void){return idAnterior;};
    int getOrdenPagina(void){return numOrden;};
    int getAnchoDeBanda(void){return anchoBanda;};

    //Setters
    void setCantiPaqMas(void){numOrden++;};
    void setCantiPaqMenos(void){numOrden--;};

    T tope(void) { return this->last(); };
    bool colavacia(void) { return this->esVacia(); };
    void encolar(T a) { this->add(a); };
    void desencolar(void) { this->borrarLast(); };
    T ultimo(void) { return this->cabeza(); };
};

template <class T> Cola<T>::Cola(int numRout, int idAnt, int numOrdenCola, int anchoBanda)
{
    numRouter = numRout;
    idAnterior = idAnt;
    numOrden = numOrdenCola;
}

//----------------------------------------Pagina----------------------------------------------------------------

class Pagina {
private:
    int maqOrigen;
    int routOrigen;
    int maqFinal;
    int routFinal;
    int tamanioPagina;
    int numeroPagina;
    bitset<16> IPorigen;
    bitset<16> IPdestino;

public:
    Pagina(void){};
    Pagina(int maquinaOrigen, int routerOrigen, int maquinaFinal, int routFinal, int tamanio, int pagina, bitset<16> IpO, bitset<16> IpD);
    ~Pagina(void){};

    //Getters
    int getNumMaqOrigen(void){return maqOrigen;};
    int getRoutOrigen(){return routOrigen;};
    int getNumMaqFinal(void){return maqFinal;};
    int getRoutFinal(void){return routFinal;}
    int getTamanoPagina(void){return tamanioPagina;};
    int getNumeroPagina(void){return numeroPagina;};
    bitset<16> getIPorigen(void){return IPorigen;};
    bitset<16> getIPdestino(void){return IPdestino;};

    void imprimirPagina();
};

Pagina::Pagina(int maquinaOrigen, int routerOrigen, int maquinaFinal, int routerFinal,  int tamanio, int pagina, bitset<16> IpO, bitset<16> IpD)
{   
    maqOrigen = maquinaOrigen;
    routOrigen = routerOrigen;
    maqFinal = maquinaFinal;
    routFinal = routerFinal;
    tamanioPagina = tamanio;
    numeroPagina = pagina;
    IPorigen = IpO;
    IPdestino = IpD;
}

void Pagina::imprimirPagina()
{
    cout<<"Pagina numero "<<numeroPagina<<endl;
    cout<<"Maquina Origen: "<<maqOrigen<<endl;
    cout<<"Router Origen: "<<routOrigen<<endl;
    cout<<"Maquina Destino: "<<maqFinal<<endl;
    cout<<"Router Destino: "<<routFinal<<endl;
    cout<<"Tamanio de la pagina: "<<tamanioPagina<<endl;
    cout<<"Direccion de Origen: "<<IPorigen<<endl;
    cout<<"Direccion de Destino: "<<IPdestino<<endl;
}

//-----------------------------------------------Paquete--------------------------------------------------

class Paquete {
private:
    int maqOrigen, routOrigen, maqFinal, routFinal, routActual, tamanioPag, numeroPagina, numeroOrdenPagina, numeroOrdenCola;
    Lista<int>* camino;
    bitset<16> IPorigen;
    bitset<16> IPdestino;

public:
    Paquete(void){};
    Paquete(int maquinaOrigen, int routerOrigen, int maquinaFinal, int routerFinal, int routerActual, int tamanioPagina, int numPagina, int numOrdenPagina, bitset<16> IPorig, bitset<16> IPdest);
    ~Paquete(void){};

    //Getters
    int getMaquinaOrigen(void){ return maqOrigen;};
    int getRouterOrigen(void){return routOrigen;};
    int getMaquinaFinal(void){return maqFinal;};
    int getRouterFinal(void){return routFinal;};
    int getRoutActual(void){return routActual;};
    int getCantiTotalPaq(void){return tamanioPag;};
    int getNumPag(void){return numeroPagina;};
    int getOrdenPagina(void){return numeroOrdenPagina;};
    int getOrdenCola(void){return numeroOrdenCola;};
    bitset <16> getIPorigen(void){return IPorigen;};
    bitset <16> getIPdestino(void){return IPdestino;};
    Lista<int>* getCamino(void){return camino;};
    int getSigRouter(void);

    //Setters
    void setNuevoCamino(Lista<int>* nuevoCamino){camino = nuevoCamino;};
    void setRoutActual(int routerAct){routActual = routerAct;};
    void setOrdenCola(int nuevoNum){numeroOrdenCola = nuevoNum;};

    //Borra el primer elemento del camino
    void borrarElemCamino(void){camino->borrar();};

};

Paquete::Paquete(int maquinaOrigen, int routerOrigen, int maquinaFinal, int routerFinal, int routerActual, int tamanioPagina, int numPagina, int numOrdenPagina, bitset<16> IPorig, bitset<16> IPdest)
{
    maqOrigen = maquinaOrigen;
    routOrigen = routerActual;
    maqFinal = maquinaFinal;
    routFinal = routerFinal;
    routActual = routerActual;
    tamanioPag = tamanioPagina;
    numeroPagina = numPagina;
    numeroOrdenPagina = numOrdenPagina;
    numeroOrdenCola = 0;
    IPorigen = IPorig;
    IPdestino = IPdest;
}

//Al llegar un paquete al routerAEnviar, este se elimina de la lista de su camino por lo que el primer elemento del mismo va a ser el siguiente destino
int Paquete::getSigRouter()
{   
    int sigRout = camino->cabeza();
    return sigRout;
}
//---------------------------------------------------------Terminal--------------------------------------------------------------
template <class T> class Terminal  {
private:
    int numTerminal;
    int numRouter;
    T routerAsociado;
    bitset <16> direccionMaquina;
    Pagina* paginaEnviar;
    Lista<Pagina*>* paginasRecibidas;
    int numMaqGnral;

public:
    Terminal(void){};
    Terminal(int numeroMaquina, int numeroROuter, T router, int numMaqGeneral);

    //Getters
    int getNumeroTerminal(void){return numTerminal;};
    int getNumeroRouter(void){return numRouter;};
    T getRouterAsociado(void){return routerAsociado;};
    bitset<16> getDirecMaquina(void){return direccionMaquina;};
    int getNumMaqGeneral(void){return numMaqGnral;}

    bitset<16> setDireccion(int maq,  int rout);

    Pagina* generarPaginaAleatoria(Lista<T>* rout,int cantidadRout, int numPag, int numTerminalFinal, int numRoutFinal);
    int generarTamanioAleatorio(void);
    void enviarPagina(Lista<T>* rout,int cantidadRout, int numPag, int numMaqFinal, int numRoutFinal);
    void recibirPagina(Pagina* pag);
    void imprimirRecepcion(Pagina* pag);
};

template <class T> Terminal<T>::Terminal(int numeroMaquina, int numeroRouter, T router, int numMaqGeneral)
{
    numTerminal = numeroMaquina;
    numRouter = numeroRouter;
    routerAsociado = router;
    direccionMaquina = setDireccion(numeroMaquina, numeroRouter);
    paginasRecibidas = new Lista<Pagina*>;
    numMaqGnral = numMaqGeneral;
}

template<class T> bitset<16> Terminal<T>::setDireccion(int maq, int rout)
{
    bitset<16> a (maq);
    bitset<16> b (rout);
    bitset<16> direcMaquina;
    (a<<=8);
    direcMaquina = (a|b);
    return direcMaquina;
}

//Generar Pagina a enviar

template <class T> Pagina* Terminal<T>::generarPaginaAleatoria(Lista<T>* rout,int cantidadRout, int numPag, int numeroTerminalFinal, int numeroRoutFinal)
{   
    int tamanioPag = generarTamanioAleatorio();
    int numTerminalFinal = numeroTerminalFinal;
    int numRoutFinal = numeroRoutFinal;
    int routOrigen = numRouter;
    bitset<16> IPdest = setDireccion(numTerminalFinal, numRoutFinal);
    Pagina* nueva = new Pagina(numTerminal, routOrigen, numTerminalFinal,numRoutFinal, tamanioPag, numPag, direccionMaquina, IPdest);
    return nueva;
}

template<class T> int Terminal<T>::generarTamanioAleatorio(void)
{
    //Eleccion de 60 como tamanio maximo aleatorio
    srand(time(nullptr));
    int tamanio = rand()% 60;
    return tamanio;
}

//Crear la pagina y luego enviarla al router asociado
template<class T> void Terminal<T>::enviarPagina(Lista<T>* rout,int cantidadRout, int numPag, int numMaqFinal, int numRoutFinal)
{
    paginaEnviar = generarPaginaAleatoria(rout, cantidadRout, numPag, numMaqFinal, numRoutFinal);
    routerAsociado->recibirPagina(paginaEnviar);
}

//Recibir pagina enviada
template<class T> void Terminal<T>::recibirPagina(Pagina* pag)
{
    paginasRecibidas->add(pag);
    imprimirRecepcion(pag);
}

template<class T> void Terminal<T>::imprimirRecepcion(Pagina* pag)
{
    cout<<"La maquina "<<numTerminal<<" recibio exitosamente la pagina numero: "<<pag->getNumeroPagina()<<endl;
    cout<<"Direccion de envio: "<<pag->getIPorigen()<<endl;
    cout<<"Direccion de destino: "<<pag->getIPdestino()<<endl;
}

//----------------------------------------------------------Router-------------------------------------------------------
class Router{
private:
    int ID;
    int anchoBanda;
    int cantidadVecinos;
    int cantidadTerminales;
    Lista<Router*>* vecinos;
    Lista<Terminal<Router*>*>* terminales;
    Lista<Cola<Paquete*>*>* colasTrafico;
    Lista<Cola<Paquete*>*>* colasEspera;
    Lista<Lista<Lista<Paquete*>*>*>* listasTerminales;
    Lista<int>* veciConEspera;
    Lista<int>* veciConColaTrafico;
    Lista<int>* numVecinos;
    Lista<Paquete*>* listaVacia;
    Lista<Paquete*>* listaAEnviar;
    Lista<Paquete*>* listaConCaminos;
    Lista<Paquete*>* paquetes;

public:
    Router(void){};
    Router(int id, Lista<Paquete*>* paqs);
    ~Router(void){};

    //Gettters
    int getID(void){return ID;};
    int getAnchoBanda(void){return anchoBanda;};
    int getCantidadVecinos(void){return cantidadVecinos;};
    int getCantidadTerminales(void){return cantidadTerminales;};
    Lista<Router*>* getVecinos(void){return vecinos;};
    Lista<Terminal<Router*>*>* getTerminales(void){return terminales;};
    Lista<Cola<Paquete*>*>* getColasEspera(void){return colasEspera;};
    Lista<Cola<Paquete*>*>* getColasTrafico(void){return colasTrafico;};
    Lista<Lista<Lista<Paquete*>*>*>* getListasTerminales(void){return listasTerminales;};
    Lista<int>* getNumVecinos(void){return numVecinos;};
    Lista<Paquete*>* getListaAEnviar(void){return listaAEnviar;};

    //Setters
    void setAnchoBanda(int anchBandavoid);
    void setNuevoVecino(Router* vecino);
    void setNuevaTerminal(Terminal<Router*>* terminal);
    void setListaConCaminos(Lista<Paquete*>* listaCaminos){ listaConCaminos = listaCaminos;};

    //Busqueda
    static Router* buscarRouter(int ID,Lista<Router*>* vecinos);
    static Terminal<Router*>* buscarTerminal(int ID, Lista<Terminal<Router*>*>* terminales);
    Lista<Paquete*>* buscarLista(int ID, Lista<Lista<Lista<Paquete*>*>*>* listasTerminales, int numeroPag);
    Cola<Paquete*>* buscarCola(int ID, Lista<Cola<Paquete*>*>* colas);

    //Recibir Pagina y crear Paquetes
    void recibirPagina(Pagina* pagina);
    void armadoDePaquetes(Pagina* pagina);
    void enviarDirecto(Paquete* paquete);
    void addOrdenado(Paquete* paquete, Lista<Paquete*>* listaTerminal);
    Lista<Paquete*>* armadoLista(Lista<Paquete*>* listaTerminal, int numIndice, Paquete* paquete);

    //Reconstruccion y envio de pagina
    void recibirPaqueteFinal(Paquete* paquete);
    void reconstruirPagina(Lista<Paquete*>* paquetes);
    void recibirListaConCaminos(Lista<Paquete*>*);
    void enviarPagina(Pagina* pagina);

    //Recepcion de Paquetes y reenvio
    void recibirPaquete(Paquete* paquete); 
    void recibirPaqueteVecino(Paquete* paquete); //Uso gestion de vecinos
    void ciclo();
    bool pertenece(int numPag, int router);
    void enviarPaqueteVecino(Paquete* paquete);
    bool veciTieneEspera(int ID);
    bool veciTieneTrafico(int ID);

    void vaciarLista();
    void imprimirRecepcion(Paquete* paquete, bool final);
    int turno(int ID);
    void borrarPaquete(Paquete* paq);
    void agregarPaquete(Paquete* paq);
    void printVecinos(Lista<Router*>* vecis);
};

Router::Router(int id, Lista<Paquete*>* paqs)
{
    ID = id;
    cantidadVecinos = 0;
    cantidadTerminales = 0;
    vecinos = new Lista<Router*>();
    terminales = new Lista<Terminal<Router*>*>();
    colasTrafico = new Lista<Cola<Paquete*>*>();
    colasEspera = new Lista<Cola<Paquete*>*>();
    veciConColaTrafico = new Lista<int>();
    veciConEspera = new Lista<int>();
    listasTerminales = new Lista<Lista<Lista<Paquete*>*>*>();
    numVecinos = new Lista<int>();
    listaVacia = new Lista<Paquete*>();
    listaAEnviar = new Lista<Paquete*>();
    listaConCaminos = new Lista<Paquete*>();
    paquetes = paqs;
}

void Router::setAnchoBanda(int anchBanda)
{
    anchoBanda = anchBanda;
}

void Router::setNuevoVecino(Router* vecino)
{
    vecinos->add(vecino);
    numVecinos->add(vecino->getID());
    cantidadVecinos++;
}

void Router::printVecinos(Lista<Router*>* vecis)
{
    if(vecis->esVacia()) return;
    else{
        cout<<"Router vecino: "<<vecis->cabeza()->getID()<<endl;
        printVecinos(vecis->resto());
    }
    
}

void Router::setNuevaTerminal(Terminal<Router*>* terminal)
{
    terminales->add(terminal);
    cantidadTerminales++;
}

Router* Router::buscarRouter(int IDVecino, Lista<Router*>* vecinos)
{   
    if(vecinos->esVacia()) return nullptr;
    if(vecinos->cabeza()->getID() == IDVecino){
        cout<<"Vecino encontrado: "<<vecinos->cabeza()->getID()<<endl;
        return vecinos->cabeza();
    }
    if(vecinos->esVacia()) return nullptr;
    else return buscarRouter(IDVecino, vecinos->resto());
}

Terminal<Router*>* Router::buscarTerminal(int IDTerminal, Lista<Terminal<Router*>*>* terminales)
{
    if(terminales->esVacia()) return nullptr;
    if(terminales->cabeza()->getNumeroTerminal() == IDTerminal){
        return terminales->cabeza();
    }
    else return buscarTerminal(IDTerminal, terminales->resto());
}

Lista<Paquete*>* Router::buscarLista(int IDTerminal, Lista<Lista<Lista<Paquete*>*>*>* listasTerminales, int numeroPagina)
{
    if(listasTerminales->esVacia()) return nullptr;
    Lista<Lista<Paquete*>*>* listaTerminalesAux = listasTerminales->cabeza(); 

    if(listaTerminalesAux->cabeza()->cabeza()->getMaquinaFinal() == IDTerminal){
        int cantidadPaginas = listaTerminalesAux->size();
        for(int i=0; i<cantidadPaginas; i++){
            Lista<Paquete*>* listaBuscada = listaTerminalesAux->cabeza();
            if(listaBuscada->cabeza()->getNumPag()==numeroPagina){
                return listaBuscada;
            }
            else listaTerminalesAux = listaTerminalesAux->resto(); //Pasa a la siguiente pagina
        }
    }
    else return buscarLista(IDTerminal, listasTerminales->resto(), numeroPagina); //Pasa a la siguiente maquina
    return nullptr;
}

Cola<Paquete*>* Router::buscarCola(int IDCola, Lista<Cola<Paquete*>*>* colas)
{
    if(colas->esVacia()) return nullptr;
    Cola<Paquete*>* colaBuscada = colas->cabeza();
    if(colaBuscada->getNumRout() == IDCola) return colaBuscada;
    else return buscarCola(IDCola, colas->resto());
}

void Router::recibirPagina(Pagina* pagina)
{
    cout<<"Recibi la pagina"<<endl;
    cout<<"Direccion de origen: "<<pagina->getIPorigen()<<endl;
    cout<<"Direccion de envio: "<<pagina->getIPdestino()<<endl;
    armadoDePaquetes(pagina);
}

void Router::armadoDePaquetes(Pagina* pagina)
{
    int cantidadPaquetes = pagina->getTamanoPagina();
    for(int i= 0; i<cantidadPaquetes; i++){
        Paquete* paquete = new Paquete(pagina->getNumMaqOrigen(),pagina->getRoutOrigen(), pagina->getNumMaqFinal(), pagina->getRoutFinal(), ID, pagina->getTamanoPagina(), pagina->getNumeroPagina(), i+1, pagina->getIPorigen(),pagina->getIPdestino());
        Lista<int>* camino = new Lista<int>();
        camino->add(1);
        camino->add(2);
        paquete->setNuevoCamino(camino);
        enviarPaqueteVecino(paquete);
        //listaAEnviar->add(paquete);
    }
}
void Router::recibirListaConCaminos(Lista<Paquete*>* listaCaminos)
{
    setListaConCaminos(listaCaminos);
    for(int i = 0; i<listaCaminos->size(); i++){
        Paquete* paquete = listaConCaminos->cabeza();
        paquetes->add(paquete);
        enviarDirecto(paquete);
        listaConCaminos->borrar();
    }
    vaciarLista();
}
void Router::enviarDirecto(Paquete* paquete)
{
    int sigRouter = paquete->getSigRouter();
    buscarRouter(sigRouter, vecinos)->recibirPaquete(paquete);
}

void Router::reconstruirPagina(Lista<Paquete*>* paquetes)
{
    Paquete* paqAux = paquetes->cabeza();
    Pagina* pagina = new Pagina(paqAux->getMaquinaOrigen(), paqAux->getRouterOrigen(), paqAux->getMaquinaFinal(),paqAux->getRouterFinal(),paqAux->getCantiTotalPaq(),paqAux->getNumPag(),paqAux->getIPorigen(), paqAux->getIPdestino());
    enviarPagina(pagina);
}

void Router::enviarPagina(Pagina* pagina)
{
    int maqDestino = pagina->getNumMaqFinal();
    buscarTerminal(maqDestino, terminales)->recibirPagina(pagina);
}

void Router::recibirPaquete(Paquete* paquete)
{
    if(paquete->getRouterFinal()==ID){
        recibirPaqueteFinal(paquete);
    }
    else {
        cout<<"Hasta aca todo ok"<<endl;
        recibirPaqueteVecino(paquete);
    }
}

void Router::recibirPaqueteFinal(Paquete* paquete)
{
    borrarPaquete(paquete);
    int maqTerminal = paquete->getMaquinaFinal();
    int cantiTotalPaquetes = paquete->getCantiTotalPaq();
    int numPagina = paquete->getNumPag();
    Lista<Paquete*>* listaMaquina = buscarLista(maqTerminal, listasTerminales,numPagina);
    addOrdenado(paquete, listaMaquina);
    if(listaMaquina->size()==cantiTotalPaquetes)reconstruirPagina(listaMaquina);
    imprimirRecepcion(paquete, true);
}

void Router::addOrdenado(Paquete* paquete, Lista<Paquete*>* listaTerminal)
{   
    if (listaTerminal->esVacia()) {//el primero
        listaTerminal->add(paquete);
    }

    if(paquete->getOrdenPagina()>listaTerminal->last()->getOrdenPagina()){//el ultimo
        Lista<Paquete*>* paqueteAlFinal = new Lista<Paquete*>();
        paqueteAlFinal->add(paquete);
        listaTerminal->concat(paqueteAlFinal);
    }

    else{
        Lista<Paquete*>* listaAux = listaTerminal;
        for(int i = 0; i<listaAux->size() ; i++){
            if(listaAux->cabeza()->getOrdenPagina()>paquete->getOrdenPagina()){//entre Medio
                armadoLista(listaTerminal, i, paquete);
            }
            else listaAux = listaAux->resto();
        }
    }
}

Lista<Paquete*>* Router::armadoLista(Lista<Paquete*>* listaTerminal, int numIndice, Paquete* paquete)
{
    Lista<Paquete*>* listaAux1 = listaTerminal;
    listaAux1->tomar(numIndice);
    Lista<Paquete*>* listaAux2 = listaTerminal;
    for(int j = 0; j<numIndice; j++){
        listaAux2->borrar();
    }
    listaAux2->add(paquete);
    listaAux1->concat(listaAux2);
    return listaAux1;
}

void Router::recibirPaqueteVecino(Paquete* paquete)
{
    paquete->borrarElemCamino();
    int siguienteRout = paquete->getSigRouter();
    cout<<siguienteRout<<endl;

    if(veciTieneEspera(siguienteRout)){//Si existe una cola de espera para el trafico del router
        if(pertenece(paquete->getNumPag(), siguienteRout)){//Si pertenece a esa cola de espera
            Cola<Paquete*>* colaEsp= buscarCola(siguienteRout, colasEspera);//Se añade
            colaEsp->encolar(paquete);
            colaEsp->setCantiPaqMas();
        }
        else{
            Cola<Paquete*>* colaTraf = buscarCola(siguienteRout, colasTrafico);
            colaTraf->encolar(paquete);
            colaTraf->setCantiPaqMas();
            Paquete* paq = buscarCola(siguienteRout, colasEspera)->tope(); 
            colaTraf->encolar(paq); //Se encola tambien un paquete de la cola de espera
            colaTraf->setCantiPaqMas();
            Cola<Paquete*>* colaEspe = buscarCola(siguienteRout, colasEspera); //Se desencola de la lista de espera
            colaEspe->desencolar();
            colaEspe->setCantiPaqMenos();

            if(colaEspe->esVacia()){
                int numRouter = colaEspe->getNumRout();
                colasEspera->borrarDato(colaEspe);
                veciConEspera->borrarDato(numRouter);
            }
        }
    }

    else{

        if(veciTieneTrafico(siguienteRout)){
            int ant = buscarCola(siguienteRout, colasTrafico)->getIdAnt();

            if(paquete->getNumPag()==ant){
                int anchoBanda = buscarRouter(siguienteRout, vecinos)->getAnchoBanda();
                Cola<Paquete*>* colaEspera = new Cola<Paquete*>(siguienteRout,paquete->getNumPag(),1,anchoBanda);
                colaEspera->encolar(paquete);
                colasEspera->add(colaEspera);
                veciConEspera->add(siguienteRout);
            }

            else{
                Cola<Paquete*>* colaTraf = buscarCola(siguienteRout, colasTrafico);
                colaTraf->encolar(paquete);
                colaTraf->setCantiPaqMas();
            }
        }
        else{
            Router* veci = buscarRouter(siguienteRout,vecinos);
            int anchoDeBandaVecino = veci->getAnchoBanda();
            Cola<Paquete*>* colaTrafico = new Cola<Paquete*>(siguienteRout,paquete->getNumPag(),1, anchoDeBandaVecino);
            colaTrafico->encolar(paquete);
            colasTrafico->add(colaTrafico);
            veciConColaTrafico->add(siguienteRout);
        }
    }

    imprimirRecepcion(paquete, false);
}

void Router::ciclo()
{   
    Cola<Paquete*>* colaAux;
    Lista<Cola<Paquete*>*>* listaColasAux = colasTrafico;

    //Recorre la Lista
    for(int i = 0; i< colasTrafico->size(); i++){
        colaAux = listaColasAux->cabeza();

        //Recorre la cola
        for(int j = 0; j<colaAux->getAnchoDeBanda(); j++){
            Paquete* paquete= colaAux->tope();
            enviarPaqueteVecino(paquete);
            colaAux->desencolar();
            colaAux->setCantiPaqMenos();

            //Caso en que quede vacia la cola de Espera
            if(colaAux->esVacia()){
                int numRout = colaAux->getNumRout();
                colasTrafico->borrarDato(colaAux);
                veciConColaTrafico->borrarDato(numRout);
                break;
            }
        }
        listaColasAux = listaColasAux->resto();
    }
}

bool Router::pertenece(int numPag, int router)
{
    int ant = buscarCola(router, colasEspera)->getIdAnt();
    if(ant == numPag) return true;
    else return false;
}

bool Router::veciTieneEspera(int IDVeciEspera)
{   
    if(veciConEspera->esVacia()) return false;

    if(veciConEspera->esta(IDVeciEspera)){
        return true;
    }
    else return false;
}

bool Router::veciTieneTrafico(int IDVeciTrafico)
{
    if(veciConColaTrafico->esVacia())return false;

    if(veciConColaTrafico->esta(IDVeciTrafico)){
        return true;
    }
    else return false;
}

void Router::enviarPaqueteVecino(Paquete* paquete)
{
    int routDestino = paquete->getSigRouter();
    cout<<routDestino<<endl;
    printVecinos(vecinos);
    Router* destino = buscarRouter(routDestino, vecinos);
    cout<<"Hasta aca todo bien"<<endl;
    destino->recibirPaquete(paquete);
}

void Router::vaciarLista()
{
    listaAEnviar = listaVacia;
}

void Router::imprimirRecepcion(Paquete* paquete, bool final){
    cout<<"Se recibio paquete de pagina numero: "<<paquete->getNumPag()<<endl;
    if(final)cout<<"Final: si"<<endl;
    else cout<<"Final: no"<<endl;
    cout<<"Router receptor: "<<ID;
    cout<<"Emitido por maquina con direccion: "<<paquete->getIPorigen()<<endl;
    cout<<"Con destino maquina con direccion: "<<paquete->getIPdestino()<<endl;
}

int Router::turno(int ID)
{
    Cola<Paquete*>* colaVecino = buscarCola(ID, colasTrafico);
    if(colaVecino == nullptr)return 0;
    else{
        int anchoBanda = colaVecino->getAnchoDeBanda();
        int numOrden = colaVecino->getOrdenPagina();
        int ciclosEspera = 0;
        numOrden = numOrden - anchoBanda;
        
        while(numOrden>anchoBanda){
            ciclosEspera++;
            numOrden = numOrden - anchoBanda;
        }
        return ciclosEspera;
    }
}

void Router::borrarPaquete(Paquete* paq)
{
    paquetes->borrarDato(paq);
}

void Router::agregarPaquete(Paquete* paq)
{
    paquetes->add(paq);
}

//----------------------------------------------------------Extra------------------------------------------------------------
Router* buscarRouter(int numRout, Lista<Router*>* routers) 
{
    if(routers->esVacia()) return nullptr; 
    if(routers->cabeza()->getID() == numRout){
        return routers->cabeza();
    }
    else return buscarRouter(numRout, routers->resto());
}

Terminal<Router*>* buscarTerminal(int numTerminal, Lista<Terminal<Router*>*>* terminales)
{
    if (terminales->esVacia()) return nullptr;
    if (terminales->cabeza()->getNumMaqGeneral() == numTerminal) {
        return terminales->cabeza();
    } 
    else return buscarTerminal(numTerminal, terminales->resto());
}

//----------------------------------------------------------Main-------------------------------------------------------------
int main(){
    int matrizAdy[3][3];
    fill(*matrizAdy,*matrizAdy + 9,0);
    Lista<Paquete*>* paqs = new Lista<Paquete*>();
    Lista<Router*>* routs = new Lista<Router*>();
    Lista<Paquete*>* listaRecibida = new Lista<Paquete*>();

    Router* router1 = new Router(1, paqs);
    Router* router2 = new Router(2, paqs);
    Router* router3 = new Router(3, paqs);

    routs->add(router1);
    routs->add(router2);
    routs->add(router3);

    router1->setNuevoVecino(router2);
    router2->setNuevoVecino(router1);
    matrizAdy[0][1] = 1;
    matrizAdy[1][2] = 1;
    router2->setNuevoVecino(router3);
    router3->setNuevoVecino(router2);
    matrizAdy[1][2] = 1;
    matrizAdy[2][1] = 1;

    router1->setAnchoBanda(3);
    router2->setAnchoBanda(5);
    router3->setAnchoBanda(1);

    for(int i = 0; i<3; i++){
        for(int j = 0; j<3; j++){
            cout<<matrizAdy[i][j]<<" ";
        }
        cout<<"\n";
    }

    cout<<"Router numero: "<<router1->getID()<<" con ancho de banda "<<router1->getAnchoBanda()<<endl;
    cout<<"Router numero: "<<router2->getID()<<" con ancho de banda "<<router2->getAnchoBanda()<<endl;
    cout<<"Router numero: "<<router3->getID()<<" con ancho de banda "<<router3->getAnchoBanda()<<endl;

    Lista<Terminal<Router*>*>* terminales = new Lista<Terminal<Router*>*>();
    Terminal<Router*>* terminal1 = new Terminal<Router*>(1,1,router1,1);
    Terminal<Router*>* terminal2 = new Terminal<Router*>(2,1,router1,2);
    Terminal<Router*>* terminal3 = new Terminal<Router*>(1,2,router2,3);
    Terminal<Router*>* terminal4 = new Terminal<Router*>(2,2,router2,4);
    Terminal<Router*>* terminal5 = new Terminal<Router*>(1,3,router3,5);
    Terminal<Router*>* terminal6 = new Terminal<Router*>(2,3,router3,6);

    router1->setNuevaTerminal(terminal1);
    router1->setNuevaTerminal(terminal2);
    router2->setNuevaTerminal(terminal3);
    router2->setNuevaTerminal(terminal4);
    router3->setNuevaTerminal(terminal5);
    router3->setNuevaTerminal(terminal6);

    terminal6->enviarPagina(routs, 3, 1, 2, 1);

}
