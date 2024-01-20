#ifndef _DOUBLESTACK_HPP
#define _DOUBLESTACK_HPP


#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <deque>

#include "console.hpp"

namespace BKM {
	template<typename varType>
	class DoubleStack
	{
		// private data members
		std::mutex core_guard;
		std::deque<varType> core;
		std::condition_variable conditional;
		std::mutex execution_guard;

	public:
		DoubleStack() = default;
		DoubleStack(const DoubleStack<varType>&) = delete;
		virtual ~DoubleStack() { clear(); }

		// Returns without removal from the front of the core (dequeue)
		const varType& Front()
		{
			std::scoped_lock corelock(core_guard);
			return core.front();
		}

		// Returns without removal from the end of the core (dequeue)
		const varType& Back()
		{
			std::scoped_lock coreLock(core_guard);
			return core.back();
		}

		// Removes and returns element from front of the core (dequeue)
		varType PopFront()
		{
			std::scoped_lock coreLock(core_guard);
			auto element = std::move(core.front());
			core.pop_front();
			return element;
		}

		// Removes and returns element from back of the core (dequeue)
		varType PopBack()
		{
			std::scoped_lock coreLock(core_guard);
			auto element = std::move(core.back());
			core.pop_back();
			return element;
		}

		// Adds an element to the end of the core (dequeue)
		void PushBack(const varType& element)
		{
			std::scoped_lock coreLock(core_guard);
			core.emplace_back(std::move(element));

			std::unique_lock<std::mutex> threadLock(execution_guard);
			conditional.notify_one();
		}

		// Adds an element to front of the core (dequeue)
		void PushFront(const varType& element)
		{
			std::scoped_lock coreLock(core_guard);
			core.emplace_front(std::move(element));

			std::unique_lock<std::mutex> threadLock(execution_guard);
			conditional.notify_one();
		}

		// Returns true if the core has no elements
		bool Empty()
		{
			std::scoped_lock coreLock(core_guard);
			return core.empty();
		}

		// Returns number of elements in core
		size_t Count()
		{
			std::scoped_lock coreLock(core_guard);
			return core.size();
		}

		// Clears the core (dequeue)
		void Clear()
		{
			std::scoped_lock coreLock(core_guard);
			core.clear();
		}

		void Wait()
		{
			while (empty())
			{
				std::unique_lock<std::mutex> threadLock(execution_guard);
				conditional.wait(threadLock);
			}
		}
	};
}

#endif // _DOUBLESTACK_HPP