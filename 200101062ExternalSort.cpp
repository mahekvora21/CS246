#include <bits/stdc++.h>
#define INT_SIZE 4
using namespace std;

class page
{
    public:
        page(int);
        void add_record(int);
        page* get_prev();
        page*  get_next();
        void set_prev(page*);
        void set_next(page*);
        int get_at_index(int);
        void set_at_index(int,int);
        void set_count(int);
        int get_count();
        void set_id(int);
        int get_id();
        bool is_full();
        void sort_page();
        virtual ~page ();
    protected:
    private:
        int page_size;
        int page_id;
        page* prev;
        page* next;
        int rec_count=0;
        vector<int>records;

};
page::page(int capacity)
{
    page_size=capacity;
    rec_count=0;
}
int page::get_count()
{
    return rec_count;
}
void page::set_count(int c)
{
    rec_count=c;
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
bool page::is_full()
{
    if(rec_count==page_size)
    {
        return true;
    }
    return false;
}
int page::get_at_index(int index)
{
    return records[index];
}
void page::set_at_index(int index,int rec)
{
    if(index>=records.size())
    {
        cout<<rec<<" "<<index<<"invalid.\n";return;
    }
    records[index]=rec;
}
void page::add_record(int r)
{
    records.push_back(r);
    rec_count++;
}
void page::set_id(int id)
{
    page_id=id;
}
int page::get_id()
{
    return page_id;
}
void page::sort_page()
{
    sort(records.begin(),records.end());
    return;
}
page::~page()
{

}
class datafile
{
    public:
        datafile(int,int,int);
        void insert_record(int);
        void print();
        void print_status();
        void external_sort();
        void create_runs();
        void print_runs();
        virtual ~datafile ();
    protected:
    private:
        int buffer_pages;
        int page_capacity;
        int total_records;
        page* header;
        page* cur_page;
        page* add_page_in_file(page*);
        int page_count=0;
        void write_back();
        vector<vector<int> >runs;
};
datafile::datafile(int pages,int rec_per_pg,int recs)
{
    buffer_pages=pages;
    page_capacity=rec_per_pg;
    total_records=recs;
}
page* datafile::add_page_in_file(page* previous_node)
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
void datafile::insert_record(int r)
{
    page* p=header;
    page* pre=p;
    page* q;
    if(header==nullptr)
    {
        p=add_page_in_file(nullptr);
    }
    while(p->is_full())
    {
        pre=p;
        p=p->get_next();
        if(p==nullptr)
        {
            break;
        }
    }
    if(p==nullptr)
    {
        p=add_page_in_file(pre);
    }
    p->add_record(r);
    return;
}
void datafile::print()
{
    page* p=header;
    int index=0;
    while(p!=nullptr)
    {
        cout<<index<<": ";
        for(int i=0;i<p->get_count();i++)
        {
            cout<<p->get_at_index(i)<<" ";
        }
        cout<<"\n";
        index++;
        p=p->get_next();
    }
    return;
}
void datafile::print_status()
{
    page* p=header;
    while(p!=nullptr)
    {
        for(int i=0;i<p->get_count();i++)
        {
            cout<<p->get_at_index(i)<<"\n";
        }
        p=p->get_next();
    }
    return;
}
void datafile::create_runs()
{
    page* cur=header;
    vector<int>run;
    int f=0;
    while(cur!=nullptr)
    {
        for(int i=0;i<buffer_pages;i++)
        {
            if(cur==nullptr)
            {
                //cout<<"cur null";
                f=1;
                break;
            }
            for(int j=0;j<cur->get_count();j++)
            {
               run.push_back(cur->get_at_index(j));
            }
            cur=cur->get_next();
        }
        sort(run.begin(),run.end());
        runs.push_back(run);
        run.clear();
        if(f==1)break;

    }
}
void datafile::print_runs()
{
    cout<<"\n runs "<<runs.size()<<"\n";
    for(int i=0;i<runs.size();i++)
    {
        cout<<i<<":";
        for(int j=0;j<runs[i].size();j++)
        {
            cout<<runs[i][j]<<" ";
        }cout<<"\n";
    }
}
void datafile::external_sort()
{
    create_runs();
    int merging_runs_size=buffer_pages-1;
    int l=0;
    //merge b-1 runs at a time ==> b(b-1) pages per run ==> b(b-1)^k sized run.
    while(runs.size()>1)
    {
        int l=0;
        vector<vector<int> >tmp_runs;
        while(l<runs.size())
        {
            vector<int>tmp;
            int ptr[runs.size()]={0};
            bool new_run_made=false;

            while(!new_run_made)
            {
                int cur_min=INT_MAX,index_used=-1;
                new_run_made=true;
                for(int i=l;i<min(l+merging_runs_size,int(runs.size()));i++)
                {
                    if(ptr[i]<runs[i].size())
                    {
                        new_run_made=false;break;
                    }
                }
                for(int i=l;i<min(l+merging_runs_size,int(runs.size()));i++)
                {
                   //merge runs
                   //store to temporary run.
                    if(ptr[i]>=runs[i].size())
                    {
                        continue;
                    }
                    if(cur_min>=runs[i][ptr[i]])
                    {
                        cur_min=runs[i][ptr[i]];
                        index_used=i;
                    }
                }
                if(!new_run_made)tmp.push_back(cur_min);
                ptr[index_used]++;
            }
        tmp_runs.push_back(tmp);
        tmp.clear();
        l+=merging_runs_size;
        }
        runs.clear();
        for(int j=0;j<tmp_runs.size();j++)runs.push_back(tmp_runs[j]);
        tmp_runs.clear();
    }
    write_back();
}
void datafile::write_back()
{
    page* p=header;
    int index=0;
    while(p!=nullptr)
    {
        for(int i=0;i<p->get_count()&&index<runs[0].size();i++)
        {
            p->set_at_index(i,runs[0][index]);
            index++;
        }
        p=p->get_next();
    }
    return;
}
datafile::~datafile()
{

}

int main()
{
    int buffer_pages,records_per_page,total_records;
    cin>>buffer_pages>>records_per_page>>total_records;
    datafile d(buffer_pages,records_per_page,total_records);
    int r;
    for(int i=0;i<total_records;i++)
    {
        cin>>r;
        d.insert_record(r);
    }
    d.external_sort();
    d.print_status();
}
