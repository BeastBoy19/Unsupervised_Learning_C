#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "include\EDA.h"
char **header;
int features = 0;
int records = 0;
int count = 0;
char ***trainX, **trainY, ***testX, **testY;
int traincount = 0, testcount = 0;
FILE *fp;
char ***get_dataset(FILE *fp){
    char ch = '\0';
    int i = 0, j = 0, flag = 0;
    char ***words = (char ***)malloc(SMAX * sizeof(char **));
    words[i] = (char **)malloc(RMAX * sizeof(char *));
    words[i][j] = (char *)malloc(WMAX * sizeof(char));
    memset(words[i][j], 0, sizeof(char) * WMAX);
    while (!feof(fp)){
        ch = fgetc(fp);
        if (!strncmp(&ch, ",", 1)){
            j++;
            words[i][j] = (char *)malloc(WMAX * sizeof(char));
            memset(words[i][j], 0, sizeof(char) * WMAX);
            continue;
        }
        if ((int)ch == 10){
            i++;
            if (flag == 0){
                features = ++j;
                flag = 1;
            }
            j = 0;
            words[i] = (char **)malloc(RMAX * sizeof(char *));
            words[i][j] = (char *)malloc(WMAX * sizeof(char));
            memset(words[i][j], 0, sizeof(char) * WMAX);
            continue;
        }
        strncat(words[i][j], &ch, 1);
    }
    records = i;
    return words;
}
void get_file(const char *file_name, const char *mode){
    fp = fopen(file_name, mode);
}
void display(char ***dataset, int n,int m){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++)
            printf("%s\t", dataset[i][j]);
        printf("\n");
    }
}
char **get_funiques(char ***dataset, int col){
    count = 0;
    char **unique = (char **)malloc(RMAX * sizeof(char *));
    for (int i = 2; i < records; i++){
        int flag = 0;
        for (int j = 0; j < count; j++){
            if (!strcmp(dataset[i][col], unique[j])){
                flag = 1;
                break;
            }
        }
        if (flag == 0){
            unique[count] = (char *)malloc(WMAX * sizeof(char));
            memset(unique[count], 0, sizeof(char) * WMAX);
            strcpy(unique[count], dataset[i][col]);
            count++;
        }
    }
    return unique;
}
void headerseperator(char ***dataset){
    header = (char **)malloc(RMAX * sizeof(char *));
    for (int i = 0; i < features; i++){
        header[i] = (char *)malloc(WMAX * sizeof(char));
        strcpy(header[i], dataset[0][i]);
    }
    for (int i = 0; i < records - 1; i++)
        for (int j = 0; j < features; j++)
            strcpy(dataset[i][j], dataset[i + 1][j]);
    records--;
}
void labelencoding(char ***dataset, int col){
    char **u = get_funiques(dataset, col);
    int ucount = count;
    for (int i = 0; i < records; i++){
        for (int j = 0; j < ucount; j++){
            if (!strcmp(dataset[i][col], u[j])){
                char num = '0' + j;
                memset(dataset[i][col], 0, sizeof(char) * WMAX);
                strncpy(dataset[i][col], &num, 1);
                break;
            }
        }
    }
}
char ***get_subset(char ***dataset, char *str,int col){
    count = 0;
    char ***set = (char ***)malloc(SMAX * sizeof(char **));
    for (int i = 0; i < records; i++){
        if (!strcmp(str, dataset[i][col])){
            set[count] = (char **)malloc(RMAX * sizeof(char *));
            for (int j = 0; j < features; j++){
                set[count][j] = (char *)malloc(WMAX * sizeof(char));
                strcpy(set[count][j], dataset[i][j]);
            }
            count++;
        }
    }
    return set;
}
void adisplay(char **dataset,int row){
    for (int i = 0; i < row; i++)
        printf("%s\n", dataset[i]);
}
void xdisplay(float *vector, int row){
    for (int i = 0; i < row; i++)
        printf("%f\t", vector[i]);
}
void get_feature(char ***dataset, int row, int col, const char *file){
    char *buffer = (char *)malloc(20 * sizeof(char));
    sprintf(buffer, "%s.dat", file);
    FILE *data = fopen(buffer, "w");
    for (int i = 0; i < row; i++)
        fprintf(data, "%s\n", dataset[i][col]);
    fclose(data);
}
void plot(const char *xstr, const char *ystr, char ***dataset, int col, int row, const char *file){
    FILE *gnuplot;
    gnuplot = popen("gnuplot -persist", "w");
    if (gnuplot == NULL)
        perror("Cannot open file!\n");
    get_feature(dataset, row, col, file);
    fprintf(gnuplot, "set ylabel \"%s\"\n"
                     "set xlabel \"%s\"\n"
                     "set title \"Data analysis %s\"\n"
                     "plot '%s.dat' smooth freq with boxes lc 7 \n",
            ystr, xstr, file, file);
    fclose(gnuplot);
}
void recordswap(char **record1,char **record2){
    for(int i=0;i<features;i++){
        char *temp=record1[i];
        record1[i]=record2[i];
        record2[i]=temp;
    }
}
char **copy(char** data,int row){
    char** temp=(char**)malloc(sizeof(char*)*row);
    for(int i=0;i<row;i++){
        temp[i]=(char*)malloc(sizeof(char)*WMAX);
        strcpy(temp[i],data[i]);
    }
    return temp;
}
void shuffle(char*** dataset,int n){
    for(int j=0;j<n;j++){
        for(int i=0;i<records;i++)
            recordswap(dataset[i],dataset[randi(records)]);
    }
}
void train_test_split(char ***dataset,float train_size){
    traincount=records*train_size;
    testcount=records-traincount;
    int traincounter=0,testcounter=0;
    trainX=(char***)malloc(sizeof(char**)*traincount);
    trainY=(char**)malloc(sizeof(char*)*traincount);
    testX=(char***)malloc(sizeof(char**)*testcount);
    testY=(char**)malloc(sizeof(char*)*testcount);
    for(int i=0;i<records;i++){
        if(i<traincount)
            trainX[traincounter]=(char**)malloc(sizeof(char*)*features-1);
        else
            testX[testcounter]=(char**)malloc(sizeof(char*)*features-1);
        for(int j=0;j<features-1;j++){
            if(i<traincount){
                trainX[traincounter][j]=(char*)malloc(sizeof(char)*WMAX);
                strcpy(trainX[traincounter][j],dataset[i][j]);
            }
            else{
                testX[testcounter][j]=(char*)malloc(sizeof(char)*WMAX);
                strcpy(testX[testcounter][j],dataset[i][j]);
            } 
            }
            if(i<traincount){
                trainY[traincounter]=(char*)malloc(sizeof(char)*WMAX);
                strcpy(trainY[traincounter],dataset[i][features-1]);
                traincounter++;
            }
            else{
                testY[testcounter]=(char*)malloc(sizeof(char)*WMAX);
                strcpy(testY[testcounter],dataset[i][features-1]);
                testcounter++;
            }
    }
}
int index(char **unique,int col,char *data){
    for(int i=0;i<col;i++)
        if(!strcmp(unique[i],data))
            return i;
    return -1;
}
int **evaluate(char ***dataset,char **prediction){
    char **u = get_funiques(dataset, features - 1);
    int ucount = count;
    int **confusion=(int**)malloc(sizeof(int*)*ucount);
    for(int i=0;i<ucount;i++){
        confusion[i] = (int *)malloc(sizeof(int) * ucount);
        memset(confusion[i],0,sizeof(int)*ucount);
    }
    for(int j=0;j<testcount;j++)
        confusion[index(u,ucount, prediction[j])][index(u, ucount, testY[j])]++;
    printf("CONFUSION MATRIX\n");
    printf("\t\t");
    for(int j=0;j<ucount;j++)
        printf("%s\t", u[j]);
    printf("\n");
    for(int i=0;i<ucount;i++){
        printf("%s\t", u[i]);
        for(int j=0;j<ucount;j++)
            printf("%d\t\t",confusion[i][j]);
        printf("\n");
    }
    return confusion;
}
int uniquecount(char ***dataset,int row,int fetch,char *str){
    count=0;
    for(int i=0;i<row;i++){
        if(!strcmp(dataset[i][fetch],str))
            count++;
    }
    return count;
}
