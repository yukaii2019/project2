#include <iostream>
#include <fstream>
#include <iomanip>
#include <time.h>
using namespace std;

template<class T> class list;
class FCR;

template<class T>
class link{

friend class FCR;
friend class list<T>;
public:
    link(T& x):value(x),next(nullptr){}
private:
    T value;
    link<T>* next;
};

class adjnode{
    friend class FCR;
public:
    adjnode():r(0),c(0){}
    adjnode(int rr,int cc):r(rr),c(cc){}                     
private:
    int r,c; 
};

template<class T>
class list{
friend class FCR;
public:
    list():first(nullptr),last(nullptr),my_size(0){}
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
    ElementNode(int rr,int cc,char dd):r(rr),c(cc),data(dd),side(0),distance_to_R(0),dead_end(0),save_point(0),save_point_2(0),save_point_3(0){
        for(int i=0;i<10;i++) savept[i] = 0;
    }

private:   
    list<adjnode> adjlist;
    int distance_to_R;
    int r,c;
    char data;
    int side;
    int dead_end;
    int dead_len[3] = {0};
    adjnode endstart[3];
    adjnode parent;
    int save_point;
    int save_point_2;
    int save_point_3;
    int savept[10];
};
class FCR{
public:
    FCR():m(0),n(0),B(0),num_of_0(0){}
    void read()
    {   
        ifstream rdfile;
        rdfile.open("floor2.data",ios::in);
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
    int SavePointUtil_N(int N,int rr,int cc)
    {
        int re1=0,re2=0,re3=0,re4=0;
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                if(matrix[n*(rr+i)+cc+j]->distance_to_R==0)break;
                if(matrix[n*rr+cc]->distance_to_R-matrix[n*(rr+i)+cc+j]->distance_to_R==i+j)                       re1++;
                if(matrix[n*rr+(cc+N-1)]->distance_to_R-matrix[n*(rr+i)+(cc+N-1-j)]->distance_to_R==i+j)           re2++;
                if(matrix[n*(rr+N-1)+cc]->distance_to_R-matrix[n*(rr+N-1-i)+cc+j]->distance_to_R==i+j)             re3++;
                if(matrix[n*(rr+N-1)+(cc+N-1)]->distance_to_R-matrix[n*(rr+N-1-i)+(cc+N-1-j)]->distance_to_R==i+j) re4++;
            }
        }
        if(re1==N*N)      return 1;
        else if(re2==N*N) return 2;
        else if(re3==N*N) return 3;
        else if(re4==N*N) return 4;
        else return 0;
    }
    void SetSavePoint()
    {
        for(int i=1;i<m-2;i++){
            for(int j=1;j<n-2;j++){
                for(int k=2;k<=11;k++){
                    if(i<m-k&&j<n-k){
                        int s = SavePointUtil_N(k,i,j);
                        if(s==1)
                            matrix[n*(i+k-1)+j+k-1]->savept[k-2] = 4;
                        else if(s==2)
                            matrix[n*(i+k-1)+j]->savept[k-2] = 3;
                        else if(s==3)
                            matrix[n*i+j+k-1]->savept[k-2] = 2;
                        else if(s==4)
                            matrix[n*i+j]->savept[k-2] = 1;
                    }
                }
            }
        }
        
    }
    void test()
    {
        fstream d_file;
        d_file.open("distance.data",ios::out);
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                d_file << setw(4) << matrix[i*n+j]->distance_to_R << " ";
            }
            d_file << endl;
        }
        d_file << endl;   
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                d_file << setw(4) << matrix[i*n+j]->savept[3] << " ";
            }
            d_file << endl;
        }
        d_file << endl;
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                d_file << setw(4) << matrix[i*n+j]->savept[4] << " ";
            }
            d_file << endl;
        }
        d_file << endl;
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                d_file << setw(4) << matrix[n*i+j]->savept[5] << " ";
            }
            d_file << endl;
        }
        d_file << endl;
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                d_file << setw(4) << matrix[i*n+j]->dead_len[0] << " ";
            }
            d_file << endl;
        }
        d_file.close();
    }
    void BFS_FindDisranceToR(){
        visited = new bool* [m];
        int disc = 0;   
        int maxdis = -1;  
        link<adjnode>* curr;

        for(int i=0;i<m;i++)
            visited[i] = new bool [n];
        
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                visited[i][j] = false;
            }
        }
        
        list<adjnode> q;
        visited[R.r][R.c] = true;
        matrix[n*R.r+R.c]->distance_to_R = disc;
        q.push(R);
        q_DistanceSorted.push(R);
        while(!q.empty()){
            adjnode node = q.front();  
            q.pop(); 
            for(curr = matrix[n*node.r+node.c]->adjlist.first;curr!=nullptr;curr = curr->next){
                if(visited[curr->value.r][curr->value.c]==false){
                    visited[curr->value.r][curr->value.c] = true;             
                    matrix[n*curr->value.r+curr->value.c]->distance_to_R = matrix[n*node.r+node.c]->distance_to_R+1;
                    if(matrix[n*node.r+node.c]->distance_to_R+1>maxdis)maxdis = matrix[n*node.r+node.c]->distance_to_R+1;//可以刪掉
                    matrix[n*curr->value.r+curr->value.c]->parent = node;
                    q.push(curr->value);
                    q_DistanceSorted.push(curr->value);
                }
            }
        }
        cout << maxdis << endl;
        for(int i=0;i<m;i++)
            delete[] visited[i];
        delete []visited;
    }
    void DFS(adjnode dest)
    {
        int batt = B;
        list<adjnode> q;
        list<adjnode> s_haspass;
        list<adjnode> s_forDFS;      
        adjnode tmp(dest.r,dest.c);
        while(!(tmp.r==R.r&&tmp.c==R.c)){
            q.push(tmp);
            tmp = matrix[n*tmp.r+tmp.c]->parent;
        }
        batt-=q.my_size;
        for(;!q.empty();q.pop_back()){ 
            s_haspass.push(tmp);
            tmp = q.back();
            visited[tmp.r][tmp.c] = true;
            ans_list.push(tmp);
        }
        int d = matrix[n*tmp.r+tmp.c]->distance_to_R;
        while(batt>d){
            int issave = -1;
            int save[10];
            for(int i=0;i<10;i++)save[i]=-1;

            for(int i=11;i>=2;i--){
                if(batt-matrix[n*tmp.r+tmp.c]->distance_to_R>=i*i+i && matrix[n*tmp.r+tmp.c]->savept[i-2]!=0){
                    save[i-2] = matrix[n*tmp.r+tmp.c]->savept[i-2];
                    issave = i-2;
                    break;
                }
            }
            for(link<adjnode>* a=matrix[n*tmp.r+tmp.c]->adjlist.first;a!=nullptr;a=a->next){
                if(matrix[n*a->value.r+a->value.c]->distance_to_R==d+1){
                    if(visited[a->value.r][a->value.c]==false)
                        q.push(a->value);
                }
            }
            if(q.size()==1&&batt-1>=matrix[n*q.back().r+q.back().c]->distance_to_R){
                s_haspass.push(tmp);
                tmp = q.back();
                visited[tmp.r][tmp.c] = true;
                ans_list.push(tmp);
                batt--;
                //d++;
                q.clean();
            }
            else if(q.size()>1&&issave!=-1){
                int i = issave;
                int minus_r=1,minus_c=1;
                adjnode a;
                if(save[i]==2)minus_c=-1;
                else if(save[i]==3)minus_r=-1;
                else if(save[i]==4){minus_r=-1;minus_c=-1;}
                    
                //     a.r = tmp.r;   a.c = tmp.c-minus_c*1;  
                //     if(matrix[n*a.r+a.c]->data!='$') s_forDFS.push(a);
                    
                // for(int k=1;k<=i+1;k++){
                //     a.r = tmp.r-minus_r*1;   a.c = tmp.c+minus_c*k;  
                //     if(matrix[n*a.r+a.c]->data!='$') s_forDFS.push(a);
                // }
                // for(int k=0;k<=i+1;k++){
                //     a.r = tmp.r+minus_r*k;   a.c = tmp.c+minus_c*(i+2); 
                //     if(matrix[n*a.r+a.c]->data!='$') s_forDFS.push(a);
                // }
                // for(int k=i+1;k>=1;k--){
                //     a.r = tmp.r+minus_r*(i+2);   a.c = tmp.c+minus_c*k;  
                //     if(matrix[n*a.r+a.c]->data!='$') s_forDFS.push(a);
                // }
                // for(int k=1;k<=i+1;k++){
                //     a.r = tmp.r+minus_r*k;   a.c = tmp.c-minus_c*1;  
                //     if(matrix[n*a.r+a.c]->data!='$') s_forDFS.push(a);
                // }
                s_haspass.push(tmp);
                if(i%2==0){
                    for(int k=1;k<=i+1;k++){
                        a.r = tmp.r;   a.c = tmp.c+minus_c*k; visited[a.r][a.c] = true; ans_list.push(a); s_haspass.push(a);
                    }
                    for(int l=1;l<=i+1;l++){
                        if(l%2==1){
                            for(int k=i+1;k>=i;k--){
                                a.r = tmp.r+minus_r*l;   a.c = tmp.c+minus_c*k; visited[a.r][a.c] = true; ans_list.push(a); 
                                if(l==i+1) s_haspass.push(a);
                                else if(k==i+1) s_haspass.push(a);   
                            }
                        }
                        else{
                            for(int k=i;k<=i+1;k++){
                                a.r = tmp.r+minus_r*l;   a.c = tmp.c+minus_c*k; visited[a.r][a.c] = true; ans_list.push(a); 
                                if(k==i+1)s_haspass.push(a);
                            }
                        }
                    }
                    for(int l=i-1;l>=0;l--){
                        if(l%2==1){
                            for(int k=i+1;k>=1;k--){
                                a.r = tmp.r+minus_r*k;   a.c = tmp.c+minus_c*l; visited[a.r][a.c] = true; ans_list.push(a); 
                                if(l==1) s_haspass.push(a);
                                else if(k==i+1)s_haspass.push(a);
                            }
                        }
                        else{
                            for(int k=1;k<=i+1;k++){
                                a.r = tmp.r+minus_r*k;   a.c = tmp.c+minus_c*l; visited[a.r][a.c] = true; ans_list.push(a);
                                if(l==0&&k!=i+1) s_haspass.push(a);
                                else if(k==i+1)s_haspass.push(a);
                            }
                        }
                    }

                }
                else if(i%2==1){
                    for(int k=1;k<=i+1;k++){
                        a.r = tmp.r;   a.c = tmp.c+minus_c*k; visited[a.r][a.c] = true; ans_list.push(a); s_haspass.push(a);
                    }
                    for(int l=i+1;l>=0;l--){
                        if(l%2==0){
                            for(int k=1;k<=i+1;k++){
                                a.r = tmp.r+minus_r*k;   a.c = tmp.c+minus_c*l; visited[a.r][a.c] = true; ans_list.push(a); 
                                if(l==0&&k!=i+1) s_haspass.push(a);
                                else if(k==i+1)s_haspass.push(a);
                            }
                        }
                        else{
                            for(int k=i+1;k>=1;k--){
                                a.r = tmp.r+minus_r*k;   a.c = tmp.c+minus_c*l; visited[a.r][a.c] = true; ans_list.push(a);
                                if(l==1) s_haspass.push(a);
                                else if(k==i+1)s_haspass.push(a);
                            }
                        }
                    }
                }
                tmp = a;
                //d += i+1;
                batt-=(i+2)*(i+2)-1;
                q.clean();
                }
            else if(q.size()>1&&batt-1>=matrix[n*q.back().r+q.back().c]->distance_to_R){
                s_haspass.push(tmp);
                tmp = q.back();
                visited[tmp.r][tmp.c] = true;
                ans_list.push(tmp);
                batt--;
                //d++;
                q.clean();
            }
            else if(batt-1>=matrix[n*s_haspass.back().r+s_haspass.back().c]->distance_to_R){
                tmp = s_haspass.back();
                s_haspass.pop_back();
                //visited[tmp.r][tmp.c] = true;
                ans_list.push(tmp);
                batt--;
                if(s_haspass.empty()) break;
                //d--; //怪怪的
                // bool nomorebig = true;
                // for(link<adjnode>* a=matrix[n*tmp.r+tmp.c]->adjlist.first;a!=nullptr;a=a->next){
                //     if(matrix[n*a->value.r+a->value.c]->distance_to_R==d+1){
                //         tmp = a->value;
                //         visited[tmp.r][tmp.c] = true;
                //         s_haspass.push(tmp);
                //         ans_list.push(tmp);
                //         batt--;
                //         d++;
                //         q.clean();
                //         nomorebig = false;
                //         break;
                //     }
                // } 
                // if(nomorebig == true){break;} 
            }
            else 
                break;
            d = matrix[n*tmp.r+tmp.c]->distance_to_R;
        }
        while(!(tmp.r==R.r&&tmp.c==R.c)){
            tmp = matrix[n*tmp.r+tmp.c]->parent;
            visited[tmp.r][tmp.c] = true;
            ans_list.push(tmp);
        }
        s_haspass.clean();
        cout << batt << endl;
        static int g=1;
        fstream d_file;
        d_file.open("trace.data",ios::out|ios::app);
        d_file << "step: " << g << endl;
        g++;
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                d_file << visited[i][j];
            }
            d_file << endl;
        }
        d_file << endl;
        d_file.close();
    }
    void go_to(adjnode dest)
    {   
        int batt=B;
        list<adjnode> q;
        adjnode tmp(dest.r,dest.c);
        while(!(tmp.r==R.r&&tmp.c==R.c)){
            q.push(tmp);
            tmp = matrix[n*tmp.r+tmp.c]->parent;
        }
        batt-=q.my_size;
        for(;!q.empty();q.pop_back()){ 
            tmp = q.back();
            visited[tmp.r][tmp.c] = true;
            ans_list.push(tmp);
        }
        int d = matrix[n*tmp.r+tmp.c]->distance_to_R-1;

        while(!(tmp.r==R.r&&tmp.c==R.c)){
            for(link<adjnode>* a = matrix[n*tmp.r+tmp.c]->adjlist.first;a!=nullptr;a=a->next){
                if(matrix[n*a->value.r+a->value.c]->distance_to_R==d){
                    if(visited[a->value.r][a->value.c]==false){
                        q.push(a->value);
                    }
                }
            }
            // adjnode a1;
            // adjnode a2;         //選哪個好
            // adjnode a3;
            int save[10];
            for(int i=0;i<10;i++)save[i]=-1;
            // int save = -1;
            // int save2 = -1;
            // int save3 = -1;
            int enterdead = -1;

            int battneed;
            if(matrix[n*tmp.r+tmp.c]->dead_end !=0 ){
                for(int i=0;i<matrix[n*tmp.r+tmp.c]->dead_end;i++){                  
                    if(visited[matrix[n*tmp.r+tmp.c]->endstart[i].c][matrix[n*tmp.r+tmp.c]->endstart[i].r]==false)  //start_r start_c
                        if(batt-battneed>=matrix[n*tmp.r+tmp.c]->distance_to_R){
                            enterdead = i; 
                            battneed = 2*matrix[n*tmp.r+tmp.c]->dead_len[i];
                            break;
                        }   
                }
            }

            bool issave=false;
            for(int i=11;i>=2;i--){
                if(batt-matrix[n*tmp.r+tmp.c]->distance_to_R>=i*i-i && matrix[n*tmp.r+tmp.c]->savept[i-2]!=0){
                    save[i-2] = matrix[n*tmp.r+tmp.c]->savept[i-2];
                    issave=true;
                    break;
                }
            }
            if(enterdead != -1){
                list<adjnode> q_forback;
                q_forback.push(tmp);
                tmp = matrix[n*tmp.r+tmp.c]->endstart[enterdead];
                while(matrix[n*tmp.r+tmp.c]->side!=1){
                    visited[tmp.r][tmp.c] = true;
                    ans_list.push(tmp);
                    q_forback.push(tmp);
                    for(link<adjnode>* a = matrix[n*tmp.r+tmp.c]->adjlist.first;a!=nullptr;a=a->next){
                        if(!(a->value.r==tmp.r&&a->value.c==tmp.c))
                            tmp = a->value;
                    }
                }
                visited[tmp.r][tmp.c] = true;
                ans_list.push(tmp);
                for(;!q_forback.empty();q_forback.pop_back()){
                    tmp = q_forback.back();
                    visited[tmp.r][tmp.c] = true;
                    ans_list.push(tmp);
                }
                batt -= battneed;
                d++;
            }
            else if(q.size() == 1){
                tmp = q.back();   //front還是back好
                visited[tmp.r][tmp.c] = true;
                ans_list.push(tmp);
                batt--;
                q.clean();     
            }
            else if(q.size()>1&&issave){
                for(int i=9;i>=0;i--){
                    if(save[i]!=-1){
                        adjnode a;
                        int minus_r=1,minus_c=1;
                        if(save[i]==2)minus_c=-1;
                        else if(save[i]==3)minus_r=-1;
                        else if(save[i]==4){minus_r=-1;minus_c=-1;}
                        if(i%2==0){
                            for(int k=1;k<=i+1;k++){
                                a.r = tmp.r;   a.c = tmp.c+minus_c*k; visited[a.r][a.c] = true; ans_list.push(a);
                            }
                            for(int l=1;l<=i+1;l++){
                                if(l%2==1){
                                    for(int k=i+1;k>=i;k--){
                                        a.r = tmp.r+minus_r*l;   a.c = tmp.c+minus_c*k; visited[a.r][a.c] = true; ans_list.push(a);
                                    }
                                }
                                else{
                                    for(int k=i;k<=i+1;k++){
                                        a.r = tmp.r+minus_r*l;   a.c = tmp.c+minus_c*k; visited[a.r][a.c] = true; ans_list.push(a);
                                    }
                                }
                            }
                            for(int l=i-1;l>=0;l--){
                                if(l%2==1){
                                    for(int k=i+1;k>=1;k--){
                                        a.r = tmp.r+minus_r*k;   a.c = tmp.c+minus_c*l; visited[a.r][a.c] = true; ans_list.push(a);
                                    }
                                }
                                else{
                                    for(int k=1;k<=i+1;k++){
                                        a.r = tmp.r+minus_r*k;   a.c = tmp.c+minus_c*l; visited[a.r][a.c] = true; ans_list.push(a);
                                    }
                                }
                            }

                        }
                        else if(i%2==1){
                            for(int k=1;k<=i+1;k++){
                                a.r = tmp.r;   a.c = tmp.c+minus_c*k; visited[a.r][a.c] = true; ans_list.push(a);
                            }
                            for(int l=i+1;l>=0;l--){
                                if(l%2==0){
                                    for(int k=1;k<=i+1;k++){
                                        a.r = tmp.r+minus_r*k;   a.c = tmp.c+minus_c*l; visited[a.r][a.c] = true; ans_list.push(a);
                                    }
                                }
                                else{
                                    for(int k=i+1;k>=1;k--){
                                        a.r = tmp.r+minus_r*k;   a.c = tmp.c+minus_c*l; visited[a.r][a.c] = true; ans_list.push(a);
                                    }
                                }
                            }
                        }
                        tmp = a;
                        d-=i;
                        batt-=(i+2)*(i+2)-1;
                        q.clean();
                        break;
                    }
                }
            }
            else{
                tmp = matrix[n*tmp.r+tmp.c]->parent;
                visited[tmp.r][tmp.c] = true;
                ans_list.push(tmp);
                batt--;
            }
            d--; 
        }
        cout << batt << endl;
        static int g=1;
        fstream d_file;
        d_file.open("trace.data",ios::out|ios::app);
        d_file << "step: " << g << endl;
        g++;
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                d_file << visited[i][j];
            }
            d_file << endl;
        }
        d_file << endl;
        d_file.close();
    }
    void traverse(){
        SetDeadEnd();
        SetSavePoint();
        fstream d_file;
        d_file.open("trace.data",ios::out);
        d_file.close();
        cout << (double)clock() / CLOCKS_PER_SEC << "S" << endl;
        visited = new bool* [m];    
        for(int i=0;i<m;i++)
            visited[i] = new bool[n];
        for(int i=0;i<m;i++)
            for(int j=0;j<n;j++)
                visited[i][j] = false;
                cout << endl;
        while(!q_DistanceSorted.empty()){
            adjnode node = q_DistanceSorted.front(); 
            if(visited[node.r][node.c]==false)
                DFS(node);
            q_DistanceSorted.pop();
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
    void write(){
        fstream opfile;
        opfile.open("final.path",ios::out);
        if(!opfile){
            cout << "error";
        }
        opfile << ans_list.size() <<endl;
        opfile << R.r << " " << R.c;
        for(;!ans_list.empty();ans_list.pop()){
            opfile << endl << ans_list.front().r << " " << ans_list.front().c;
        } 
        opfile.close(); 
    }
private:
    adjnode R;
    int m,n,B;
    ElementNode** matrix;
    bool** visited;
    int num_of_0;
    list<adjnode> q_DistanceSorted;
    list<adjnode> q_DeadEnd;
    list<adjnode> ans_list;
};


int main(){
    FCR fcr;
    fcr.read();
    fcr.BFS_FindDisranceToR();
    fcr.traverse();
    //fcr.SetSavePoint();
    fcr.test();
    cout << (double)clock() / CLOCKS_PER_SEC << "S" << endl;
    fcr.write();
    cout << (double)clock() / CLOCKS_PER_SEC << "S" << endl;
}