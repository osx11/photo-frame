#pragma once

#ifndef FIXEDSIZELIST_H
#define FIXEDSIZELIST_H


template <typename T>
class FixedSizeList {
  public:
    explicit FixedSizeList(int capacity);
    ~FixedSizeList();

    void append(T item);
    int size();
    T get(int index);

  private:
    int _capacity;
    int _size;
    T *_list;
};

template <typename T>
FixedSizeList<T>::FixedSizeList(int capacity) {
  _capacity = capacity;
  _size = 0;
  _list = new T[capacity];
}

template <typename T>
FixedSizeList<T>::~FixedSizeList() {
  delete _list;
}

template <typename T>
void FixedSizeList<T>::append(T item) {
  _list[_size++] = item;
}

template <typename T>
int FixedSizeList<T>::size() {
  return _size;
}

template <typename T>
T FixedSizeList<T>::get(int index) {
  return _list[index];
}


#endif //FIXEDSIZELIST_H
