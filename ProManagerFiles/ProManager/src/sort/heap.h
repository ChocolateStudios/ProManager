#ifndef HEAP_H
#define HEAP_H

#include <QList>

template<typename T>
void siftDown(QList<T>& arr, int start, int end, std::function<bool(T, T)> compare) {
    for (int root = start; root*2 + 1 <= end; ) {
        int child = root*2 + 1;
        int swap = root;
        if (compare(arr[swap], arr[child])) swap = child;
        if (child+1 < end && compare(arr[swap], arr[child+1])) swap = child+1;
        if (swap == root) {
            return;
        } else {
            auto temp = arr[root];
            arr[root] = arr[swap];
            arr[swap] = temp;
            root = swap;
        }
    }
}

template<typename T>
void heapify(QList<T>& arr, int end, std::function<bool(T, T)> compare) {
    for (int i = (end - 1) / 2; i >= 0; i--) {
        siftDown(arr, i, end, compare);
    }
}

template<typename T>
void heapSort(QList<T>& arr, int n, std::function<bool(T, T)> compare) {
    heapify(arr, n, compare);
    for (int end = n - 1; end > 0; end--) {
        auto temp = arr[end];
        arr[end] = arr[0];
        arr[0] = temp;
        siftDown(arr, 0, end-1, compare);
    }
}

#endif // HEAP_H
