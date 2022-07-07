#include<iostream>
#include<thread>
#include<mutex>
#include <queue>

using namespace std;

// g++ productorConsumidorFinal.cpp -lpthread

#define NUM_PRODUC 1
#define NUM_CONSUM 3

queue<char> colaElementos;	// Cola de almacenamiento de los datos
const char letras[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};	//Lista de datos
int cont = 0;
int estres =0;			//Contador de estres
int producido = 0;		//Contador de datos producidos
int consumido = 0;		//Contador de datos consumidos
mutex flag;			//Para el manejo de procesos e interbloqueos

class Monitor {
	public:
	//Método producir del monitor
	void producir(int productor){
		flag.lock();	// LOCK
		producido++;
		int aleatorio = rand() % 26;                //Elemento útil para generar la letra random
		colaElementos.push(letras[aleatorio]);      //Ingresamos a la cola la letra random
		//cout<<"Productor "<<productor<<" \t Produció: "<<colaElementos.back()<<" \t Tamaño cola: "<<colaElementos.size()<<endl;
		cout<<"Producido "<<producido<<" \t Consumido: --"<<" \t Desperdicio: --"<<" \t Tamaño cola: "<<colaElementos.size()<<endl;
		flag.unlock();	// UNLOCK		
	}
	//Método consumir del monitor
	void consumir(int consumidor){
	    flag.lock();	// LOCK
		if(!colaElementos.empty()){                 //Mientras la cola no se encuentre vacía imprimir el último elemento
    		consumido++;
    		//cout<<"Consumidor "<<consumidor<<" \t Consumió: "<<colaElementos.front()<<" \t Tamaño cola: "<<colaElementos.size()<<endl;
    		cout<<"Producido: --"<<" \t Consumido: "<<consumido<<" \t Desperdicio: --"<<" \t Tamaño cola: "<<colaElementos.size()<<endl;
    		colaElementos.pop();                    //Eliminar de la cola el elemento final
		    estres--;
		}
		else{                                        //Si la cola se encuentre vacía consumidor estresado
		    estres++;
		    //cout<<"Consumidor "<<consumidor<<" \t Consumió: ESTRES"<<" \t Tamaño cola: "<<colaElementos.size()<<endl;
		    cout<<"Producido: --"<<" \t Consumido: ESTRES"<<" \t Desperdicio: "<<estres<<" \t Tamaño cola: "<<colaElementos.size()<<endl;
		}
		flag.unlock();	// UNLOCK		
	}
};


class Productor {
	private:
		Monitor* monitor;
		thread t;
		int id;
		void run_thread(){			
			for (int i=1; true; i++) {
			    monitor->producir(id);		//	RUN
			}
		}
			
	public:
		Productor(Monitor* mon, int idd){
			id = idd;
			monitor = mon;
			t = thread(&Productor::run_thread, this);
		}
		void join_thread(){
			t.join();	// START
		}
		
};


class Consumidor {
	private:
		Monitor* monitor;
		thread t;
		int id;
		void run_thread(){			
			for (int i=1; true; i++) {
			    monitor->consumir(id);		//	RUN
			}
		}
			
	public:
		Consumidor(Monitor* mon, int idd){
			id = idd;
			monitor = mon;
			t = thread(&Consumidor::run_thread, this);
		}
		void join_thread(){
			t.join();	// START
		}
};


int main() {
	Productor* productor[NUM_PRODUC];        //Instancio una lista de productores de tamaño NUM_HILOS 
	Consumidor* consumidor[NUM_CONSUM];      //Instancio una lista de consumidores de tamaño NUM_HILOS 
	Monitor* mo;                            //Instancio nuestra clase monitor para controlar los estádos críticos
	
	for(int i=0; i<NUM_PRODUC; i++) {
		productor[i] = new Productor(mo, i);
	}
	for(int i=0; i<NUM_CONSUM; i++) {
		consumidor[i] = new Consumidor(mo, i);
	}
	
	for(int i=0; i<NUM_PRODUC; i++) {
		productor[i]->join_thread();	//	START
	}
	for(int i=0; i<NUM_CONSUM; i++) {
	    consumidor[i]->join_thread();	//	START
	}
	return 0;
}
