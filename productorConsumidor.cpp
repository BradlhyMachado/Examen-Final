#include<iostream>
#include<thread>
#include<mutex>

using namespace std;

// g++ ProductoresConsumidores2.cpp -lpthread

#define NUM_HILOS 10

const char letras[26]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
int cont=0;
mutex flag;

class Monitor {
	public:
	void producir(int productor){
		//flag.lock();	// LOCK
		int aleatorio;
		for (int i=1; true; i++) {
		    aleatorio = rand() % 26;
		    char producido=letras[aleatorio];
		    cout<<"Productor ->"<<productor<<"<- produció: "<<producido<<endl;
		}
		//flag.unlock();	// UNLOCK		
	}
};

class Hilo {
	private:
		Monitor* monitor;
		thread t;
		int id;
		void run_thread(){			
			flag.lock();
			monitor->producir(id);		//	RUN
		    flag.unlock();
		}
			
	public:
		Hilo(Monitor* mon, int idd){
			id = idd;
			monitor = mon;
			t = thread(&Hilo::run_thread, this);
		}
		void join_thread(){
			t.join();	// START
		}
		
};

int main() {

	Hilo* hilos[NUM_HILOS];
	Monitor* mo;
	
	int i;
	for(i=0; i<NUM_HILOS; i++) {
		hilos[i] = new Hilo(mo, i);
	}
	
	for(i=0; i<NUM_HILOS; i++) {
		hilos[i]->join_thread();	//	START
	}

	return 0;
}
