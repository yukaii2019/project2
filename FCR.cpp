#include <iostream>
#include <fstream>
#include <time.h>
using namespace std;
template<class T> class stack;
template<class T> class queue;
class FCR;

template<class T>
class link{
friend class FCR;
friend class stack<T>;
friend class queue<T>;
public:
    link(T& x):value(x),next(nullptr){}
    ~link(){}
private:
    T value;
    link<T>* next;
};

class adjnode{
    friend class FCR;
public:
    adjnode():r(0),c(0){}
    adjnode(int rr,int cc):r(rr),c(cc){}         
    ~adjnode(){}            
private:
    int r,c; 
};

template<class T>
class queue{
friend class FCR;
public:
    queue():first(nullptr),last(nullptr),my_size(0){}
    ~queue(){}
    void push(T& n){
        link<T>* tmp = new link<T>(n);
        if(empty()){
            first = tmp;
            last = tmp;
        }            
        else{
            last->next = tmp;
            last = tmp;
        } 
        my_size++;
    }
    void pop(){
        if(empty())
            return;
        link<T>* deletenode = first;
        first = first->next;
        delete deletenode;
        my_size--;
    }  
    bool empty(){
        return first == nullptr;
    }
    T& front(){
        return first->value;
    }
    int size(){
        return my_size;
    }
private:
    link<T>* first;
    link<T>* last;
    int my_size;
};

template<class T>
class stack{
friend class FCR;
public:
    stack():topp(nullptr),my_size(0){}
    ~stack(){}
    void push(T& n){
        link<T>* tmp = new link<T>(n);
        if(empty()){
            topp = tmp;
        }            
        else{
            tmp->next = topp;
            topp = tmp;
        } 
        my_size++;
    }
    void pop(){
        if(empty())
            return;
        link<T>* deletenode = topp;
        topp = topp->next;
        delete deletenode;
        my_size--;
    }  
    bool empty(){
        return topp == nullptr;
    }
    T& top(){
        return topp->value;
    }
    int size(){
        return my_size;
    }
    void clean(){
        if(empty())
            return;
        link<adjnode>* newtop;
        while(!empty()){
            newtop = topp->next;
            delete topp;
            topp = newtop;
        }
        my_size = 0;
    }
private:
    link<T>* topp;
    int my_size;
};

class ElementNode{
friend class FCR;
public:
    ElementNode(){}
    ElementNode(int rr,int cc,char dd):r(rr),c(cc),data(dd),distance_to_R(0){}
    ~ElementNode(){}
private:   
    queue<adjnode> adjlist;
    int distance_to_R;
    int r,c;
    char data;
    adjnode parent;
};

