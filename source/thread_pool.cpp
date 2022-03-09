#include "../includes/thread_pool.h"
using namespace my_pool;

void ThreadRoutine::work()
{
	while (enable)
	{
		if (!task_queue.empty())
		{
			key.lock();
			std::function<void()> func = task_queue.front();
			task_queue.pop();
			key.unlock();
			func();
		}
	}
}

void ThreadRoutine::add_task(std::function<void()> func)
{
	key.lock();
	task_queue.push(func);
	counter++;
	key.unlock();
}

ThreadRoutine::~ThreadRoutine()
{
	enable = false;
	working_thread.join();
}

ThreadPool::ThreadPool(int num_thr) : num_thr_(num_thr)
{
	if (num_thr < 0)
	{
		throw(std::logic_error("error"));
	}
	if (num_thr == 0)
	{
		num_thr = 1;
	}
	for (size_t count = 0 ; count < num_thr ; ++count)
	{
		workers.push_back(std::make_shared<ThreadRoutine>());
	}
}

std::shared_ptr<ThreadRoutine> ThreadPool::FindFreeThr()
{
	std::shared_ptr<ThreadRoutine> per = std::make_shared<ThreadRoutine>();
	per->counter = workers[0]->counter;
	for (auto it = workers.begin() ; it != workers.end() ; ++it)
	{
		if ((*it)->counter == 0)
			return *it;
		if (per->counter <= (*it)->counter)
		{
			per = (*it);
		}
	}
	return per;
}

