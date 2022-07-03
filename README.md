<div align="center">
<table>
    <theader>
        <tr>
            <th><img src="https://github.com/rescobedoulasalle/git_github/blob/main/ulasalle.png?raw=true" alt="EPIS" style="width:50%; height:auto"/></th>
            <th>
                <span style="font-weight:bold;">UNIVERSIDAD LA SALLE</span><br />
                <span style="font-weight:bold;">FACULTAD DE INGENIERÍAS</span><br />
                <span style="font-weight:bold;">DEPARTAMENTO DE INGENIERÍA Y MATEMÁTICAS</span><br />
                <span style="font-weight:bold;">CARRERA PROFESIONAL DE INGENIERÍA DE SOFTWARE</span>
            </th>            
        </tr>
    </theader>
    
</table>
</div>

-----------------------------------
<table>
</<tr>
<tr><td colspan="1">Alumno:
    <ul>
        <li>Bradlhy Luis Machado Medina  - bmachadom@ulasalle.edu.pe</li>
    </ul>
</td>
</<tr>
</table>

-----------------------------------
# Productor Consumidor

## Implementación consola:
Para implementar en C++ Productor y consumidor, lo primero que realicé es la implementación de las librerías y métodos que me serían de utilidad para el desarrollo posterior del proyecto:

```cpp
#include<iostream>
#include<thread>
#include<mutex>
#include <queue>
```

La primera librería ```iostream``` servirá para entrada y salida de datos, ```thread``` servirá para la generacion y uso de los hilo de concurrencia, ```mutex``` para controlar el interbloqueo de hilos o demonios, y ```queue``` para el uso de la estructura de datos tipo Cola.

Posterior a ello, definí las variables y constantes globales que usaré en el código:

```cpp
queue<char> colaElementos;	// Cola de almacenamiento de los datos
const char letras[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};	//Lista de datos
int cont = 0;
int estres =0;			//Contador de estres
int producido = 0;		//Contador de datos producidos
int consumido = 0;		//Contador de datos consumidos
mutex flag;			//Para el manejo de procesos e interbloqueos
```

Creé la clase ```monitor```, ya que se usará esta técnica para la exclusión mútua, donde se implementó los métodos ```producir``` y ```consumir```, los cuales serán encargados de manejar las partes críticas de la programación paralela de nuestro código:

```cpp
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
```

Siguientemente, se hizo la clase ```productor``` que será un hilo, en ella se implementan la funcionalidad básica y necesaria de nuestro "Thread", para que posteriormente sea controlado por nuestro monitor:

```cpp
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
```

De igual manera se hizo la clase ```consumidor```:
```cpp
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
```

Finalmente se ralizó el ```main``` o parte principal de nuestro programa, donde se instancian y se mandan a ejecutar nuestros hilos con su correspondiente monitor:

```cpp
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
```

#### Ejecución del programa: 
<p align="center"> 
  <th><img src="https://github.com/BradlhyMachado/Examen-Final/blob/main/ejecucion_consola.JPG?raw=true" alt="EPIS" style="width:50%; height:auto"/></th>
</p>

-----------------------------------
## Implementación Gráfica:
