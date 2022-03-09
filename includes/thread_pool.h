#include <iostream>
#include <thread>
#include <functional>
#include <exception>
#include <queue>
#include <vector>
#include <mutex>
#include <memory>
#include <stdlib.h>

namespace my_pool
{

template<class T>
struct my_fut
{
	my_fut() : ready(false) {};
	T data;
	bool ready;
};

class ThreadRoutine final
{
public:
	ThreadRoutine() 
	{
		enable = true;
		working_thread = std::thread(&ThreadRoutine::work , this);
		counter = 0;
	};
	~ThreadRoutine();

	void work();
	void add_task(std::function<void()> task);

	size_t counter;
private:
	std::thread working_thread;
	std::queue<std::function<void()>> task_queue;
	std::mutex key;
	bool enable;
};

class ThreadPool final
{
public:
	ThreadPool(int num_thr = 1);
	int num_thr_;
	~ThreadPool() {};
public:
	template<class Fn , class... Args>
	std::shared_ptr<my_fut<int>> push(Fn func , Args... args)
	{
		std::function<void()> per_func = std::bind(func , args...);
		my_fut<int> res_fut;
		std::shared_ptr<my_fut<int>> res = std::make_shared<my_fut<int>>();
		std::function<void()> res_func = [=]{
								res->data = 0; 
								per_func();
								res->ready = true;
							};
		FindFreeThr()->add_task(std::bind(res_func));
		return res;
	}
	template<class R , class Fn , class... Args>
	std::shared_ptr<my_fut<R>> push(Fn func , Args... args)
	{
		std::function<R()> per_func = std::bind(func , args...);
		my_fut<R> res_fut;
		std::shared_ptr<my_fut<R>> res = std::make_shared<my_fut<R>>();
		std::function<void()> res_func = [=]{
								res->data = per_func();
								res->ready = true;
							};
		FindFreeThr()->add_task(std::bind(res_func));
		return res;
	}
	std::shared_ptr<ThreadRoutine> FindFreeThr();
private:
	std::vector<std::shared_ptr<ThreadRoutine>> workers;
};

};
