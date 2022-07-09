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
