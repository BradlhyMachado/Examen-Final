#ifndef VARIABLESGLOBALES_H
#define VARIABLESGLOBALES_H

#include <QSemaphore>
#include "Constantes.h"

extern char buffer[BufferSize];

extern QSemaphore freeBytes;
extern QSemaphore usedBytes;

#endif // VARIABLESGLOBALES_H
