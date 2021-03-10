#include<bits/stdc++.h>
#include<thread>
#include<mutex>
#include<map>
#include<boost/thread/shared_mutex.hpp>

using namespace std;

class dns_entry
{
    //~dns_entry()=default;
};

class dns_cache
{
    map<string,dns_entry> entries;
    mutable boost::shared_mutex entry_mutex;
public:
    dns_entry find_entry(string const& domain) const
    {
        boost::shared_lock<boost::shared_mutex> lk(entry_mutex);
        map<string,dns_entry>::const_iterator const it=entries.find(domain);
        return (it==entries.end())?dns_entry():it->second;
    }
    void update_or_add_entry(string const& domain,dns_entry const& dns_details)
    {
        lock_guard<boost::shared_mutex> lk(entry_mutex);
        entries[domain]=dns_details;
    }
};

int main()
{

}