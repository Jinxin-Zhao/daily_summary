#ifndef OBSERVER_H
#define OBSERVER_H

class IObserver
{
public:
    virtual void update(float price) = 0;
};

#endif