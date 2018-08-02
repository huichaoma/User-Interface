#ifndef THREADS_H
#define THREADS_H

#include<QThread>
#include"serial_config.h"

class Threads:public QThread
{
public:
    Threads();
    void run();
};

#endif // THREADS_H
