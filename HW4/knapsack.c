#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void knapsack(double *item_w, double *item_v, int item_n, int max_w){
    double bag[max_w+1];
    double item_s[max_w+1][item_n];
    int tmp = 0;

    memset(bag,0,sizeof(double)*(max_w+1));
    memset(item_s,0,sizeof(double)*(max_w+1)*item_n);

    for(int i=1; i<item_n+1; i++){
        for(int w=max_w; w>0; w--){
            tmp = w-item_w[i-1];
            if(tmp >= 0){
                if(item_v[i-1] + bag[tmp] > bag[w]){
                    bag[w] = item_v[i-1] + bag[tmp];
                    memmove(&item_s[w][0], &item_s[tmp][0], sizeof(double)*item_n);
                    item_s[w][i-1] = 1;
                }
            }
        }
    }

    fprintf(stderr, "knapsack problem\n");
    for(int q=0; q<item_n; q++){
        if( item_s[max_w][q]>0 ){ fprintf(stderr, "%d ", q+1); }
    }
    fprintf(stderr, "%.0f\n", bag[max_w]);
}

void knapsack_onesplit(double *item_w, double *item_v, int item_n, int max_w){
    double bag[max_w+1];
    double item_s[max_w+1][item_n+1]; //item_n + split?
    int tmp, tmp2;

    memset(bag,0,sizeof(double)*(max_w+1));
    memset(item_s,0,sizeof(double)*(max_w+1)*(item_n+1));

    for(int i=1; i<item_n+1; i++){
        for(int w=max_w; w>0; w--){
            tmp = w-item_w[i-1];
            tmp2 = w-item_w[i-1]/2;
            if(tmp >= 0){
                if(item_v[i-1] + bag[tmp] > bag[w]){
                    bag[w] = item_v[i-1] + bag[tmp];
                    memmove(&item_s[w][0], &item_s[tmp][0], sizeof(double)*(item_n+1));
                    item_s[w][i-1] = 1;
                }
            }
            else if(tmp2 >= 0){
                if((item_v[i-1]/2 + bag[tmp2] > bag[w])&&(item_s[tmp2][item_n] == 0)){
                    bag[w] = item_v[i-1]/2 + bag[tmp2];
                    memmove(&item_s[w][0], &item_s[tmp][0], sizeof(double)*item_n);
                    item_s[w][i-1] = 0.5;
                    item_s[w][item_n] = 1;
                }
            }
        }
    }

    fprintf(stderr, "knapsack with one item split\n");
    for(int q=0; q<item_n; q++){
        if(item_s[max_w][q] == 1){ fprintf(stderr, "%d ", q+1); }
        else if(item_s[max_w][q] == 0.5){ fprintf(stderr, "%d*0.5 ", q+1); }
    }
    fprintf(stderr, "%.1f\n", bag[max_w]);
}

void knapsack_oneduplicate(double *item_w, double *item_v, int item_n, int max_w){
    double bag[max_w+1];
    double item_s[max_w+1][item_n+1];//item + double?
    int tmp, tmp2;

    memset(bag,0,sizeof(double)*(max_w+1));
    memset(item_s,0,sizeof(double)*(max_w+1)*(item_n+1));

    for(int i=1; i<item_n+1; i++){
        for(int w=max_w; w>0; w--){
            tmp2 = w-item_w[i-1]*2;
            tmp = w-item_w[i-1];
            if(tmp2 >= 0){
                if((item_v[i-1]*2 + bag[tmp2] > bag[w])&&(item_s[tmp2][item_n] == 0)){
                    bag[w] = item_v[i-1]*2 + bag[tmp2];
                    memmove(&item_s[w][0], &item_s[tmp][0], sizeof(double)*item_n);
                    item_s[w][i-1] = 2;
                    item_s[w][item_n] = 1;
                }
                else{
                    if(tmp >= 0){
                        if(item_v[i-1] + bag[tmp] > bag[w]){
                            bag[w] = item_v[i-1] + bag[tmp];
                            memmove(&item_s[w][0], &item_s[tmp][0], sizeof(double)*(item_n+1));
                            item_s[w][i-1] = 1;
                        }
                    }
                }
            }
            else if(tmp >= 0){
                if(item_v[i-1] + bag[tmp] > bag[w]){
                    bag[w] = item_v[i-1] + bag[tmp];
                    memmove(&item_s[w][0], &item_s[tmp][0], sizeof(double)*(item_n+1));
                    item_s[w][i-1] = 1;
                }
            }
        }
    }

    fprintf(stderr, "knapsack with one duplicate item\n");
    for(int q=0; q<item_n; q++){
        if( item_s[max_w][q] == 1 ){ fprintf(stderr, "%d ", q+1); }
        else if(item_s[max_w][q] == 2){ fprintf(stderr, "%d*2 ", q+1); }
    }
    fprintf(stderr, "%.0f\n", bag[max_w]);
}

