#include <bits/stdc++.h>

using namespace std;

class bucket
{
    public:
        bucket(int);
        virtual ~bucket();
        bool isFull();
        bool set_local_depth(int);
        int get_local_depth();
        bool set_capacity(int);
        int get_capacity();
        bool set_occupied(int);
        int get_occupied();
        int* data;
    protected:
    private:
        int local_depth;
        int occupied;
        int capacity;
};

bucket::bucket(int c)
{
    local_depth=1;
    occupied=0;
    capacity=c;
    data=new int[capacity];
}
bool bucket::isFull()
{
    if(occupied==capacity)
    {
        return true;
    }
    return false;
}
int bucket::get_capacity()
{
    return capacity;
}
int bucket::get_local_depth()
{
    return local_depth;
}
int bucket::get_occupied()
{
    return occupied;
}
bool bucket::set_capacity(int n)
{
    capacity=n;
    return true;
}
bool bucket::set_local_depth(int n)
{
    local_depth=n;
    return true;

}
bool bucket::set_occupied(int n)
{
    occupied=n;
    return true;

}
bucket::~bucket()
{
    //dtor
}

class directory
{
    public:
        directory(int, int);
        virtual ~directory();
        vector<bucket*>buckets_timestamp;
        int global_depth;
        void insert_function(int);
        void lazy_delete(int);
        void delete_function(int);
        void delete_function_2(int);
        string bucket_id(int);
        void print();
        void print_status();
        void print_by_time();
        int search_dir(int);
    protected:

    private:
        vector<bucket*>dir;
        void expand_dir();
        void contract_dir();
        void split(int);
        int next_bucket(int, int);
        int hash_function(int);
        int get_global_depth();
};
directory::directory(int n,int c)
{
    global_depth=n;

    for(int i=0;i<pow(2,n);i++)
    {
        bucket* b1=new bucket(c);
        b1->set_local_depth(global_depth);
        dir.push_back(b1);
        buckets_timestamp.push_back(b1);
    }
}

