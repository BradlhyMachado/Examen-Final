#include<iostream>
#include<thread>
#include<mutex>
#include <queue>

using namespace std;

// g++ monitor04.cpp -lpthread

#define NUM_HILOS 10

queue<char> colaElementos;
const char letras[26]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
int cont=0;
mutex flag;

class Monitor {
	public:
	void producir(int productor){
		flag.lock();	// LOCK
		int aleatorio;
		aleatorio = rand() % 26;
		char producido=letras[aleatorio];
		colaElementos.push(producido);
		cout<<"Productor ->"<<productor<<"<- produció: "<<producido<<endl;
		flag.unlock();	// UNLOCK		
	}
	
	void consumir(int consumidor){
	    flag.lock();	// LOCK
		int aleatorio;
		aleatorio = rand() % 26;
		char consumido=letras[aleatorio];
		colaElementos.pop();
		cout<<"Consumidor ->"<<consumidor<<"<- consumió: "<<consumido<<endl;
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

	Productor* productor[NUM_HILOS];
	Consumidor* consumidor[NUM_HILOS];
	Monitor* mo;
	
	int i;
	for(i=0; i<NUM_HILOS; i++) {
		productor[i] = new Productor(mo, i);
		consumidor[i] = new Consumidor(mo, i);
	}
	
	for(i=0; i<NUM_HILOS; i++) {
		productor[i]->join_thread();	//	START
	    consumidor[i]->join_thread();	//	START
	}

	return 0;
}
