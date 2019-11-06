#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
using namespace std;

template<class T> class list;

template<class T>
class link{
friend class list<T>;
public:
    link(T& x):value(x),next(nullptr){}
private:
    T value;
    link<T>* next;
};

template<class T>
class list{
public:
    list():first(nullptr),last(nullptr){}
    void push_back(T& n){
        link<T>* tmp = new link<T>(n);
        if(empty()){
            first = tmp;
            last = tmp;
        }            
        else{
            last->next = tmp;
            last = tmp;
        }        
    }
    bool empty(){
        return first==nullptr;
    }
    void showlist(){
        link<T>* curr = first;
        while(curr){
            cout << curr->value.r << " " << curr->value.c << endl;
            curr = curr->next;
        }
    }
private:
    link<T>* first;
    link<T>* last;
};

class adjnode{
public:
    adjnode(int rr,int cc):r(rr),c(cc){}
    int r,c;                                //怎麼寫進private
private:
    
};

class ElementNode{
friend class FCR;
public:
    ElementNode(){}
    ElementNode(int rr):r(rr),c(-1),data('$'){}
    ElementNode(int rr,int cc,char dd):r(rr),c(cc),data(dd){}

private:   
    list<adjnode> adjlist;
    int r,c;
    char data;
    int side = 0;
    bool dir[4]={0,0,0,0};
};

class PathNode{
friend class FCR;
public:
    PathNode(int rr,int cc):r(rr),c(cc),next(nullptr){}
private:
    int r,c;
    PathNode* next;
};

class FCR{
public:

    // void pushbackrow(ElementNode* e,ElementNode* n,int i)
    // {
    //     if(e==nullptr)row[i] = n;
    //     else{
    //         for(;e->right!=nullptr;e=e->right){} 
    //         e->right = n;
    //         n->left = e;
    //     }
    // }
    // void pushbackcol(ElementNode* e,ElementNode* n,int i)
    // {
    //     if(e==nullptr)col[i] = n;
    //     else{
    //         for(;e->down!=nullptr;e=e->down){} 
    //         e->down = n;
    //         n->up = e;
    //     }
    // }
    // void printrow(ElementNode* f)
    // {
    //     while(f!=nullptr){
    //         cout<< f->data;
    //         f=f->right;
    //     }
    // }
    // void printrowback(ElementNode* b)
    // {
    //     while(b!=nullptr){
    //         if(b->right == nullptr) break;
    //         b= b->right;
    //     }
    //     while(b!=nullptr){
    //         cout << b->data;
    //         b=b->left;
    //     }
    // }
    // void printcol(ElementNode* f)
    // {
    //     while(f!=nullptr){
    //         cout<< f->data;
    //         f=f->down;
    //     }
        
    // }
    void read()
    {   
        ifstream rdfile;
        rdfile.open("floor.data",ios::in);
        if(!rdfile){
           cout<<"error";
        }
        rdfile >> m >> n >> B;
        matrix = new ElementNode* [m*n];
        char d;
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                rdfile >> d;
                ElementNode* tmp;
                if(d=='0')
                    tmp = new ElementNode(i,j,'#');
                else if(d=='1')
                    tmp = new ElementNode(i,j,'$');
                else 
                    tmp = new ElementNode(i,j,'R');  
                *(matrix+i*n+j) = tmp;
            }
        }
                
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                if((*(matrix+i*n+j))->data != '$'){     
                    if(i!=0&&i!=m-1&&j!=0&&j!=n-1){
                        if((*(matrix+i*n+j+1))->data != '$'){
                            adjnode adjn(i,j+1);
                            (*(matrix+i*n+j))->adjlist.push_back(adjn);
                        }
                        if((*(matrix+(i-1)*n+j))->data != '$'){
                            adjnode adjn(i-1,j);
                            (*(matrix+i*n+j))->adjlist.push_back(adjn);
                        }
                        if((*(matrix+i*n+j-1))->data != '$'){
                            adjnode adjn(i,j-1);
                            (*(matrix+i*n+j))->adjlist.push_back(adjn);
                        }
                        if((*(matrix+(i+1)*n+j))->data != '$'){
                            adjnode adjn(i+1,j);
                            (*(matrix+i*n+j))->adjlist.push_back(adjn);
                        }
                    }
                    else{
                        if(i==0)        {adjnode adjn(i+1,j);(*(matrix+i*n+j))->adjlist.push_back(adjn);}
                        else if(i==m-1) {adjnode adjn(i-1,j);(*(matrix+i*n+j))->adjlist.push_back(adjn);}
                        else if(j==0)   {adjnode adjn(i,j+1);(*(matrix+i*n+j))->adjlist.push_back(adjn);}
                        else if(j==n-1) {adjnode adjn(i,j-1);(*(matrix+i*n+j))->adjlist.push_back(adjn);}
                    }       
                }
            }
        }

        
        // row = new ElementNode* [m];
        // col = new ElementNode* [n];
        // memset(row,0,m);
        // memset(col,0,n);
        // char d;
        // for(int i=0;i<m;i++){
        //     for(int j=0;j<n;j++){
        //         rdfile >> d;
        //         ElementNode *tmp;
        //         if(d =='1'){           
        //             tmp = new ElementNode(i,j,'#');           
        //         }
        //         else if(d == '0'){
        //             tmp = new ElementNode(i,j,'$');         
        //         } 
        //         else if(d =='R'){
        //             R[0]=i,R[1]=j; 
        //             tmp = new ElementNode(i,j,'R');         
        //         }  
        //         pushbackrow(row[i],tmp,i);
        //         pushbackcol(col[j],tmp,j);    
        //     }
        // }
        rdfile.close();      
    }
    void test()
    {
        (*(matrix+2*n+2))->adjlist.showlist();
    }
    // void setdir()
    // {
    //     ElementNode* curr;
    //     for(int i=0;i<m;i++){
    //         curr = row[i];
    //         for(;curr!=nullptr;curr = curr->right){
    //             if(curr->data!='#'){
    //                 if(curr->right!=nullptr&&curr->right->data =='#') curr->dir[0] = 1;  
    //                 if(curr->left!=nullptr&&curr->left->data =='#') curr->dir[2] = 1;
    //             }
    //         }
    //     }
    //     for(int i=0;i<n;i++){
    //         curr = col[i];
    //         for(;curr!=nullptr;curr = curr->down){
    //             if(curr->data!='#'){
    //                 if(curr->down!=nullptr&&curr->down->data =='#') curr->dir[3] = 1;  
    //                 if(curr->up!=nullptr&&curr->up->data =='#') curr->dir[1] = 1;
                
    //                 if((curr->dir[0]==1&&curr->dir[2]==1)||(curr->dir[1]==1&&curr->dir[3]==1))curr->data = '%';
    //             }
    //         }
    //     }
    // }
private:
    int R[2];
    int m,n,B;
    // ElementNode** row;
    // ElementNode** col;
    ElementNode** matrix;
};


int main(){
    FCR fcr;
    fcr.read();
    fcr.test();

}