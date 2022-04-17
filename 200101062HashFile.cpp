#include <bits/stdc++.h>
#define INT_SIZE 4
using namespace std;

class record
{
    public:
        record(int, int);
        int get_primary_key();
        int get_record_size();
        void set_primary_key(int);
        void set_record_size(int);
        virtual ~record ();
    protected:
    private:
        int primary_key;
        int record_size;

};
record::record(int capacity, int pkey)
{
    record_size=capacity;
    primary_key=pkey;
}
int record::get_primary_key()
{
    return(primary_key);
}
int record::get_record_size()
{
    return(record_size);
}
void record::set_primary_key(int pkey)
{
    primary_key=pkey;
    return;
}
void record::set_record_size(int capacity)
{
    record_size=capacity;
    return;
}
record::~record ()
{

}
class page
{
    public:
        page(int);
        bool add_record(int, int);
        int search_in_page(int);
        bool is_full();
        page* get_prev();
        page*  get_next();
        void set_prev(page*);
        void set_next(page*);
        int get_count();
        record* get_at_index(int);
        void set_id(int);
        int get_id();
        virtual ~page ();
    protected:
    private:
        int page_id;
        int record_count;
        int page_size;
        page* prev;
        page* next;
        int occupied=0;
        vector<record*>directory;
};
page::page(int capacity)
{
    record_count=0;
    page_size=capacity;
    prev=nullptr;
    next=nullptr;
    occupied=4*INT_SIZE;
    page_id=0;
}
typedef struct
{
    int slot;
    page* page_ptr;
} record_location;
bool page::is_full()
{
    if(occupied>=page_size)
    {
        return true;
    }
    return false;
}
int page::get_count()
{
    return record_count;
}
void page::set_id(int id)
{
    page_id=id;
}
int page::get_id()
{
    return page_id;
}
record* page::get_at_index(int index)
{
    return directory[index];
}
bool page::add_record(int rec_size, int rec_id)
{
    if(is_full())
    {
        return false;
    }
    else
    {
        if(occupied+rec_size+INT_SIZE<=page_size)
        {
            record* r=new record(rec_size,rec_id);
            directory.push_back(r);
            record_count++;
            occupied+=rec_size;
            occupied+=INT_SIZE;
            return true;
        }
        return false;
    }
}
int page::search_in_page(int key)
{
    for(int i=0;i<directory.size();i++)
    {
        if(directory[i]->get_primary_key()==key)
        {
            return i;
        }
    }
    return -1;
}
page* page::get_prev()
{
    return prev;
}
page*  page::get_next()
{
    return next;
}
void page::set_prev(page* p)
{
    prev=p;
    return;
}
void page::set_next(page* p)
{
    next=p;
    return;
}
page::~page()
{

}
class hash_file
{
    public:
        hash_file(int);
        void add_record_in_file(int,int);
        record_location search_in_file(int);
        void print_file();
        void print_status();
        virtual ~hash_file ();
    protected:
    private:
        page* header;
        int page_capacity;
        int page_count;
        page* add_page_in_file(page*);

};
hash_file::hash_file(int pg_capacity)
{
    header=nullptr;
    page_capacity=pg_capacity;
    page_count=0;
}
page* hash_file::add_page_in_file(page* previous_node)
{
    page_count++;
    page* p=new page(page_capacity);
    if(header==nullptr)
    {
        header=p;
        p->set_prev(nullptr);
        p->set_next(nullptr);
        p->set_id(0);
        return p;
    }
    p->set_prev(previous_node);
    if(previous_node!=nullptr)
    {
       p->set_next(previous_node->get_next());
       previous_node->set_next(p);
       p->set_id(1+previous_node->get_id());
    }
    if(p->get_next()!=nullptr)
    {
        p->get_next()->set_prev(p);
    }
    return p;
}
void hash_file::add_record_in_file(int rec_size, int rec_id)
{
    page* p=header;
    page* pre;
    int flag=0;
    if(header==nullptr)pre=nullptr;
    while(1)
    {
        if(p==nullptr)
        {
            break;
        }
        else if(!p->add_record(rec_size,rec_id))
        {
            cout<<"no space";
            pre=p;
            p=p->get_next();
        }
        else
        {
            flag=1;
            break;
        }
    }
    if(flag==0)
    {
        page* q=add_page_in_file(pre);
        if(!q->add_record(rec_size, rec_id))
        {
            cout<<"page too small\n";
        }
    }
    return;
}
record_location hash_file::search_in_file(int key)
{
    page* p=header;
    record_location rec_loc;
    while(1)
    {
        if(p==nullptr)
        {
            rec_loc.slot=-1;
            rec_loc.page_ptr=nullptr;
            return rec_loc;
        }
        else if(p->search_in_page(key) == -1)
        {
            p=p->get_next();
        }
        else
        {
            rec_loc.slot=p->search_in_page(key);
            rec_loc.page_ptr=p;
            return rec_loc;
        }
    }
    return rec_loc;
}
void hash_file::print_file()
{
    page* p=header;
    int index=0;
    while(p!=nullptr)
    {
        cout<<index<<": ";
        for(int i=0;i<p->get_count();i++)
        {
            cout<<p->get_at_index(i)->get_primary_key()<<"("<<p->get_at_index(i)->get_record_size()<<") ";
        }
        cout<<"\n";
        index++;
        p=p->get_next();
    }
    return;
}
void hash_file::print_status()
{
    cout<<page_count<<" ";
    page* p=header;
    while(p!=nullptr)
    {
        cout<<p->get_count()<<" ";
        p=p->get_next();
    }
    cout<<"\n";
    return;
}
hash_file::~hash_file ()
{

}
int main()
{
    int file_page_size,command,key,rsize;
    cin>>file_page_size;
    hash_file h(file_page_size);
    while(1)
    {
        cin>>command;
        if(command==1)
        {
           // cout<<"Enter the value which is to be inserted.\n";
            cin>>rsize>>key;
            h.add_record_in_file(rsize, key);
        }
        if(command==2)
        {
           // h.print_file();
            h.print_status();
        }
        if(command==3)
        {
           // cout<<"Enter the value which is to be searched.\n";
            cin>>key;
            record_location r=h.search_in_file(key);
            if(r.page_ptr!=nullptr)cout<<r.page_ptr->get_id()<<" "<<r.slot<<"\n";
            else cout<<-1<<" "<<-1<<"\n";
        }
        if(command==4)
        {
            break;
        }
    }
    return 0;
}

