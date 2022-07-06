#include <shared_mutex>
#include <iostream>
#include <thread>
#include <queue>

template <class T> class multithread_queue {
public:
	explicit multithread_queue() = default;
	bool empty();
	T front();
	T back();
	void push(const T&);
	void pop();

private:
	std::queue<T> queue;
	std::shared_mutex mutex;
};

template <class T> bool multithread_queue<T>::empty() {
	std::shared_lock<std::shared_mutex> lock(mutex);
	return queue.empty();
}

template <class T> T multithread_queue<T>::front() {
	std::shared_lock<std::shared_mutex> lock(mutex);
	return queue.front();
}

template <class T> T multithread_queue<T>::back() {
	std::shared_lock<std::shared_mutex> lock(mutex);
	return queue.back();
}

template <class T> void multithread_queue<T>::push(const T& element) {
	std::unique_lock<std::shared_mutex> lock(mutex);
	queue.push(element);
}

template <class T> void multithread_queue<T>::pop() {
	std::unique_lock<std::shared_mutex> lock(mutex);
	queue.pop();
}

int task2() {
	multithread_queue<int> queue;
	for (int i = -10; i < 0; i++) {
		queue.push(i);
	}
	std::thread fill_queue([&queue]() {for (unsigned int i = 0; i < 100; i++) {queue.push(i);}});
	std::thread read_queue([&queue]() {while (!queue.empty()) {std::cout << queue.front() << ' '; queue.pop();}});
	
	fill_queue.join();
	read_queue.join();
	
	std::cout << '\n';
	
	return 0;
}

int main() {
	std::cout << "Task 2:\n\n";
	task2();
	
	return 0;
}
