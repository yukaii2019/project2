#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

//template<class T> class list;
template<class T> class queue;
class FCR;
//template<class T> class list_iterator;

template<class T>
class link{
//friend class list<T>;
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

// template<class T>
// class list{
// friend class FCR;
// public:
//     list():first(nullptr),last(nullptr){}
//     void push_back(T& n){
//         link<T>* tmp = new link<T>(n);
//         if(empty()){
//             first = tmp;
//             last = tmp;
//         }            
//         else{
//             last->next = tmp;
//             last = tmp;
//         }        
//     }
//     bool empty(){
//         return first==nullptr;
//     }
//     void showlist(){
//         link<T>* curr = first;
//         while(curr){
//             cout << curr->value.r << " " << curr->value.c << endl;
//             curr = curr->next;
//         }
//     }
//     T& front(){
//         return first->value;
//     }
// private:
//     link<T>* first;
//     link<T>* last;
// };

class adjnode{
    //friend class list<adjnode>;
    friend class FCR;
public:
    adjnode():r(0),c(0){}
    adjnode(int rr,int cc):r(rr),c(cc){}                     
private:
    int r,c; 
};

template<class T>
class queue{
friend class FCR;
public:
    queue():first(nullptr),last(nullptr),my_size(0){}
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
        if(first == last) last = nullptr;
        first = first->next;
        delete deletenode;
        deletenode = nullptr;
        my_size--;
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
        my_size--;
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
    int size(){
        return my_size;
    }
    void clean(){
        for(;!empty();pop()){}
        my_size = 0;
    }
private:
    link<T>* first;
    link<T>* last;
    int my_size;
};



class ElementNode{
friend class FCR;
public:
    ElementNode(){}
    ElementNode(int rr,int cc,char dd):r(rr),c(cc),data(dd),side(0),distance_to_R(0),dead_end(0){}

private:   
    queue<adjnode> adjlist;
    int distance_to_R;
    int r,c;
    char data;
    int side;
    int dead_end;
    int dead_len[3] = {0};
    adjnode endstart[3];
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
        rdfile.close();
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                int s = 0;
                if((*(matrix+i*n+j))->data != '$'){     
                    if(i!=0&&i!=m-1&&j!=0&&j!=n-1){
                        if((*(matrix+i*n+j+1))->data != '$'){
                            adjnode adjn(i,j+1);
                            (*(matrix+i*n+j))->adjlist.push(adjn);
                            s++;
                        }
                        if((*(matrix+(i-1)*n+j))->data != '$'){
                            adjnode adjn(i-1,j);
                            (*(matrix+i*n+j))->adjlist.push(adjn);
                            s++;
                        }
                        if((*(matrix+i*n+j-1))->data != '$'){
                            adjnode adjn(i,j-1);
                            (*(matrix+i*n+j))->adjlist.push(adjn);
                            s++;
                        }
                        if((*(matrix+(i+1)*n+j))->data != '$'){
                            adjnode adjn(i+1,j);
                            (*(matrix+i*n+j))->adjlist.push(adjn);
                            s++;
                        }
                    }
                    else{
                        if(i==0)        {adjnode adjn(i+1,j);(*(matrix+i*n+j))->adjlist.push(adjn);s++;}
                        else if(i==m-1) {adjnode adjn(i-1,j);(*(matrix+i*n+j))->adjlist.push(adjn);s++;}
                        else if(j==0)   {adjnode adjn(i,j+1);(*(matrix+i*n+j))->adjlist.push(adjn);s++;}
                        else if(j==n-1) {adjnode adjn(i,j-1);(*(matrix+i*n+j))->adjlist.push(adjn);s++;}
                    }  
                    (*(matrix+i*n+j))->side = s;     
                    if(s==1&&!(i==R.r&&j==R.c)){
                        adjnode a(i,j);
                        q_DeadEnd.push(a);
                    }
                }
            }
        }      
    } 
    void SetDeadEnd()
    {
        while(!q_DeadEnd.empty()){
            int len = 1;
            adjnode D = q_DeadEnd.front();
            q_DeadEnd.pop();
            adjnode parentloc(D.r,D.c);
            D = matrix[n*D.r+D.c]->adjlist.front();
            while(matrix[n*D.r+D.c]->side==2){
                len++;
                for(link<adjnode>* curr = matrix[n*D.r+D.c]->adjlist.first;curr!=nullptr;curr=curr->next){
                    if(!(curr->value.r==parentloc.r&&curr->value.c==parentloc.c)){
                        parentloc = D;
                        D = curr->value;
                        break;
                    }
                }
            }
            matrix[n*D.r+D.c]->dead_len[matrix[n*D.r+D.c]->dead_end] = len;  
            matrix[n*D.r+D.c]->endstart[matrix[n*D.r+D.c]->dead_end] = parentloc;
            matrix[n*D.r+D.c]->dead_end++;        
        }    
    }
    void test()
    {
        SetDeadEnd();
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                cout << matrix[i*n+j]->dead_len[0] << " ";
            }
            cout << endl;
        }
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
        int s=0,batt=B;
        queue<adjnode> q;
        adjnode tmp(dest.r,dest.c);
        while(!(tmp.r==R.r&&tmp.c==R.c)){
            q.push(tmp);
            tmp = matrix[n*tmp.r+tmp.c]->parent;
        }
        for(;!q.empty();q.pop_back()){ 
            tmp = q.back();
            visited[tmp.r][tmp.c] = true;
            s++;
        }
        batt -= s;
        int d = matrix[n*tmp.r+tmp.c]->distance_to_R-1;

        while(!(tmp.r==R.r&&tmp.c==R.c)){
            for(link<adjnode>* a = matrix[n*tmp.r+tmp.c]->adjlist.first;a!=nullptr;a=a->next){
                if(matrix[n*a->value.r+a->value.c]->distance_to_R==d){
                    if(visited[a->value.r][a->value.c]==false){
                        q.push(a->value);
                    }
                }
            }
            adjnode a1;
            adjnode a2;         //選哪個好
            adjnode a3;
            bool save = false;
            int enterdead = -1;
            if(q.size()>=2){  
                a1 = q.front();
                a2 = q.back();         
                for(link<adjnode>* i= matrix[n*a1.r+a1.c]->adjlist.first;i!=nullptr;i=i->next)
                    for(link<adjnode>* j=matrix[n*a2.r+a2.c]->adjlist.first;j!=nullptr;j=j->next)
                        if((i->value.r == j->value.r&&i->value.c == j->value.c)&&(i->value.r!=tmp.r&&i->value.c!=tmp.c)&&(batt-matrix[n*tmp.r+tmp.c]->distance_to_R>=2)){
                            a3 = i->value;
                            save = true;
                        }                      
            }
            int battneed;
            if(matrix[n*tmp.r+tmp.c]->dead_end !=0 ){
                for(int i=0;i<matrix[n*tmp.r+tmp.c]->dead_end;i++){
                    int start_r = matrix[n*tmp.r+tmp.c]->endstart[i].r;
                    int start_c = matrix[n*tmp.r+tmp.c]->endstart[i].c;
                    battneed = 2*matrix[n*tmp.r+tmp.c]->dead_len[i];
                    if(visited[start_r][start_c]==false)
                        if(batt-battneed>=matrix[n*tmp.r+tmp.c]->distance_to_R)
                            enterdead = i;     
                }
            }
            if(enterdead != -1){
                queue<adjnode> q_forback;
                q_forback.push(tmp);
                tmp = matrix[n*tmp.r+tmp.c]->endstart[enterdead];
                while(matrix[n*tmp.r+tmp.c]->side!=1){
                    visited[tmp.r][tmp.c] = true;
                    q_forback.push(tmp);
                    for(link<adjnode>* a = matrix[n*tmp.r+tmp.c]->adjlist.first;a!=nullptr;a=a->next){
                        if(!(a->value.r==tmp.r&&a->value.c==tmp.c))
                            tmp = a->value;
                    }
                }
                visited[tmp.r][tmp.c] = true;
                for(;!q_forback.empty();q_forback.pop_back()){
                    tmp = q_forback.back();
                    visited[tmp.r][tmp.c] = true;
                }
                batt -= battneed;
                s += battneed;
                d++;
                cout << "cc";
            }
            else if(save == true){
                visited[a1.r][a1.c] = true;
                visited[a3.r][a3.c] = true;
                visited[a2.r][a2.c] = true;
                tmp = a2;
                batt-=3;
                s+=3;
                q.clean();
            }
            else if(!q.empty()){
                tmp = q.back();   //front還是back好
                visited[tmp.r][tmp.c] = true;
                batt--;
                s++;
                q.clean();     
            }
            else{
                tmp = matrix[n*tmp.r+tmp.c]->parent;
                visited[tmp.r][tmp.c] = true;
                batt--;
                s++;
            }
            d--; 
        }
        step += s;
        cout << "step: " << step << endl;
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
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
        // adjnode a(1,9);
        // go_to(a);
        // a.r = 1;
        // a.c = 9;
        // go_to(a);
        // a.r = 1;
        // a.c = 9;
        // go_to(a);
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
    queue<adjnode> q_DeadEnd;
};


int main(){
    FCR fcr;
    fcr.read();
    fcr.BFS_FindDisranceToR();
    fcr.test();
    fcr.traverse();
}