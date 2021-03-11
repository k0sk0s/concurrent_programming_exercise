#include <bits/stdc++.h>
#include <mutex>
#include <queue>
#include <condition_variable>
using namespace std;

mutex m;
queue<data_chunk> data_queue;
condition_variable data_cond;

void data_preparation_thread()
{
	while(more_data_to_prepare())
	{
		data_chunk const data=prepare_data();
		lock_guard<mutex> lk(m);
		data_queue.push(data);
		data_cond.notify_one();
	}
}

void data_processing_thread()
{
	while(true)
	{
		unique_lock<mutex> lk(m);
		data_cond.wait(
			lk,[](return !data_queue.empty();));
		data_chunk data=data_queue.front();
		data_queue.pop();
		lk.unlock();
		process(data);
		if(is_last_chunk(data))
			break;
	}
}

int main()
{

}