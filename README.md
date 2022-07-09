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
##### 1.- Ejecición en compilador online, para apreciar los hilos de mejor manera, ya que los procesos en este, se ejecutan de manera más lenta.
<p align="center"> 
  <th><img src="https://github.com/BradlhyMachado/Examen-Final/blob/main/ejecucion_consola.JPG?raw=true" alt="EPIS" style="width:50%; height:auto"/></th>
</p>

##### 2.- Ejecición en mi máquina virtual (Ubuntu 20.04).
<p align="center"> 
  <th><img src="https://github.com/BradlhyMachado/Examen-Final/blob/main/ejecucion_consola_2.JPG?raw=true" alt="EPIS" style="width:50%; height:auto"/></th>
</p>

<p align="center"> 
  <th><img src="https://github.com/BradlhyMachado/Examen-Final/blob/main/ejecucion_consola_3.JPG?raw=true" alt="EPIS" style="width:50%; height:auto"/></th>
</p>

-----------------------------------
## Implementación Gráfica:
Para implementar el programa de "Consumidor-Productor". se me complicó un poco, ya que no tenía nuchos conocimientos de implementación de interfaces en ```C++```, por lo que despues de una ardua, larga y luchada investigación logré encontrar QT, herramienta con la cual se generan interfaces atractivas y bonitas en ```C++```, por lo que prosegí a instalarla en mi entorno de trabajo.

Luego de tener la herramienta instalada en mi máquina, empecé con el correspondiente desarrollo.
<p align="center"> 
  <th><img src="https://github.com/BradlhyMachado/Examen-Final/blob/main/qt_ubuntu.JPG?raw=true" alt="EPIS" style="width:50%; height:auto"/></th>
</p>

Creé el proyecto:
<p align="center"> 
  <th><img src="https://github.com/BradlhyMachado/Examen-Final/blob/main/qt_new_project.JPG?raw=true" alt="EPIS" style="width:50%; height:auto"/></th>
</p>

Y posterior a ello generé las nuevas clases y encabezados para productor, consumidor y los elementos necesarios para el buen funcionamiento de nuestro programa.

Los archivos resaltados, son los que vienen por defecto al crear el proyecto.
<p align="center"> 
  <th><img src="https://github.com/BradlhyMachado/Examen-Final/blob/main/qt_packages.JPG?raw=true" alt="EPIS" style="width:20%; height:auto"/></th>
</p>

Dado que QT tiene sus propios métodos, clases y funcionamientos y librerías, usé los ```QSemaphore```, ```Qthreads``` y ```Q...``` archivos necesarios para el funcionamiento.

En el encabezado ```Constantes.h``` definí los valores de auquellas variables que no pueden cambiar de valor durante la ejecución del programa, tales como el tamaño del buffer, y tamaño de los datos, que fueron establecidos como constantes.

```cpp
#ifndef CONSTANTES_H
#define CONSTANTES_H

const int DataSize = 100000;
const int BufferSize = 8192;

#endif // CONSTANTES_H
```
En cuanto a ```VariablesGlobales.h```, se definión los semáforos, y el buffer.

```cpp
#ifndef VARIABLESGLOBALES_H
#define VARIABLESGLOBALES_H

#include <QSemaphore>
#include "Constantes.h"

extern char buffer[BufferSize];

extern QSemaphore freeBytes;
extern QSemaphore usedBytes;

#endif // VARIABLESGLOBALES_H
```

En cuanto a ```productor.h``` es el encabezado de nuestra clase ```productor.cpp```, en otras palabras, allí es donde se define la estructura que se trabajará la clase.

```cpp
#ifndef PRODUCTOR_H
#define PRODUCTOR_H

#include <QThread>
#include <QTime>

class Productor : public QThread
{
    Q_OBJECT
public:
    explicit Productor(QObject *parent = 0);
    void run();

signals:
    void bufferFillCountChanged(int bCount);
    void producerCountChanged(int count);

public slots:
};

#endif // PRODUCTOR_H
```

de igual manera con ```consumidor.h```

```cpp
#ifndef CONSUMIDOR_H
#define CONSUMIDOR_H

#include <QThread>
#include <QTime>

class Consumidor : public QThread
{
    Q_OBJECT
public:
    explicit Consumidor(QObject *parent = 0);
    void run();

signals:
    //void stringConsumed(const QString &text;);
    void bufferFillCountChanged(int cCount);
    void consumerCountChanged(int count);
public slots:
};

#endif // CONSUMIDOR_H
```

