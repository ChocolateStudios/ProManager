#ifndef DOCKARGUMENTS_H
#define DOCKARGUMENTS_H

#include <QList>

class QMainWindow;

struct DockArguments {
    DockArguments(QMainWindow* mainWin)
        : mainWin(mainWin)
    {
    }

    template<typename T>
    DockArguments& add(T* pointer) {
        pointers.append(pointer);
        return *this;
    }

    template<typename T>
    T* get(const unsigned int& index) {
        return (T*)(pointers[index]);
    }

    QMainWindow* mainWin;
    QList<void*> pointers;
};

#endif // DOCKARGUMENTS_H
