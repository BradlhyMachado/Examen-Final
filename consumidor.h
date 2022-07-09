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