En cuanto al paquete ```source```, allí es donde se encuentra la implementación de todas y cada una de las clases que vayamos a utilizar en el aplicativo.

```consumidor.cpp```
```cpp
#include "consumidor.h"
#include "VariablesGlobales.h"

Consumidor::Consumidor(QObject *parent) :
    QThread(parent)
{
}

//Correr hilo
void Consumidor::run(){
    for (int i = 0; i < DataSize; ++i) {
        usedBytes.acquire();
        fprintf(stderr, "%c", buffer[i % BufferSize]);
        freeBytes.release();
        emit bufferFillCountChanged(usedBytes.available());
        emit consumerCountChanged(i);
    }
    fprintf(stderr, "\n");
}
```

```productor.cpp```
```cpp
#include "productor.h"
#include "VariablesGlobales.h"

Productor::Productor(QObject *parent)
    :QThread(parent)
{
}

//Correr hilo
void Productor::run(){
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    for (int i = 0; i < DataSize; ++i) {
        freeBytes.acquire();
        buffer[i % BufferSize] = "abcdefghijklmnopqrstuvwxyz"[(int)qrand() % 26];
        usedBytes.release();
        if(i % 20 == 0)
            emit bufferFillCountChanged(usedBytes.available());
            emit producerCountChanged(i);
    }
}
```
En cuanto a la parte gráfica, usé las funcionalidades y herramientas que brinda QT 
<p align="center"> 
  <th><img src="https://github.com/BradlhyMachado/Examen-Final/blob/main/qt_interface.JPG?raw=true" alt="EPIS" style="width:70%; height:auto"/></th>
</p>

Posterior a esto, conecté todos lo elementos, cree los eventos, y los configuré, con las señales y alertas correspondientes en el archivo principal de la parte gráfica ```mainwindow.cpp```.

```cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Constantes.h"

//TamañoBuffer: Màximo almacenamiento
char buffer[BufferSize];

QSemaphore freeBytes(BufferSize);
QSemaphore usedBytes;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Definiendo rando del progress bar
    ui->Productor->setRange(0, DataSize);
    ui->Consumidor->setRange(0, DataSize);
    ui->Buffer->setRange(0, DataSize);

    //Creando Hilos
    mProductor = new Productor(this);
    mConsumidor = new Consumidor(this);

    //Conectando señales para el progress bar del "Buffer"
    connect(mProductor, SIGNAL(bufferFillCountChanged(int)), this, SLOT(on_Buffer_valueChanged(int)));
    connect(mConsumidor, SIGNAL(bufferFillCountChanged(int)),this, SLOT(on_Buffer_valueChanged(int)));

    //Conectando señales para el progress bar de "Consumidor y Productor"
    connect(mProductor, SIGNAL(producerCountChanged(int)),this, SLOT(on_Productor_valueChanged(int)));
    connect(mConsumidor, SIGNAL(consumerCountChanged(int)), this, SLOT(on_Consumidor_valueChanged(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mProductor;
    delete mConsumidor;
}

void MainWindow::on_Buffer_valueChanged(int value)
{
    ui->Buffer->setValue(value);
}

void MainWindow::on_Productor_valueChanged(int value)
{
    ui->Productor->setValue(value);
}

void MainWindow::on_Consumidor_valueChanged(int value)
{
    ui->Consumidor->setValue(value);
}

//Boton de inicio
void MainWindow::on_iniciar_clicked()
{
    ui->iniciar->setEnabled(false); //Desactivando boton de inicio
    ui->detener->setEnabled(true);  //Activando boton de detener

    //Iniciando hilos
    mProductor->start();
    mConsumidor->start();
}

void MainWindow::on_detener_clicked()
{
    ui->iniciar->setEnabled(true);  //Activando boton de inicio
    ui->detener->setEnabled(false); //Desactivadno boton de detener

    //Terminando hilos
    mProductor->terminate();
    mConsumidor->terminate();
}
```

#### Ejecución del programa:

<p align="center"> 
  <th><img src="https://github.com/BradlhyMachado/Examen-Final/blob/main/ejecucion_GUI.JPG?raw=true" alt="EPIS" style="width:50%; height:auto"/></th>
</p>

<p align="center"> 
  <th><img src="https://github.com/BradlhyMachado/Examen-Final/blob/main/ejecucion_GUI_2.JPG?raw=true" alt="EPIS" style="width:50%; height:auto"/></th>
</p>

## Bibliografía:
- https://doc.qt.io/qt-6/qtcore-threads-semaphores-example.html
- https://doc.qt.io/qt-6/qthread.html
