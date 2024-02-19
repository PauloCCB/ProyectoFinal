#include <iostream>
#include <windows.h>
#include <math.h>

using namespace std;

int m,n;
int i,j,k;
char CONTINUE;
bool okInput[10][10];
float constant [10][10];
float temp;

// Funciones
void header(){
    cout<<"Gauss - Jordan Method"<<endl;
}
void dataInput(){
    step1:
    system("cls");
    header();
    cout<<"Enter the number of equations to solved"<<m<<endl;
    cout<<endl;
    cout<<endl;
    for(i=0;i<m;i++){
        for(j=0;j<n;j++){
            cout<<"\t";
            if(j==n-1){
                cout<<"| ";
            }
            if(okInput[i][j]){
                if(j==n-1){
                    cout<<constant[i][j]<<endl <<endl;
                }else{
                    cout<<constant[i][j];
                }
            }else 
            {
                cin>>constant[i][j];
                okInput[i][j]=true;
                goto step1;
            }
            
        }
    }

}
int setprecision(int n){
    return n;
}
void solution(){
    stepOne:
    for(k=0;k<m;k++){
        if(constant[k][k]!=1){
            if(constant[k][k]==0){
                
            }
        }
    }
}

void clearAll(){

}
int main(){
    start:
    header();
    cout<<"Enter the number of equations to be solved: ";
    cin>>m;
    n=m+1;
    //cout<<setprecision(3);
    //dataInput();
    cout<<endl;
    //solution();
    cout<<endl;
    cout<<"Type 'Y' to restart , otherwise exit."<<endl;
    cin>>CONTINUE;
    if (CONTINUE=='Y'|| CONTINUE=='y'){
       // clearAll();
        goto start;
    }
}


