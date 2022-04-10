//#include <bits/stdc++.h>
#include <iostream>
#include <vector>

using namespace std;

class node
{
    public:
        node(bool,int);
        virtual ~node ();
        vector<int>data;
        vector<node*>children;
        bool isFull();
        bool isLeaf();
        void setCapacity(int);
        int getCapacity();
        node* get_parent();
        void set_parent(node*);
        void insert_in_node(int);
        void insert_child_in_node(node*);
    protected:
    private:
        bool type;
        int capacity;
        node* parent;
};
node::node(bool leaf,int c)
{
    type=leaf;
    capacity=c;
    parent=nullptr;
}
bool node::isLeaf()
{
    if(type)return true;
    else return false;
}
node* node::get_parent()
{
    return parent;
}
void node::set_parent(node* ptr)
{
    parent=ptr;
    return;
}
void node::setCapacity(int c)
{
    capacity=c;
}
int node::getCapacity()
{
    return capacity;
}
bool node::isFull()
{
    if(data.size()==capacity)
    {
        return true;
    }
    return false;
}
void node::insert_in_node(int key)
{
    int index=0;
    while(index<data.size() && data[index]<key)index++;
    data.insert(data.begin()+index,key);
    return;
}
void node::insert_child_in_node(node* ptr)
{
    int index=0;
    while(index<children.size() && ((children[index]->data[0])<(ptr->data[0])))index++;
    children.insert(children.begin()+index,ptr);
    return;
}
node::~node()
{

}
class BplusTree
{
    public:
        BplusTree(node*,int,int);
        virtual ~BplusTree ();
        int get_d();
        int get_t();
        int get_index_capacity();
        int get_leaf_capacity();
        void insert_function(int);
        node* search_tree(int);
        void print();
        node* get_root();
        void print_status();
    protected:
    private:
        node* root;
        int d;
        int t;
        void insert_in_tree(node*,int);
        void print_preorder(node*);
        void split_index_node(node*);
        void print_status_in_tree(node*);
        node* search_in_tree(node*,int);

};
BplusTree::BplusTree(node* r,int dtmp,int ttmp)
{
    root=r;
    if(root!=nullptr)
    {
        root->set_parent(nullptr);
    }
    d=dtmp;
    t=ttmp;
}
int BplusTree::get_d()
{
    return(d);
}
int BplusTree::get_t()
{
    return(t);
}
node* BplusTree::get_root()
{
    return root;
}
int BplusTree::get_index_capacity()
{
    return((2*t)+1);
}
int BplusTree::get_leaf_capacity()
{
    return(2*d);
}
BplusTree:: ~BplusTree ()
{

}
void BplusTree::insert_function(int key)
{
    insert_in_tree(root, key);
}
void BplusTree::split_index_node(node* r)
{
    int t=get_t();
    int ic=get_index_capacity();
    if(r==nullptr)return;
    node* p=r->get_parent();
    int mid=r->data[t];
    node* n2=new node(0,ic);
    for(int i=1+t;i<r->data.size();i++)
    {
        n2->data.push_back(r->data[i]);
    }
    int initial_size=r->data.size();
    for(int i=t;i<initial_size;i++)
    {
        r->data.pop_back();
    }
    for(int i=t+1;i<r->children.size();i++)
    {
        n2->children.push_back(r->children[i]);
        (r->children[i])->set_parent(n2);
    }
    initial_size=r->children.size();
    for(int i=t+1;i<initial_size;i++)
    {
        r->children.pop_back();
    }
    if(p==nullptr)
    {
        p=new node(0,ic);
        p->data.push_back(mid);
        p->children.push_back(r);
        p->children.push_back(n2);
        n2->set_parent(p);
        r->set_parent(p);
        root=p;
        root->set_parent(nullptr);
        return;
    }
    p->insert_in_node(mid);
    p->insert_child_in_node(n2);
    if(!p->isFull())
    {
        return;
    }
    else
    {
        split_index_node(p);
    }
}
void BplusTree::insert_in_tree(node* r, int key)
{
    int d=get_d();
    int t=get_t();
    int lc=get_leaf_capacity();
    int ic=get_index_capacity();
    if(root==nullptr)
    {
        root=new node(1,lc);
        root->insert_in_node(key);
        root->set_parent(nullptr);
        return;
    }
    if(!r->isLeaf())
    {
        int next=r->data.size()-1;
        for(int i=0;i<r->data.size();i++)
        {
            if(r->data[i]>key)
            {
                next=i-1;
                break;
            }
        }
        insert_in_tree(r->children[next+1], key);
    }
    if(r->isLeaf())
    {
        if(!r->isFull())
        {
            //cout<<"required leaf has space.\n";
            r->insert_in_node(key);
            return;
        }
        else
        {
            //split leaf
            //cout<<"splitting leaf.\n";
            r->insert_in_node(key);
            node* l2;
            l2=new node(1,lc);
            l2->set_parent(r->get_parent());
            for(int i=d;i<r->data.size();i++)
            {
                l2->data.push_back(r->data[i]);
            }
            int initial_size=r->data.size();
            for(int i=d;i<initial_size;i++)
            {
                r->data.pop_back();
            }
            if(r==root)
            {
                node* new_child_indicator=new node(0,ic);
                new_child_indicator->data.push_back(l2->data[0]);
                new_child_indicator->children.push_back(r);
                new_child_indicator->children.push_back(l2);
                r->set_parent(new_child_indicator);
                l2->set_parent(new_child_indicator);
                root=new_child_indicator;
                root->set_parent(nullptr);
            }
            else
            {
                if(!((r->get_parent())->isFull()))
                {
                    (r->get_parent())->insert_in_node(l2->data[0]);
                    (r->get_parent())->insert_child_in_node(l2);
                }
                else
                {
                    //cout<<"parent full";

                    (r->get_parent())->insert_in_node(l2->data[0]);
                    (r->get_parent())->insert_child_in_node(l2);
                    l2->set_parent(r->get_parent());
                    split_index_node(r->get_parent());
                }
            }
            return;
        }
    }
}
void BplusTree::print()
{
    cout<<"\n";
    print_preorder(root);
}
void BplusTree::print_preorder(node* r)
{
    if(r==nullptr)return;
    cout<<"("<<r->isLeaf()<<")";
    for(int i=0;i<r->data.size();i++)
    {
        cout<<r->data[i]<<" ";
    }
    cout<<"\n";
    for(int i=0;i<r->children.size();i++)
    {
        print_preorder(r->children[i]);
    }
}
int data_count=0;
int index_count=0;
void BplusTree::print_status()
{
    index_count=0,data_count=0;
    print_status_in_tree(root);
    cout<<index_count<<" "<<data_count<<" ";
    if(root==nullptr)
    {
        cout<<"\n";
        return;
    }
    for(int i=0;i<(root->data.size());i++)
    {
        cout<<root->data[i]<<" ";
    }
    cout<<"\n";
}
void BplusTree::print_status_in_tree(node* r)
{
    if(r==nullptr)return;
    if(r->isLeaf())
    {
        data_count++;
    }
    else
    {
        index_count++;
    }
    for(int i=0;i<r->children.size();i++)
    {
        print_status_in_tree(r->children[i]);
    }
}
node* BplusTree::search_in_tree(node* r,int key)
{
    if(r==nullptr)
    {
        return nullptr;
    }
    if(!r->isLeaf())
    {
        int next=r->data.size()-1;
        for(int i=0;i<r->data.size();i++)
        {
            if(r->data[i]>key)
            {
                next=i-1;
                break;
            }
        }
        return(search_in_tree(r->children[next+1], key));
    }
    else
    {
        int f=0;
        for(int i=0;i<r->data.size();i++)
        {
            if(r->data[i]==key)
            {
                f=1;
                return r;
            }
        }
        if(f==0)
        {
            return nullptr;
        }
    }
    return nullptr;

}
node* BplusTree::search_tree(int key)
{
   return search_in_tree(root, key);
}
int main()
{
    int d,t,command,key;
    cin>>d>>t;
    BplusTree b(nullptr,d,t);
    node* r=nullptr;
    while(cin>>command)
    {
        if(command==1)
        {
            cin>>key;
            b.insert_function(key);
        }
        if(command==2)
        {
            b.print_status();
        }
        if(command==3)
        {
            break;
        }
        if(command==4)
        {
            cin>>key;
            node* p=b.search_tree(key);
            if(p==nullptr)
            {
                cout<<"not found.\n";
            }
            else cout<<"found.\n";
        }
    }
    return 0;
}
