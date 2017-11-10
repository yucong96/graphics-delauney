#pragma once

#include "head.h"

template<class T> class Queue {

private:
	T* queue = NULL;
	int size = 10, head = 0, tail = 0;
	bool(*isEqual) (const T& a, const T& b);

public:
	static const int IN_QUEUE = 0;
	static const int NOT_IN_QUEUE = 1;
	Queue(bool(*_isEqual)(const T& a, const T&b)) {
		queue = (T*)malloc(size * sizeof(T));
		isEqual = _isEqual;
	}
	~Queue() {
		deleteQueue();
	}
	
	int length() {
		return (tail - head + size) % size;
	}

	T at(int index) {
		return queue[(head + index) % size];
	}

	bool isInQueue(const T& element) {
		bool inQueue = false;
		if (head <= tail) {
			for (int i = head; i < tail; i++) {
				if (isEqual(queue[i], element)) {
					inQueue = true;
					break;
				}
			}
		}
		else {
			for (int i = head; i < size; i++) {
				if (isEqual(queue[i], element)) {
					inQueue = true;
					break;
				}
			}
			for (int i = 0; i < tail; i++) {
				if (isEqual(queue[i], element)) {
					inQueue = true;
					break;
				}
			}
		}
		return inQueue;
	}
	
	void enlarge() {
		T* newQueue = (T*)malloc(size * 2 * sizeof(T));
		int j = 0;
		if (head <= tail) {
			for (int i = head; i < tail; i++, j++) {
				newQueue[j] = queue[i];
			}
		}
		else {
			for (int i = head; i < size; i++, j++) {
				newQueue[j] = queue[i];
			}
			for (int i = 0; i < tail; i++, j++) {
				newQueue[j] = queue[i];
			}
		}
		head = 0;
		tail = size - 1;
		size = size * 2;
		deleteQueue();
		queue = newQueue;
	}

	int push(const T& element) {
		if (isInQueue(element)) {
			return IN_QUEUE;
		} 
		else {
			if ((tail + 1) % size == head) {
				enlarge();
			}
			queue[tail] = element;
			tail = (tail + 1) % size;
			return NOT_IN_QUEUE;
		}
	}

	T pop() {
		if (head == tail) {
			cout << "Error: T pop()" << endl;
		}
		head = (head + 1) % size;
		return queue[(head + size - 1) % size];
	}

	void clear() {
		head = 0;
		tail = 0;
	}

	void deleteQueue() {
		delete[] queue;
		queue = NULL;
	}
};