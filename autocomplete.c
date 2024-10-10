#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "autocomplete.h"
#include <time.h>


// comment this out and include automplete.h
// typedef struct term{
//     char term[200]; // assume terms are not longer than 200
//     double weight;
// } term;

int compare_things(const void *a, const void *b){

    return strcmp(((struct term *)a)->term,((struct term *)b)->term);

}

int lowest_match(struct term *terms, int nterms, char *substr) {
    int high = nterms-1;
    int low = 0;
 
    //printf("the size of the substring is %d\n", szsub);
    int res = -1;
   
    while (low <= high) {
        int mid = low + (high - low) / 2;
        //this is the only time we need to call strncmp
        int comp  = strncmp((terms[mid]).term,substr,strlen(substr));
        if (comp == 0) {
 
            res = mid;
            high = mid-1;
       
        } else if(comp > 0){
 
            high = mid-1;
 
        } else {
                                                                                                                                                                                           
            low = mid + 1;
        }
    }
 
    return res;
 
}

    
//THIS IS EASY TO COMPLETE ONCE LOWEST MATCH WORKS
int highest_match(struct term *terms, int nterms, char *substr){

    int high = nterms-1;
    int low = 0;

    
    //printf("the size of the substring is %d\n", szsub);
    int res = -1;
   
    while (low <= high) {
       // printf("im in the while loop\n");
        int mid = low + (high - low) / 2;

            //this is the only time we need to call strncmp
        int comp  = strncmp((terms[mid]).term,substr,strlen(substr));
        if (comp == 0) {

            res = mid;
            low = mid+1;

        } else if(comp > 0){

            high = mid-1;

        } else {

            low = mid + 1;

        }

    }
    return res;

}

void read_in_terms(struct term **terms, int *pnterms, char *filename) {

    //printf("in read terms");
    FILE* thing;
    thing = fopen(filename, "r");

    if(thing == NULL){
        return;
    }

    char line[200];

    fgets(line, sizeof(line),thing);
    *pnterms = atof(line);
    //printf("%d\n", *pnterms);


    *terms = (term *)malloc((*pnterms)*sizeof (term));
    //

    for(int x = 0; x < *pnterms; x++){

        char lines[200];
        char w[200];
        char t[200];
        
        
        for(int v = 0; v < 200; v++){
            w[v] = '\0';
            t[v] = '\0';
            lines[v] = '\0';
        }



        fgets(lines, 200, thing);
        int i = 0;
        while(lines[i]!='\t'){
            w[i]= lines[i];
            i++;
        }
        i++;
        int j = 0;
        while(lines[i]!='\n'){
            t[j] = lines[i];
            j++;
            i++;
        }
        
        
        // //printf("%s\n", "start of the for loop");
        // char w[16];
        // char t[200];
        // char holdtab[3];
        
        // fgets(w, 15, thing);
        // fgets(holdtab, 2, thing);
        // fgets(t, 200, thing);

        // for (int i = 0; i<200; i++){
        //     if (t[i]=='\n') {
        //         t[i] = '\0';
        //     }
        // }

        term a;
        a.weight = strtod(w, NULL);
        strcpy(a.term, t);
        (*(*terms+x)) = a;
        //printf("Terms is: %s\n", a);

        for(int v = 0; v < 200; v++){

            lines[v] = '\0';
            w[v] = '\0';
            t[v] = '\0';

        }
        
    }

    //printf("about to do qsort");
    //is a star needed here? v
    qsort(*terms,*pnterms,sizeof(term),compare_things);

    // for(int r = 0; r < *pnterms; r++){
    //      printf("%s\t %f\n", (*terms)[r].term, (*terms)[r].weight);
    // }

}


int compare_weights(const void *a, const void *b) {

    // compare function for qsort
    struct term *a_s = (struct term *)a;
    struct term *b_s = (struct term *)b;
    
    if (a_s->weight < b_s->weight) {
        return 1;
    } else if (a_s->weight > b_s->weight) {
        return -1;
    } else {
        return 0;
    }

}

void autocomplete(struct term **answer, int *n_answer, struct term *terms, int nterms, char *substr){

    int highest = highest_match(terms, nterms, substr);
    
    //printf("The highest is %s\n ", (terms)[highest].term);
    int lowest = lowest_match(terms, nterms, substr);
    //printf("The lowest is %s\n ", (terms)[lowest].term);

    if (highest == -1){

        *n_answer = 0;
        
        return;

    } else if(lowest == -1) {

        *n_answer = 0;

        return;

    } else {
    
        int range = (highest-lowest)+1;
        //printf("The range is: %d\n", range);

        //malloc the array for answer (range + 1)

        *answer = (term *)malloc((range)*sizeof (term));

        //memcpy(*answer, &terms[lowest], (sizeof(struct term))*(range+1));

        for(int y = 0; y < range; y++){

            strcpy((*answer)[y].term, terms[lowest+y].term);
            (*answer)[y].weight = terms[lowest+y].weight;
        
        }

        // handle the case when there is no match so highest and lowest are both -1

        qsort(*answer, (range), sizeof(term), compare_weights);

        //printf("The highest is %s\n ", (terms)[highest].term);
        
        //printf("The lowest is %s\n ", (terms)[lowest].term);

        *n_answer = range;

       // int u = 0;

       // printf("The size of the answer array is: %d\n", (sizeof(**answer)/sizeof(term)));

        // for(int r = 0; r < range; r++){           

        //     printf("%s\t %f\n", (*answer)[r].term, (*answer)[r].weight);
        
        // }   
     }

}


// int main(void)
// {
//     time_t seconds;
//     seconds = time(NULL);
//     printf("Seconds beginning = %ld\n", seconds);
//     struct term *terms;
//     int nterms;
//     read_in_terms(&terms, &nterms, "wiktionary.txt");
//     // printf("this is nterms: %d\n", nterms);
//     // lowest_match(terms, nterms, "Tor");
//     // highest_match(terms, nterms, "Tor");
//     // printf("highest and lowest match have been found");
//     struct term *answer;
//     int n_answer;
//     autocomplete(&answer, &n_answer, terms, nterms, "th");
//     printf("Seconds end = %ld\n", seconds);

//     return 0;
// }