#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "include\EDA.h"
int flag=0;
float distance(float *row1,char **row2,int col){
    float sum=0;
    for(int i=0;i<col;i++)
        sum += pow(atof(row2[i])-row1[i], 2);
    return sum;
}
float **centroid(char ***df,int k){
    float **centroids = (float **)malloc(sizeof(float *) * k);
    for(int i=0;i<k;i++){
        centroids[i]=(float*)malloc(sizeof(float)*features);
        for(int j=0;j<features;j++)
            centroids[i][j]=atof(df[i][j]);
    }
    return centroids;
}
void update(float **cent,char ***df,int *c,int k,int *x){
    float *temp=(float*)malloc(sizeof(float)*features);
    for(int i=0;i<k;i++){
        memset(temp,0,sizeof(float)*features);
        for(int j=0;j<records;j++){
            for(int z=0;z<features;z++){
                if(c[j]==i){
                    temp[z] = temp[z] + atof(df[j][z]);
                }
            } 
        }
        for(int j=0;j<features;j++)
            cent[i][j]=temp[j]/x[i];
    }
}
int *clustercount(int *c,int k){
    int *x=(int*)malloc(sizeof(int)*k);
    memset(x,0,sizeof(int)*k);
    for(int j=0;j<k;j++){
        for(int i=0;i<records;i++){
            if(c[i]==j)
                x[j]++;
        }
        printf("Cluster (%d) : %d\t",j,x[j]);
    }
    printf("\n");
    return x;
}
void mat(float **mat,int r,int c){
    printf("centroid\n");
    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){
            printf("%f\t",mat[i][j]);
        }
        printf("\n");
    }
}
void cluster(float **d,int *c,int k){
    for(int i=0;i<records;i++){
        float min=FLT_MAX;
        int index;
        for(int j=0;j<k;j++){
            if(min>d[i][j]){
                    min = d[i][j];
                    index=j;
            }
        }
        if(c[i]!=index){
            c[i] = index;
            flag=1;
        }
    }
}
void kmeans(char ***df,int k){
    printf("For k = %d\n",k);
    float **cent=centroid(df,k);
    int *clus=(int*)malloc(sizeof(int)*records);
    memset(clus,0,sizeof(int)*records);
    float **dist=(float**)malloc(sizeof(float*)*records);
    while(1){
        for(int i=0;i<records;i++){
            dist[i]=(float*)malloc(sizeof(float)*k);
            for(int j=0;j<k;j++){
                dist[i][j]=distance(cent[j],df[i],features);
            }
        }
        flag=0;
        cluster(dist,clus,k);
        int *x=clustercount(clus, k);
        if(flag==0)
            break;
        update(cent,df,clus,k,x);
    }
}
int main(){
    get_file("C:/Users/bikas/Projects/Dataset/IRIS.csv","r");
    char ***df = get_dataset(fp);
    printf("THE DATASET:\n");
    display(df,5,features);
    headerseperator(df);
    features--;
    printf("\nSHAPE OF DATASET:\n");
    shape;
    shuffle(df,2);
    printf("\nAFTER SHUFFLING:\n");
    display(df,5,features);
    printf("\nCLUSTER COUNT:\n");
    kmeans(df,4);
    return 0;
}