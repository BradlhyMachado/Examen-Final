#include "productor.h"
#include "VariablesGlobales.h"

Productor::Productor(QObject *parent)
    :QThread(parent)
{
}

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
