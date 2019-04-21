#ifndef SUBJRCT_H
#define SUBJRCT_H

class IObserver;

class ISubject
{
public:
    virtual void Attach(IObserver *) = 0;
    virtual void Detach(IObserver *) = 0;
    virtual void Notify()            = 0;
};

#endif