void knapsack_twoknapsacks(double *item_w, double *item_v, int item_n, int max_w, int max_w2){
    double bag[max_w+1][max_w2+1];
    double item_s[max_w+1][max_w2+1][item_n];
    int tmp, tmp2;
    int flag = 0;

    memset(bag,0,sizeof(double)*(max_w+1)*(max_w2+1));
    memset(item_s,0,sizeof(double)*(max_w+1)*(max_w2+1)*(item_n+1));

    for(int i=1; i<item_n+1; i++){
        for(int w=max_w; w>0; w--){
            for(int w2=max_w2; w2>0;w2--){
                tmp = w-item_w[i-1];
                tmp2 = w2-item_w[i-1];
                if(tmp>=0 && tmp2>=0){
                    if(bag[tmp][w2] >= bag[w][tmp2]){
                        if(item_v[i-1] + bag[tmp][w2] > bag[w][w2]){ flag = 1; }
                    }
                    else{
                        if(item_v[i-1] + bag[w][tmp2] > bag[w][w2]){ flag = 2; }
                    }
                }
                else if(tmp>=0){
                    if(item_v[i-1] + bag[tmp][w2] > bag[w][w2]){ flag = 1; }
                }
                else if(tmp2>=0){
                    if(item_v[i-1] + bag[w][tmp2] > bag[w][w2]){ flag = 2; }
                }

                switch(flag){
                    case 1:
                        bag[w][w2] = item_v[i-1] + bag[tmp][w2];
                        memmove(&item_s[w][w2][0], &item_s[tmp][w2][0], sizeof(double)*(item_n+1));
                        item_s[w][w2][i-1] = 1;
                        break;
                    case 2:
                        bag[w][w2] = item_v[i-1] + bag[w][tmp2];
                        memmove(&item_s[w][w2][0], &item_s[w][tmp2][0], sizeof(double)*(item_n+1));
                        item_s[w][w2][i-1] = 2;
                        break;
                }
                flag = 0;
            }
        }
    }

    fprintf(stderr, "knapsack with two knapsacks\n");
    for(int q=0; q<item_n; q++){
        if( item_s[max_w][max_w2][q] > 0 ){ fprintf(stderr, "%d %.0f ", q+1, item_s[max_w][max_w2][q]); }
    }
    fprintf(stderr, "%.0f\n", bag[max_w][max_w2]);
}

int main()
{
    double *item_w;
    double *item_v;
    int item_n, tmp, num=0;
    int max_w, max_w2;

    char infile[1024];
    FILE *fp;

    printf("Input file name? ");
    scanf("%s",infile);

    if ( infile == NULL ) { fprintf(stderr, "NULL file name\n");}
    else{
        fp = fopen(infile,"r");
        if ( !fp ) { fprintf(stderr, "cannot open file %s\n",infile);}
        else{
            while(fscanf(fp, "%ld", &tmp) == 1){num++;}
            item_n = num/2 - 1;

            fclose(fp);
            fp = fopen(infile, "r");

            item_w = (double*)malloc(sizeof(double)*item_n);
            item_v = (double*)malloc(sizeof(double)*item_n);

            for(int i=0; i<item_n; i++){
                item_w[i] = (fscanf(fp, "%ld", &tmp),tmp);
                item_v[i] = (fscanf(fp, "%ld", &tmp),tmp);
            }
            max_w = (fscanf(fp, "%ld", &tmp), tmp);

            fprintf(stderr, "========================================\n");
            fprintf(stderr,"%d items, %d knapsack(s)", item_n, 1+num%2);
            fprintf(stderr, " - max: %d", max_w);

            if((num+1)%2){ //1 knapsack
                fprintf(stderr, " added\n");
                fprintf(stderr, "========================================\n");
                knapsack(item_w, item_v, item_n, max_w);
                fprintf(stderr, "========================================\n");
                knapsack_onesplit(item_w, item_v, item_n, max_w);
                fprintf(stderr, "========================================\n");
                knapsack_oneduplicate(item_w, item_v, item_n, max_w);
                fprintf(stderr, "========================================\n");
                knapsack_twoknapsacks(item_w, item_v, item_n, max_w, max_w);
                fprintf(stderr, "========================================\n");
            }
            else{ //2 knapsacks
                max_w2 = (fscanf(fp, "%ld", &tmp),tmp);
                fprintf(stderr, ", %d added\n", max_w2);
                fprintf(stderr, "========================================\n");
                knapsack_twoknapsacks(item_w, item_v, item_n, max_w, max_w2);
                fprintf(stderr, "========================================\n");
            }
            free(item_w);
            free(item_v);
        }
        fclose(fp);
    }
    return 0;
}
