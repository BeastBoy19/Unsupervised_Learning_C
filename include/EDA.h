#ifndef EDA_h
#define EDA_h
#include<stdio.h>
#define SMAX 1000
#define RMAX 200
#define WMAX 50
#define MCLASS 10
#define shape printf("%d, %d\n", records, features);
#define randi(x) rand() % x + 1
char ***get_dataset(FILE *fp);
void get_file(const char *file_name, const char *);
void display(char ***dataset, int n,int m);
void adisplay(char **dataset, int row);
void xdisplay(float *vector, int row);
char **get_funiques(char ***dataset, int col);
void headerseperator(char ***dataset);
void labelencoding(char ***dataset, int col);
char ***get_subset(char ***dataset, char *str,int col);
void get_feature(char ***dataset, int row, int col, const char *file);
void plot(const char *xstr, const char *ystr, char ***dataset, int col, int row, const char *file);
void shuffle(char*** dataset,int n);
void train_test_split(char ***dataset,float train_size);
char **copy(char** data,int row);
int **evaluate(char ***dataset,char **prediction);
int index(char **unique,int col,char *data);
int uniquecount(char ***dataset,int row,int fetch,char *str);
extern char **header;
extern int features;
extern int records;
extern int count;
extern FILE *fp;
extern char ***trainX, **trainY, ***testX, **testY;
extern int traincount, testcount;
#endif