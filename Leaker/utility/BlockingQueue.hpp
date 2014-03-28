//阻塞队列的实现
//create by zc in 2014/3/1
#ifndef LEAKER_BLOCKINGQUEUE_H_
#define LEAKER_BLOCKINGQUEUE_H_

#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>
#include <deque>
#include <assert.h>

template <typename T>
class BlockingQueue:boost::noncopyable
{
public:
	BlockingQueue()
		:queue_(),
		Mutex_(),
		Condition_()
	{
	}

	void put(const T &t)
	{
		boost::mutex::scoped_lock lock(Mutex_);
		queue_.push_back(t);
		Condition_.notify_one();
	}

	T take()
	{
		boost::mutex::scoped_lock lock(Mutex_);

		while (queue_.empty())
		{
			Condition_.wait(lock);
		}
		assert(!queue_.empty());
		T front(queue_.front());
		queue_.pop_front();
		return front;
	}

	size_t size() const
	{
		boost::mutex::scoped_lock lock(Mutex_);
		return queue_.size();
	}
private:
	std::deque<T> queue_;
	boost::mutex Mutex_;
	boost::condition_variable_any Condition_;
};


#endif