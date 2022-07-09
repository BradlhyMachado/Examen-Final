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