int directory::hash_function(int key)
{
    return key&((1<<global_depth)-1);
}
void directory::expand_dir()
{
    for(int i=0;i<(1<<global_depth);i++)
    {
        dir.push_back(dir[i]);
    }
    global_depth++;
}
void directory::contract_dir()
{
    for(int i=0;i<dir.size();i++)
    {
        if(dir[i]->get_local_depth()==global_depth)
        {
            return;
        }
    }
    global_depth--;
    for(int i=0;i<(1<<global_depth);i++)
    {
        dir.pop_back();
    }
}
int directory::next_bucket(int n, int depth)
{
    return(n^(1<<(depth-1)));
}
string directory::bucket_id(int n)
{
    int d = dir[n]->get_local_depth();
    string s="";
    while(n>0 && d>0)
    {
        s = (n%2==0?"0":"1")+s;
        n/=2;
        d--;
    }
    while(d>0)
    {
        s = "0"+s;
        d--;
    }
    return s;
}
/*void directory::split(int bucket_no)
{
    int flag=0;
    bucket tmp=*dir[bucket_no];
    int ld=tmp.get_local_depth();
    dir[bucket_no]->set_local_depth(ld+1);
    ld++;
    if(ld>global_depth)
    {
        expand_dir();
    }
    int next_bucket_index=next_bucket(bucket_no,ld);
    //cout<<"next: "<<next_bucket_index<<"\n";
    int c=dir[0]->get_capacity();
    dir[next_bucket_index]=new bucket(c);
    buckets_timestamp.push_back(dir[next_bucket_index]);
    int gap=(1<<ld);
    dir[next_bucket_index]->set_local_depth(dir[bucket_no]->get_local_depth());

    for(int i=next_bucket_index+gap;i<dir.size();i+=gap)
    {
        dir[i]=dir[next_bucket_index];
    }
    for(int i=next_bucket_index-gap;i>=0;i-=gap)
    {
        dir[i]=dir[next_bucket_index];
    }
    dir[bucket_no]->set_occupied(0);
    for(int i=0;i<tmp.get_occupied();i++)
    {
        insert_function(tmp.data[i]);
    }
}*/
void directory::split(int bucket_no)
{
    int flag=0;
    bucket tmp=*dir[bucket_no];
    int ld=tmp.get_local_depth();
    dir[bucket_no]->set_local_depth(ld+1);
    ld++;
    if(ld>global_depth)
    {
        expand_dir();
    }
    int next_bucket_index=next_bucket(bucket_no,ld);
    if ((next_bucket_index & (1<<(ld-1))) == 0){
        int temp = next_bucket_index;
        next_bucket_index = bucket_no;
        bucket_no = temp;
    }
    //cout<<"next: "<<next_bucket_index<<"\n";
    int c=dir[0]->get_capacity();
    dir[next_bucket_index]=new bucket(c);
    buckets_timestamp.push_back(dir[next_bucket_index]);
    int gap=(1<<ld);
    dir[next_bucket_index]->set_local_depth(dir[bucket_no]->get_local_depth());

    for(int i=next_bucket_index+gap;i<dir.size();i+=gap)
    {
        dir[i]=dir[next_bucket_index];
    }
    for(int i=next_bucket_index-gap;i>=0;i-=gap)
    {
        dir[i]=dir[next_bucket_index];
    }
    dir[bucket_no]->set_occupied(0);
    for(int i=0;i<tmp.get_occupied();i++)
    {
        insert_function(tmp.data[i]);
    }
}
void directory::insert_function(int key)
{
    if(search_dir(key)!=(-1))
    {
        cout<<"Key already present.\n";
        return;
    }
    int bucket_index=hash_function(key);
    if(dir[bucket_index]->isFull())
    {
        split(bucket_index);
        insert_function(key);
    }
    else
    {
        dir[bucket_index]->data[(dir[bucket_index]->get_occupied())]=(key);
        dir[bucket_index]->set_occupied((dir[bucket_index]->get_occupied())+1);
    }
}
void directory::delete_function(int key)
{
    if(search_dir(key)==-1)
    {
        //cout<<"Key absent.\n";
        return;
    }
    else
    {
        int bucket_no=hash_function(key);
        int j=search_dir(key);
        for(int i=j+1;i<dir[bucket_no]->get_occupied();i++)
        {
            dir[bucket_no]->data[i-1]=dir[bucket_no]->data[i];
        }
        dir[bucket_no]->set_occupied(dir[bucket_no]->get_occupied()-1);
        if(dir[bucket_no]->get_occupied()==0)
        {
            int gap=(1<<(dir[bucket_no]->get_local_depth()-1));
            int next_bucket_index=next_bucket(bucket_no,(dir[bucket_no]->get_local_depth()));
            if(gap>1 && dir[next_bucket_index]->get_local_depth()==dir[bucket_no]->get_local_depth())
            {
                for(int j=0;j<buckets_timestamp.size();j++)
                {
                    if(buckets_timestamp[j]==dir[bucket_no])
                    {
                        buckets_timestamp.erase(buckets_timestamp.begin()+j);break;
                    }
                }
                delete(dir[bucket_no]);
                dir[next_bucket_index]->set_local_depth((dir[next_bucket_index]->get_local_depth()-1));
                for(int i=gap+next_bucket_index;i<dir.size();i+=gap)
                {
                    dir[i]=dir[next_bucket_index];
                }
                for(int i=next_bucket_index-gap;i>=0;i-=gap)
                {
                    dir[i]=dir[next_bucket_index];
                }
            }
            if(get_global_depth()!=global_depth)
            {
                contract_dir();
            }
        }
    }

}
void directory::lazy_delete(int key)
{
    if(search_dir(key)==-1)
    {
        //cout<<"Key absent.\n";
        return;
    }
    else
    {
        int bucket_no=hash_function(key);
        int j=search_dir(key);
        for(int i=j+1;i<dir[bucket_no]->get_occupied();i++)
        {
            dir[bucket_no]->data[i-1]=dir[bucket_no]->data[i];
        }
        dir[bucket_no]->set_occupied(dir[bucket_no]->get_occupied()-1);
        return;
    }
}
void directory::delete_function_2(int key)
{
    if(search_dir(key)==-1)
    {
        //cout<<"Key absent.\n";
        return;
    }
    else
    {
        int bucket_no=hash_function(key);
        int j=search_dir(key);
        for(int i=j+1;i<dir[bucket_no]->get_occupied();i++)
        {
            dir[bucket_no]->data[i-1]=dir[bucket_no]->data[i];
        }
        dir[bucket_no]->set_occupied(dir[bucket_no]->get_occupied()-1);
        int next_bucket_index=next_bucket(bucket_no,(dir[bucket_no]->get_local_depth()));

        if(dir[bucket_no]->get_occupied()+dir[next_bucket_index]->get_occupied()<=dir[bucket_no]->get_capacity())
        {
            if(dir[next_bucket_index]->get_local_depth()==dir[bucket_no]->get_local_depth())
            {
            //cout<<"here";
            int gap=(1<<(dir[bucket_no]->get_local_depth()-1));
            if(gap>1)
            {
                dir[next_bucket_index]->set_local_depth((dir[next_bucket_index]->get_local_depth()-1));
                for(int i=0;i<dir[bucket_no]->get_occupied();i++)
                {
                    dir[next_bucket_index]->data[dir[next_bucket_index]->get_occupied()]=dir[bucket_no]->data[i];
                    dir[next_bucket_index]->set_occupied(dir[next_bucket_index]->get_occupied()+1);
                }
                for(int j=0;j<buckets_timestamp.size();j++)
                {
                    if(buckets_timestamp[j]==dir[bucket_no])
                    {
                        buckets_timestamp.erase(buckets_timestamp.begin()+j);break;
                    }
                }
                delete(dir[bucket_no]);
                for(int i=gap+next_bucket_index;i<dir.size();i+=gap)
                {
                    dir[i]=dir[next_bucket_index];
                }
                for(int i=next_bucket_index-gap;i>=0;i-=gap)
                {
                    dir[i]=dir[next_bucket_index];
                }
            }
            if(get_global_depth()!=global_depth)
            {
                contract_dir();
            }
            }
        }

    }

}
int directory::get_global_depth()
{
    int ans=-1;
    for(int i=0;i<dir.size();i++)
    {
        ans=max(ans,dir[i]->get_local_depth());
    }
    return(ans);
}
int directory::search_dir(int key)
{
    int bucket_index=hash_function(key);
    for(int i=0;i<dir[bucket_index]->get_occupied();i++)
    {
        if(dir[bucket_index]->data[i]==key)
        {
            return i;
        }
    }
    return -1;
}
void directory::print()
{
    cout<<"Global depth:"<<global_depth<<"\n";
    for(int i=0;i<dir.size();i++)
    {
        cout<<setw(2)<<i<<" ("<<dir[i]->get_local_depth()<<") ("<<setw(global_depth)<<bucket_id(i)<<") : ";
        for(int j=0;j<dir[i]->get_occupied();j++)cout<<(dir[i]->data[j])<<" ";
        cout<<"\n";
    }
    cout<<"\n";
}
void directory::print_status()
{
    cout<<global_depth<<"\n";
    set<string>bucket_ids;
    vector<pair<int,int> >answer;
    for(int i=0;i<dir.size();i++)
    {
        if(!bucket_ids.count(bucket_id(i)))
        {
            answer.push_back(make_pair(dir[i]->get_occupied(),dir[i]->get_local_depth()));
            bucket_ids.insert(bucket_id(i));
        }
    }
    cout<<answer.size()<<"\n";
    for(int i=0;i<answer.size();i++)
    {
        cout<<answer[i].first<<" "<<answer[i].second<<"\n";
    }
}
void directory::print_by_time()
{
    cout<<global_depth<<"\n";
    cout<<buckets_timestamp.size()<<"\n";
    for(int i=0;i<buckets_timestamp.size();i++)
    {
        cout<<buckets_timestamp[i]->get_occupied()<<" "<<buckets_timestamp[i]->get_local_depth()<<"\n";
    }
    cout<<"---\n";
}
directory::~directory()
{
    //dtor
}
int main()
{
    int initial_gloabal_deplth,bucket_size;
    cin>>initial_gloabal_deplth>>bucket_size;
    directory d(initial_gloabal_deplth, bucket_size);
    int command;
    while(1)
    {
        cin>>command;
        int value;
        if(command==2)
        {
            cin>>value;
            d.insert_function(value);
        }
        if(command==3)
        {
            cin>>value;
            d.search_dir(value);
            //cout<<d.search_dir(value)<<"\n";
        }
        if(command==4)
        {
            cin>>value;
            //d.delete_function(value);
            d.delete_function_2(value);
            //d.lazy_delete(value);
        }
        if(command==5)
        {
            d.print_by_time();
            d.print();
        }
        if(command==6)
        {
            break;
        }
    }
    return 0;
}

