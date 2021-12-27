#ifndef SUBJECT_H
#define SUBJECT_H

#include "observers/observer.h"

#include <QList>

class Subject
{
public:
    void attach(Observer* observer) {
        observers.append(observer);
    }
    void detach(const unsigned int& index) {
        observers.remove(index);
    }
    void notify() {
        for(unsigned int i = 0; i < observers.size(); i++)
            observers[i]->update();
    }

protected:
    QList<Observer*> observers;
};

#endif // SUBJECT_H
