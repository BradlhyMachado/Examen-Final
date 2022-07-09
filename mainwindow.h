#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "consumidor.h"
#include "productor.h"

#include <QSemaphore>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void on_Productor_valueChanged(int value);
    void on_Consumidor_valueChanged(int value);
    void on_Buffer_valueChanged(int value);

private slots:
    void on_iniciar_clicked();
    void on_detener_clicked();

private:
    Ui::MainWindow *ui;
    //Iniciar hilos
    Productor *mProductor;
    Consumidor *mConsumidor;
};

#endif // MAINWINDOW_H



/////////////////
