#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int *x; // city's x
int *y; // city's y
int *s; // selected cities index
double *r; // distance between a city and next selected city
double *r2; // distance between a city and selected city after next
double sum; // sum of distance between selected cities
int num; // number of selected cities

//initialize: sort cities, calculate distances
void init(){
    int i, j;
    int key, key2;
    int tmp;

    s = (int*)malloc(sizeof(int)*num);
    r = (double*)malloc(sizeof(double)*(num-1));
    r2 =(double*)malloc(sizeof(double)*(num-2));
    sum = 0;

    //sort by 1:x, 2:y-------------------------------------------------------
    for (i=1; i<num; i++) {
        key = x[i]; key2 = y[i];
        tmp = (key%2==0)?-1:1; //x: 2n or 2n-1 > y: ascending or descending order
        for (j=i-1; j >= 0 && x[j]*1000+y[j]*tmp > key*1000+key2*tmp ; j--){
            x[j+1] = x[j]; y[j+1] = y[j];
        }
        x[j+1] = key; y[j+1] = key2;
    }

    for (i=0; i<num; i++) printf("%d %d\n",x[i],y[i]);
    // first, select all cities
    // calculate r, r2, sum
    for(i=0; i<num; i++){
        s[i] = i;
        if(i<num-1) sum += r[i] = _hypot(x[i+1]-x[i], y[i+1]-y[i]);
        if(i<num-2) r2[i] = _hypot(x[i+2]-x[i], y[i+2]-y[i]);
    }
}

//exclude cities until sum of distance is samller than 'max' value
//and print result
void exclude(int max){
    double tmp, tmp2;
    int i, ex;

    while(sum>max){
        //find a city which make shortest distance when excluded
        tmp = 0;
        for(i=1; i<num-1; i++){
            tmp2 = r[s[i-1]] + r[s[i]] - r2[s[i-1]];
            if(tmp2 > tmp){tmp = tmp2; ex = i;}
        }

        //printf("%d\n",s[ex]);
        //update index, distances
        memmove(&s[ex], &s[ex+1], sizeof(int)*(num-ex-1));
        sum -= tmp;
        r[s[ex-1]] = r2[s[ex-1]];
        r2[s[ex-1]] = _hypot(x[s[ex+1]]-x[s[ex-1]],y[s[ex+1]]-y[s[ex-1]]);
        if(ex>1) r2[s[ex-2]] = _hypot(x[s[ex]]-x[s[ex-2]], y[s[ex]]-y[s[ex-2]]);
        num--;
    }
    //print result
    printf("%d %.1f\n", max, sum);
    for(i=0; i<num; i++) printf("%d %d\n",x[s[i]],y[s[i]]);
    printf("%d",num);
    printf("\n\n");
}

int main()
{
    int tmp;
    char infile[1024];
    FILE *fp;

    printf("Input file name? ");
    scanf("%s",infile);

    if ( infile == NULL ) { fprintf(stderr, "NULL file name\n");}
    else{
        fp = fopen(infile,"r");
        if ( !fp ) { fprintf(stderr, "cannot open file %s\n",infile);}
        else{
            //read from file-------------------------------------------------
            num = (fscanf(fp, "%ld", &tmp),tmp);
            x = (int*)malloc(sizeof(int)*num);
            y = (int*)malloc(sizeof(int)*num);
           for(int i=0; i<num; i++){
                x[i] = (fscanf(fp, "%ld", &tmp),tmp);
                y[i] = (fscanf(fp, "%ld", &tmp),tmp);
            }
            //HW5--------------------------------------------------------------
            init();
            exclude(1500);
            exclude(1000);
            exclude(800);
            exclude(600);
            exclude(400);
            exclude(200);
        }
        fclose(fp);
    }
    return 0;
}
