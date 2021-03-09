#include <bits/stdc++.h>
#include <thread>
#include <mutex>

using namespace std;

class hierarchical_mutex
{
private:
	mutex internal_mutex;
	unsigned long const hierarchy_value;
	unsigned long previous_hierarchy_value;

	static thread_local unsigned long this_thread_hierarchy_value;

	void check_for_hierarchy_violation()
	{
		if(this_thread_hierarchy_value<=hierarchy_value)
		{
			throw std::logic_error("mutex hierarchy violated");
		}
	}

	void update_hierarchy_value()
	{
		previous_hierarchy_value=this_thread_hierarchy_value;
		this_thread_hierarchy_value=hierarchy_value;
	}
public:
	explicit hierarchical_mutex(unsigned long value):
		hierarchy_value(value),
		previous_hierarchy_value(0)
	{
		//cout<<"in constuctor"<<endl;
	}

	void lock()
	{
		cout<<"in lock"<<endl;
		check_for_hierarchy_violation();
		internal_mutex.lock();
		update_hierarchy_value();
	}

	void unlock()
	{
		cout<<"in unlock"<<endl;
		this_thread_hierarchy_value=previous_hierarchy_value;
		internal_mutex.unlock();
	}


	bool try_lock()
	{
		cout<<"in try_lock"<<endl;
		check_for_hierarchy_violation();
		if(!internal_mutex.try_lock())
			return false;
		update_hierarchy_value();
		return true;
	}

};

thread_local unsigned long hierarchical_mutex::this_thread_hierarchy_value(ULONG_MAX);

hierarchical_mutex high_level_mutex(10000);
hierarchical_mutex low_level_mutex(5000);

int do_low_level_stuff()
{
	cout<<"in do_low_level_stuff"<<endl;
	return 1;
}

int low_level_func()
{
	cout<<"in low_level_func"<<endl;
	lock_guard<hierarchical_mutex> lk(low_level_mutex);
	return do_low_level_stuff();
}

void high_level_stuff(int some_param)
{
	cout<<"in high_level_stuff"<<endl;
}

void high_level_func()
{
	cout<<"in high_level_func"<<endl;
	lock_guard<hierarchical_mutex> lk(high_level_mutex);
	high_level_stuff(low_level_func());
}

void thread_a()
{
	cout<<"in thread_a"<<endl;
	high_level_func();
}

hierarchical_mutex other_mutex(100);

void do_other_stuff()
{
	cout<<"in do_other_stuff"<<endl;
}

void other_stuff()
{
	cout<<"in other_stuff"<<endl;
	high_level_func();
	do_other_stuff();
}

void thread_b()
{
	cout<<"in thread_b"<<endl;
	lock_guard<hierarchical_mutex> lk(other_mutex);
	other_stuff();
}

int main()
{
	thread a(thread_a);
	//thread b(thread_b);
	a.join();
	//b.join();
}