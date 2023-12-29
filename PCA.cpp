#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
const int columns=2;
const int records=6;
float *average(float x[][columns]){
    float *u=(float*)malloc(sizeof(float)*columns);
    memset(u,0,sizeof(float)*columns);
    for(int j=0;j<columns;j++){
        for(int i=0;i<records;i++){
            u[j]+=x[i][j];
        }
        u[j]=u[j]/records;
    }
    return u;
}
float **meandeviation(float x[][columns],float *mean){
    float **sig=(float**)malloc(sizeof(float*)*records);
    for(int i=0;i<records;i++){
        sig[i]=(float*)malloc(sizeof(float)*columns);
        for(int j=0;j<columns;j++){
            sig[i][j]=x[i][j]-mean[j];
        }
    }
    return sig;
}
void display(float **mat,int r,int c){
    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++)
            printf("%f\t",mat[i][j]);
        printf("\n");
    }
}
void vector(float *x,int n){
    for(int i=0;i<n;i++)
        printf("%f\t",x[i]);
}
float **transmul(float *x,int n){
    float **mul=(float**)malloc(sizeof(float*)*n);
    for(int i=0;i<n;i++){
        mul[i]=(float*)malloc(sizeof(float)*n);
        for(int j=0;j<n;j++){
            mul[i][j]=x[i]*x[j];
        }
    }
    return mul;
}
void sum(float **x,float **y,int n){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            x[i][j]+=y[i][j]/records;
        }
    }
}
float **covarience(float **sig){
    float **cov=(float**)malloc(sizeof(float*)*columns);
    for(int j=0;j<columns;j++){
        cov[j]=(float*)malloc(sizeof(float)*columns);
        memset(cov[j],0,sizeof(float)*columns);
    }
    for(int i=0;i<records;i++){
        float **mul=transmul(sig[i],columns);
        sum(cov,mul,columns);
    }
    return cov;
}
float *eigen(float **cov){
    float *eg=(float*)malloc(sizeof(float)*columns);
    float a=1;
    float b=(-cov[0][0]-cov[1][1]);
    float c=(cov[0][0]*cov[1][1]-cov[0][1]*cov[1][0]);
    for(int i=0;i<columns;i++){
        eg[0]=(-b+pow(pow(b,2)-(4*a*c),0.5))/(2*a);
        eg[1]=(-b-pow(pow(b,2)-(4*a*c),0.5))/(2*a);
    }
    return eg;
}
float *eigenmat(float eigen,float **cov){
    float *emat=(float*)malloc(sizeof(float)*columns);
    emat[0] = (float)cov[0][1]/(eigen - cov[0][0]) ;
    emat[1] = 1;
    return emat;
}
int main(){
    float x[records][columns]={{2,1},{3,5},{4,3},{5,6},{6,7},{7,8}};
    printf("DATASET:\n");
    for(int i=0;i<records;i++){
        for(int j=0;j<columns;j++)
            printf("%f\t",x[i][j]);
        printf("\n");
    }
    float *mean=average(x);
    printf("\nMEAN:\n");
    for(int i=0;i<columns;i++)
        printf("%f\t",mean[i]);
    float **deviation=meandeviation(x,mean);
    printf("\n\nMEAN DEVIATION:\n");
    display(deviation,records,columns);
    printf("\nCOVARIENCE MATRIX:\n");
    float **cov=covarience(deviation);
    display(cov,columns,columns);
    printf("\nEIGEN VALUES:\n");
    float *eg=eigen(cov);
    vector(eg,columns);
    printf("\nTaking MAX eigen value and rejecting MIN eigen value\n");
    printf("\nEIGEN VECTOR:\n");
    float *egmat=eigenmat(eg[0],cov);
    for(int j=0;j<columns;j++)
        printf("%f\n",egmat[j]);
    return 0;
}