class FCR{
public:
    FCR():m(0),n(0),B(0){}
    ~FCR(){}
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
                if(d=='0'){
                    tmp = new ElementNode(i,j,'#');
                }
                else if(d=='1')
                    tmp = new ElementNode(i,j,'$');
                else{
                    tmp = new ElementNode(i,j,'R');  
                    R.r = i;
                    R.c = j;
                }
                *(matrix+i*n+j) = tmp;
            }
        }
        rdfile.close();
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                if((*(matrix+i*n+j))->data != '$'){     
                    if(i!=0&&i!=m-1&&j!=0&&j!=n-1){
                        if((*(matrix+i*n+j+1))->data != '$'){
                            adjnode adjn(i,j+1);
                            (*(matrix+i*n+j))->adjlist.push(adjn);
                        }
                        if((*(matrix+(i-1)*n+j))->data != '$'){
                            adjnode adjn(i-1,j);
                            (*(matrix+i*n+j))->adjlist.push(adjn);
                        }
                        if((*(matrix+i*n+j-1))->data != '$'){
                            adjnode adjn(i,j-1);
                            (*(matrix+i*n+j))->adjlist.push(adjn);
                        }
                        if((*(matrix+(i+1)*n+j))->data != '$'){
                            adjnode adjn(i+1,j);
                            (*(matrix+i*n+j))->adjlist.push(adjn);
                        }
                    }
                    else{
                        if((i==0&&j==0)||(i==0&&j==n-1)||(i==m-1&&j==0)||(i==m-1&&j==n-1)){}
                        else if(i==0)   {adjnode adjn(i+1,j);(*(matrix+i*n+j))->adjlist.push(adjn);}
                        else if(i==m-1) {adjnode adjn(i-1,j);(*(matrix+i*n+j))->adjlist.push(adjn);}
                        else if(j==0)   {adjnode adjn(i,j+1);(*(matrix+i*n+j))->adjlist.push(adjn);}
                        else if(j==n-1) {adjnode adjn(i,j-1);(*(matrix+i*n+j))->adjlist.push(adjn);}
                    }  
                }
            }
        }      
    } 
    void BFS_FindDisranceToR(){
        visited = new bool* [m];
        int disc = 0;   
        //int maxdis = -1;  
        link<adjnode>* curr;

        for(int i=0;i<m;i++)
            visited[i] = new bool [n];
        
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                visited[i][j] = false;
            }
        }
        
        queue<adjnode> q;
        visited[R.r][R.c] = true;
        matrix[n*R.r+R.c]->distance_to_R = disc;
        q.push(R);
        while(!q.empty()){
            adjnode node = q.front();  
            q.pop(); 
            for(curr = matrix[n*node.r+node.c]->adjlist.first;curr!=nullptr;curr = curr->next){
                if(visited[curr->value.r][curr->value.c]==false){
                    visited[curr->value.r][curr->value.c] = true;             
                    matrix[n*curr->value.r+curr->value.c]->distance_to_R = matrix[n*node.r+node.c]->distance_to_R+1;
                    //if(matrix[n*node.r+node.c]->distance_to_R+1>maxdis)maxdis = matrix[n*node.r+node.c]->distance_to_R+1;//可以刪掉
                    matrix[n*curr->value.r+curr->value.c]->parent = node;
                    q.push(curr->value);
                }
            }
        }
        //cout << maxdis << endl;
        for(int i=0;i<m;i++)
            delete[] visited[i];
        delete []visited;
    }
    void DFS(adjnode dest)
    {   
        int batt = B;
        stack<adjnode> s_haspass;  
        stack<adjnode> go_to;   
        adjnode tmp(dest.r,dest.c);
        visited[tmp.r][tmp.c] = true;
        while(!(tmp.r==R.r&&tmp.c==R.c)){
            go_to.push(tmp);
            tmp = matrix[n*tmp.r+tmp.c]->parent;
        }
        batt-=go_to.my_size;
        for(;!go_to.empty();go_to.pop()){ 
            s_haspass.push(tmp);
            tmp = go_to.top();
            for(link<adjnode>* a=matrix[n*tmp.r+tmp.c]->adjlist.first;a!=nullptr;a=a->next){
                if(visited[a->value.r][a->value.c]==false)
                    NotVisit.push(a->value);     
            }
            visited[tmp.r][tmp.c] = true;
            ans_list.push(tmp);
        }
        int d = matrix[n*tmp.r+tmp.c]->distance_to_R;

        while(batt>d){
            int path = 0;
            for(link<adjnode>* a=matrix[n*tmp.r+tmp.c]->adjlist.first;a!=nullptr;a=a->next){
                    if(visited[a->value.r][a->value.c]==false){
                        NotVisit.push(a->value);
                        path++;
                    }
            }           
            if(path!=0){
                s_haspass.push(tmp);
                tmp = NotVisit.top();
                visited[tmp.r][tmp.c] = true;
                batt--;
                if(matrix[n*NotVisit.top().r+NotVisit.top().c]->distance_to_R<=batt){
                    ans_list.push(tmp);
                    NotVisit.pop();
                }
                else{
                    visited[tmp.r][tmp.c] = false;
                    tmp = s_haspass.top();
                    batt++;
                    break;
                }
                
            }
            else if(path == 0){
                if(!s_haspass.empty()){  
                    batt--;
                    if(matrix[n*s_haspass.top().r+s_haspass.top().c]->distance_to_R<=batt){
                        tmp = s_haspass.top();
                        ans_list.push(tmp);
                        s_haspass.pop();
                    }
                    else break;
                }
                else break;
            }

            d = matrix[n*tmp.r+tmp.c]->distance_to_R;

            if(batt == d){
                for(link<adjnode>* a=matrix[n*tmp.r+tmp.c]->adjlist.first;a!=nullptr;a=a->next){
                    if(visited[a->value.r][a->value.c]==false){
                        NotVisit.push(a->value);
                        path++;
                    }
                }
                break;
            }          
        }
        while(!(tmp.r==R.r&&tmp.c==R.c)){
            tmp = matrix[n*tmp.r+tmp.c]->parent;
            for(link<adjnode>* a=matrix[n*tmp.r+tmp.c]->adjlist.first;a!=nullptr;a=a->next){
                if(visited[a->value.r][a->value.c]==false)
                    NotVisit.push(a->value);     
            }
            visited[tmp.r][tmp.c] = true;
            ans_list.push(tmp);
        }
        s_haspass.clean();
    }
    
    void traverse(){
        fstream d_file;
        visited = new bool* [m];    
        for(int i=0;i<m;i++)
            visited[i] = new bool[n];
        for(int i=0;i<m;i++)
            for(int j=0;j<n;j++)
                visited[i][j] = false;
        NotVisit.push(R);
        while(!NotVisit.empty()){
            adjnode node = NotVisit.top();
            NotVisit.pop();
            if(visited[node.r][node.c]==false){
                DFS(node);
            }           
        }
        for(int i=0;i<m;i++)
            delete[] visited[i];
        delete []visited;
    }
    void write(){
        fstream opfile;
        opfile.open("final.path",ios::out);
        if(!opfile)
            cout << "error";
        cout << ans_list.size() <<endl;
        opfile << ans_list.size() <<endl;
        opfile << R.r << " " << R.c<<endl;
        for(;!ans_list.empty();ans_list.pop()){
            opfile << ans_list.front().r << " " << ans_list.front().c << endl;
        } 
        opfile.close(); 
    }
private:
    adjnode R;
    int m,n,B;
    ElementNode** matrix;
    bool** visited;
    queue<adjnode> ans_list;
    stack<adjnode> NotVisit;
};
int main(){
    FCR fcr;
    fcr.read();
    fcr.BFS_FindDisranceToR();
    fcr.traverse();
    fcr.write();
    cout << (double)clock() / CLOCKS_PER_SEC << "S" << endl;
}