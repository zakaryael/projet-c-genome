

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define n 25
#define m 1000000

typedef struct pick
{
    int a;
    int b;
    int key;
}pick;

void greedy(char* reads[], unsigned size, int k);
void substring(char str[],char substr[], unsigned long index1, unsigned long index2);
int find_str(char const* str, char const* substr);
int find(char* str1, char* str2, int beg, int end);
_Bool startswith(char* str1,char* str2);
int overlap(char* a, char* b, int min_lenth);
pick pick_maximal_overlap(char *reads[], unsigned, int k);
void _remove(char*** p, unsigned size, unsigned index);
void concatenateOverlap(char* str1,char* str2, int k);
void menu();
int recherche(char *f_souche,char *orig);
void clean(const char *buffer, FILE *fp);

//int count = 0;

int main()
{
    int cond=1;
    char c;
    while(cond){
        menu();
        c=getchar();
        while (getchar() != '\n');
        switch (c) {
        case '1' : {
                printf("\n\nle test est fait seulement pour les 25 premieres séquences du fichier donée");
                char  per[200];
                char** reads;
                reads = malloc(n * sizeof(char*));
                for (int i = 0; i < n; i++)
                    reads[i] = malloc(m * sizeof(char));
                FILE *f;
                f=fopen("text","r");
                    
                    for( int i = 0; i < n; ++i)
                    {
                         
                        
                        fscanf(f,"%s", reads[i]);
                        fscanf(f,"%s", per);
                        fscanf(f,"%s", per);
                        fscanf(f,"%s", per);
                    }
                    fclose(f);
                printf("\n");
    

                greedy(reads, n, 1);
                break;
            }
            case '2' : {
                char f_souche[100];
                char chaine_org[10000];
                printf("donner la chaine originale:\n");
                scanf("%s",chaine_org);
                printf("donner la fausse souche:\n");
                scanf("%s",f_souche);
                int test;
                test=recherche(f_souche,chaine_org);
                if (test==-1) {printf("\n\nla sequence ne contient pas cette fausse souche\n ");}
                else 
                    {printf("\n\nla sequence contient cette fausse souche\n");}
                break;
            }
            
            case 27 :{
                cond=0;break;
            }
            default :{
                printf("\nressayer le choix à nouveau!!!!\n");break;
            }
    }
    
    
 }   
   
}

void substring(char* str,char* substr, unsigned long index1, unsigned long index2) // extraire la sous chaine substr, de la chaine str, comprise entre les indices index1 et index2
{
    //methode alternative avec memcpy
    for(int i = 0; i < index2 - index1 ; ++i)
        substr[i] = str[i + index1];
        substr[index2 - index1] = '\0';
}

int find_str(char const* str, char const* substr) //si sbstr est inclus dans str, retourne l'indice de la premiere occurence de substr dans str, -1 sinon.
{
    char* pos = strstr(str, substr); //This function returns a pointer to the first occurrence in str of any of the entire sequence of characters specified in substr, or a null pointer if the sequence is not present in str.
    if(pos) {

        return pos - str;
    }
    else {
        return -1;
    }
}

_Bool startswith(char* str1, char* str2)// teste si str1 commence ou non avec str2
{
    if (find_str(str1, str2) == 0)
        return 1;
    return 0;
}

int overlap(char* a, char* b, int min_lenth)
{
    int start = 0;
    char* _a;// servira comme sous chaine de a
    char*  _b;// servira comme sous chaine de b
    _b =(char*) malloc((min_lenth + 1));
    substring(b, _b, 0, min_lenth);
    
    while(1)
    {
        _a =(char*) malloc(strlen(a) - start + 1);
        _a =(char*) realloc(_a,(strlen(a) - start) + 1);
        substring(a, _a, start, strlen(a));
        
        start = find_str(_a, _b) == -1 ? -1 : find_str(_a, _b) + start;
        if( start == -1 )
        {
            free(_a);
            free(_b);
            return 0;
        }
        _a =(char*) realloc(_a, (strlen(a) - start) + 1);
        substring(a, _a, start, strlen(a));
        
        if( startswith(b, _a) )
        {
            free(_a);
            free(_b);
            return strlen(a) - start;
        }
        start++;
        free(_a);
    }
}

pick pick_maximal_overlap(char** reads, unsigned size, int k)
{
    int a = 0; int b = 1;
    int olen;
    int best_olen = 0;
    for (int i = 0; i < size-1; ++i)
    {
        for (int j = i + 1; j < size; ++j)
        {
            olen = overlap(reads[i], reads[j], k);
            if (olen > best_olen)
            {
                a = i;
                b = j;
                best_olen = olen;

            }
            olen = overlap(reads[j], reads[i], k);
            if (olen > best_olen)
            {
                a = j;
                b = i;
                best_olen = olen;
            }
        }
    }
    struct pick best_pick = {a, b, best_olen};
    return best_pick;
}

void greedy(char* reads[], unsigned size, int k)
{
    pick best = pick_maximal_overlap(reads, size, 1);
   // printf("\n %d, %d, %d, %d \n", count++, best.key, best.a, best.b);
    int i = 0;
    while(size - i > 1)
    {
        reads[best.a] = realloc(reads[best.a], strlen(reads[best.b]) + strlen(reads[best.a]) - best.key + 1);

        //mecmcpy(reads[best.a]+strlen(reads[best.a]), reads[best.b] + best.key, strlen(reads[best.b]) - best.key + 1);
        concatenateOverlap(reads[best.a], reads[best.b], best.key);
      //  printf("%s\n", reads[best.a]);
        
        _remove(&reads, size - i, best.b);
        i++;
        best = pick_maximal_overlap(reads, size - i, 1);
/*        printf("\n %d, %d, %d, %d \n", count++, best.key, best.a, best.b);
        printf("reads after the %i th iteration:\n", i+1);
        for ( int o = 0; o < size - i; o++ )
            printf("%s ", reads[o]);
    }*/}
    printf("\n\n\n\n%s\n\n\n\n", reads[0]);
    }

void _remove(char*** p, unsigned size, unsigned index)
{
    char** temp = (char**) malloc((size - 1) * sizeof(char*));
    for (int i = 0; i < size - 1; ++i)
    {
        if ( i >= index)
        {
            temp[i] = malloc((strlen(*(*p+i+1)) + 1));
            strcpy(temp[i], *(*p+i+1));
        }
        else{
        temp[i] = malloc((strlen(*(*p+i) + 1)));
        strcpy(temp[i], *(*p+i));
        }
    }
    //free(p);
    *p = temp;

}

void concatenateOverlap(char* str1,char* str2, int k)
{
    
    unsigned long len  = strlen(str1);
    unsigned long len2 = strlen(str2);
    memcpy(str1 + len, str2 + k, len2 - k + 1);
    
}

// blob bobl blod

void menu(){
        printf("\n");
        printf("\n");
    printf("========================================================menu:========================================================================\n");
        printf("\n");
        printf("                                        1-ASSEMBLAGE DE SEQUENCES\n");
        printf("                                        2-RECHERCHE DES FAUSSES SOUCHES\n\n");
        printf("\tentrer le numero de la fonction que vous voullez utiliser ou ESC pour quitter:\n\n");
    printf("=====================================================================================================================================\n\n\n");
        
}
int recherche(char *f_souche,char *orig){
    int test1;
    test1=find_str(orig,f_souche);
    return test1;

}
void clean(const char *buffer, FILE *fp)
{
    char *p = strchr(buffer,'\n');
    if (p != NULL)
        *p = 0;
    else
    {
        int c;
        while ((c = fgetc(fp)) != '\n' && c != EOF);
    }
}
