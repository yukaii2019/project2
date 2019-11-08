#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

template<class T> class list;
template<class T> class queue;
class FCR;
//template<class T> class list_iterator;

template<class T>
class link{
friend class list<T>;
friend class FCR;
friend class queue<T>;
public:
    link(T& x):value(x),next(nullptr){}
private:
    T value;
    link<T>* next;
};
//template<class T>
// class list_iterator{
// friend class list<T>;
// public:
//     typedef list_iterator<T> iterator;
//     list_iterator(): current_link(0){}
//     list_iterator(link<T>* source_link): current_link(source_link){}
//     list_iterator(list_iterator<T>* source_iterator): current_link(source_iterator.current_link){}
// private:
//     link<T>* current_link;
// };

template<class T>
class list{
friend class FCR;
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
    T& front(){
        return first->value;
    }
private:
    link<T>* first;
    link<T>* last;
};

class adjnode{
    friend class list<adjnode>;
    friend class FCR;
public:
    adjnode():r(0),c(0){}
    adjnode(int rr,int cc):r(rr),c(cc){}                     
private:
    int r,c; 
};

template<class T>
class queue{
public:
    queue():first(nullptr),last(nullptr){}
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
    }
    void pop(){
        if(empty())
            return;
        link<T>* deletenode = first;
        if(first == last) last = nullptr;
        first = first->next;
        delete deletenode;
        deletenode = nullptr;
    }  
    void pop_back(){
        if(empty())
            return;
        if(first == last){
            delete last;
            first = nullptr;last = nullptr;
        }
        else{
            link<T>* newlast = first;
            for(;newlast->next!=last;newlast = newlast->next){}
            delete last;
            last = newlast;
            newlast = nullptr;
        }
    }
    bool empty(){
        return first==nullptr&&last==nullptr;
    }
    T& front(){
        return first->value;
    }
    T& back(){
        return last->value;
    }
private:
    link<T>* first;
    link<T>* last;
};



class ElementNode{
friend class FCR;
public:
    ElementNode(){}
    ElementNode(int rr,int cc,char dd):r(rr),c(cc),data(dd),side(0),distance_to_R(0){}

private:   
    list<adjnode> adjlist;
    int distance_to_R;
    int r,c;
    char data;
    int side;
    adjnode parent;
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
    FCR():m(0),n(0),B(0),num_of_0(0),step(0){}
    void read()
    {   
        ifstream rdfile;
        rdfile.open("floor1.data",ios::in);
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
                    num_of_0++;
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
  
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                if((*(matrix+i*n+j))->data != '$'){     
                    if(i!=0&&i!=m-1&&j!=0&&j!=n-1){
                        if((*(matrix+i*n+j+1))->data != '$'){
                            adjnode adjn(i,j+1);
                            (*(matrix+i*n+j))->adjlist.push_back(adjn);
                            (*(matrix+i*n+j))->side++;
                        }
                        if((*(matrix+(i-1)*n+j))->data != '$'){
                            adjnode adjn(i-1,j);
                            (*(matrix+i*n+j))->adjlist.push_back(adjn);
                            (*(matrix+i*n+j))->side++;
                        }
                        if((*(matrix+i*n+j-1))->data != '$'){
                            adjnode adjn(i,j-1);
                            (*(matrix+i*n+j))->adjlist.push_back(adjn);
                            (*(matrix+i*n+j))->side++;
                        }
                        if((*(matrix+(i+1)*n+j))->data != '$'){
                            adjnode adjn(i+1,j);
                            (*(matrix+i*n+j))->adjlist.push_back(adjn);
                            (*(matrix+i*n+j))->side++;
                        }
                    }
                    else{
                        if(i==0)        {adjnode adjn(i+1,j);(*(matrix+i*n+j))->adjlist.push_back(adjn);(*(matrix+i*n+j))->side++;}
                        else if(i==m-1) {adjnode adjn(i-1,j);(*(matrix+i*n+j))->adjlist.push_back(adjn);(*(matrix+i*n+j))->side++;}
                        else if(j==0)   {adjnode adjn(i,j+1);(*(matrix+i*n+j))->adjlist.push_back(adjn);(*(matrix+i*n+j))->side++;}
                        else if(j==n-1) {adjnode adjn(i,j-1);(*(matrix+i*n+j))->adjlist.push_back(adjn);(*(matrix+i*n+j))->side++;}
                    }       
                }
            }
        }
        
        rdfile.close();      
    } 
    void test()
    {
      
    }
    void BFS_FindDisranceToR(){
        visited = new bool* [m];
        int disc = 0;     
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
                    matrix[n*curr->value.r+curr->value.c]->parent = node;
                    q.push(curr->value);
                    q_DistanceSorted.push(curr->value);
                }
            }
        }
        for(int i=0;i<m;i++)
            delete[] visited[i];
        delete []visited;

        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                cout <<setw(3)<< matrix[i*n+j]->distance_to_R << " ";
            }
            cout << endl;
        }
        cout << endl;
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                cout << matrix[i*n+j]->side << " ";
            }
            cout << endl;
        }
        cout << endl;
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                cout << matrix[i*n+j]->parent.r <<matrix[i*n+j]->parent.c << " ";
            }
            cout << endl;
        }
    }
    void go_to(adjnode dest)
    {   
        queue<adjnode> q;
        adjnode tmp(dest.r,dest.c);
        while(!(tmp.r==R.r&&tmp.c==R.c)){
            q.push(tmp);
            tmp = matrix[n*tmp.r+tmp.c]->parent;
        }
        for(;!q.empty();q.pop_back()){ 
            tmp = q.back();
            visited[tmp.r][tmp.c] = true;
            step++;
        }
        int d = matrix[n*tmp.r+tmp.c]->distance_to_R-1;
        int w = 0;
        while(!(tmp.r==R.r&&tmp.c==R.c)){
            w = 0;
            for(link<adjnode>* a = matrix[n*tmp.r+tmp.c]->adjlist.first;a!=nullptr;a=a->next){
                if(matrix[n*a->value.r+a->value.c]->distance_to_R==d){
                    if(visited[a->value.r][a->value.c]==false){
                        q.push(a->value);
                        w++;
                    }
                }
            }
            if(w!=0){
                tmp = q.back();   //front還是back好
                visited[tmp.r][tmp.c] = true;
                for(;!q.empty();q.pop()){}       
            }
            else 
                tmp = matrix[n*tmp.r+tmp.c]->parent;
            d--; 
            step++; 
        }

        cout << "step: " << step << endl;
        for(int i=0;i<m;i++){
            for(int j=0;j<m;j++){
                cout << visited[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
    void traverse(){
        
        visited = new bool* [m];    
        for(int i=0;i<m;i++)
            visited[i] = new bool[n];
        for(int i=0;i<m;i++)
            for(int j=0;j<n;j++)
                visited[i][j] = false;
        while(!q_DistanceSorted.empty()){
            adjnode node = q_DistanceSorted.back();
            q_DistanceSorted.pop_back();
            if(visited[node.r][node.c]==false)
                go_to(node);
        }
        for(int i=0;i<m;i++)
            delete[] visited[i];
        delete []visited;
    }
private:
    adjnode R;
    int m,n,B;
    ElementNode** matrix;
    bool** visited;
    int num_of_0;
    queue<adjnode> q_DistanceSorted;
    int step;
};


int main(){
    FCR fcr;
    fcr.read();
    fcr.BFS_FindDisranceToR();
    fcr.test();
    fcr.traverse();
    
    

}