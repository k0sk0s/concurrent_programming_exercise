#include <bits/stdc++.h>
#include <thread>
#include <mutex>
using namespace std;

class data_packet
{
public:
	data_packet()
	{}
	~data_packet()
	{}
private:
	int value;
};

class connection_info
{
public:
	connection_info()
	{}
	~connection_info()
	{}
private:
	int info;
};

class connection_handle
{
public:
	connection_handle()
	{}
	~connection_handle()
	{}
	void send_data(data_packet data)
	{
		cout<<"send_data"<<endl;
	}
	data_packet receive_data()
	{
		cout<<"receive_data"<<endl;
	}
};

class connection_manager
{
public:
	connection_manager()
	{}
	~connection_manager()
	{}
	connection_handle open(connection_info info)
	{
		cout<<"connection is open"<<endl;
	}
};

connection_manager manager;

class X
{
private:
	connection_info connection_details;
	connection_handle connection;
	once_flag connection_init_flag;

	void open_connection()
	{
		connection=manager.open(connection_details);
	}
public:
	X(connection_info const& connection_details_):
		connection_details(connection_details)
	{}
	void send_data(data_packet const& data)
	{
		call_once(connection_init_flag,&X::open_connection,this);
		connection.send_data(data);
	}
	data_packet receive_data()
	{
		call_once(connection_init_flag,&X::open_connection,this);
		return connection.receive_data();
	}	
};

int main()
{

}