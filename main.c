#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>


//TODO funzione booleana che restituisce true se la parola cercata è nel dizionario;
//TODO albero filtrate
//TODO pensare a un modo di realizzare i constraint
//TODO da correggere stampa filtrate

struct nodo
{
    char* parola;
    struct nodo* father;
    struct nodo* left;
    struct nodo* right;
};

typedef struct nodo Node;


struct dizionario
{
    Node* root;
    int size;
    int k;
};

typedef struct dizionario dizionario;

#define newGame 'n'
#define startDic 'i'
#define endDic 'f'
#define filtered 's'
#define maxComando 15

Node* newNode(char* string, int size)
{
    Node* newNode = malloc(sizeof(Node));
    newNode->parola= malloc(sizeof(char)*(size+1));
    for(int j = 0; j<size; j++)
    {
        newNode->parola[j]=string[j];
    }
    newNode->parola[size]='\0';
    newNode->father=NULL;
    newNode->left=NULL;
    newNode->right=NULL;
}

void bstInsertion(dizionario* T, Node* nodo)
{
    Node* y = NULL;
    Node* x = T->root;
    while(x!=NULL)
    {
        y = x;
        //TODO da riscrivere la strcmp
        if(strcmp(nodo->parola, x->parola) < 0)
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }
        nodo->father = y;
    }
    if(y==NULL)
    {
        T->root=nodo;
    }
    else if(strcmp(nodo->parola, y->parola) < 0)
    {
        y->left=nodo;
    }
    else
    {
        y->right=nodo;
    }
}

void insertInDict(dizionario* dizio, char* stringa)
{

    Node* node = newNode(&stringa[0], dizio->k);
    bstInsertion(dizio, node);
}

bool ricercaInDiz(Node *x,char* parolaF)
{


    if(x==NULL || parolaF==x->parola)
        return x;
    if(strcmp(parolaF, x->parola) < 0)
        return ricercaInDiz(x->left,parolaF);
    else
        return ricercaInDiz(x->right,parolaF);
}


Node* tree_minimum(Node* x)
{
    while(x->left!=NULL)
        x=x->left;
    return x;
}

Node* tree_successor(Node* x)
{
    Node *y;
    if(x->right!=NULL)
        return tree_minimum(x->right);
    y=x->parola;
    while(y!=NULL && x==y->right)
    {
        x=y;
        y=y->parola;
    }
    return y;
}
Node* delete(dizionario* T, Node* nodo)
{
    Node *y,*x;
    if(nodo->left==NULL ||  nodo->right==NULL)
        y=nodo;
    else
        y= tree_successor(nodo);
    if(y->left!=NULL)
        x=y->left;
    else
        x=y->right;
    if(x!=NULL)
        x->father=y->father;
    if(y->father==NULL)
        T->root=x;
    else if(y==y->father->left)
        y->father->left=x;
    else
        y->father->right=x;
    if(y!=nodo)
        nodo->parola=y->parola;
    return y;
}


void stampaRic(Node* x)
{
    if(x!=NULL)
    {
        stampaRic(x->left);
        printf("%s->",x->parola);
        stampaRic(x->right);
    }
}


int strCmp(char* c, char* p)
{
    int j=0;
    do
    {
        if(c[j]>p[j])
            return -1;
        else if(c[j]<p[j])
            return 1;
        j++;
    }while(c[j]!=NULL);
    return 0;
}

int main() {
    FILE *file;
    int gianni = 0;

    if (gianni) {
        file = stdin;
    } else {
        file = fopen("../Input.txt", "r");
    }
    int lengthWord = 0;

    if (file != NULL)
    {
        //lettura lunghezza parole
        fscanf(file, "%d\n", &lengthWord);
        char *comandi = malloc(sizeof(char) * (lengthWord + 1));
        char first;
        char *tempInput = malloc(sizeof(char) * (lengthWord + 1));
        char *riferimento = malloc(sizeof(char) * (lengthWord + 1));

        bool insertStartB = true;
        bool nuovaPartitaB = false;
        int tentativi = 0;

        dizionario* dizio = malloc(sizeof(dizionario));

        dizio->root = NULL;
        dizio->k=lengthWord;
        dizio->size = 0;

        do {
            //LETTURA CARATTERI
            fscanf(file, "%c", &first);

            //CASO DI PAROLE NON STD
            if (first != '+') {
                //leggo altri caratteri
                fgets(comandi, lengthWord, file);
                fgetc(file);
                //merging stringa finale
                tempInput[0] = first;
                for (int i = 1; i < lengthWord; i++) {
                    tempInput[i] = comandi[i - 1];
                }
                tempInput[lengthWord] = '\0';


                //INVIO PAROLA IN INPUT A FUNZIONE CONFRONTO O INSERIMENTO
                if (insertStartB) {
                    printf("%s\n", tempInput);
                    insertInDict(dizio,&tempInput[0]);
                    //INSERIMETO
                    dizio->size++;
                } else if (nuovaPartitaB) {
                    /* CONFRONTO
                     * confronto(tempInput, tentativi);*/
                    if (tentativi == 0) {
                        nuovaPartitaB = false;
                    }
                }
            }
                //CASO DI PAROLE STD
            else {
                fgets(comandi, 15, file);
                switch (comandi[0]) {
                    case newGame:
                        printf("%s", comandi);
                        //tolto \n, poi leggo parola di riferimento e poi max tentavi
                        //fgets(comandi,2,file);
                        if (nuovaPartitaB == false) {
                            fgets(riferimento, lengthWord +1, file);
                            fscanf(file, "%d\n", &tentativi);
                            printf("%s\n", riferimento);
                            printf("%d\n", tentativi);

                            comandi[0] = '\0';
                            nuovaPartitaB = true;
                            insertStartB = false;
                        } else { printf("Errore nuova partita in nuova partita"); }
                        break;
                    case startDic:
                        switch (comandi[10]) {
                            case startDic:
                                printf("Inzio inserimento\n");
                                fgets(comandi, 3, file);
                                insertStartB = true;
                                break;
                            case endDic:
                                printf("Fine inserimento\n");
                                fgets(comandi, 1, file);
                                insertStartB = false;
                                break;
                        }

                        break;
                    case filtered:
                        stampaRic(dizio->root);
                        break;
                }
            }
            //clear delle stringhe
            tempInput[0] = '\0';
            comandi[0] = '\0';

        } while (!feof(file));


        char* c="ciao";
        delete(dizio,c);
        stampaRic(dizio->root);

        printf("ciao\n");
    //TODO spostare il temp a prima
    }else
    {
        printf("Errore di lettura");
    }

    fclose(file);

}

typedef struct T T;
struct T {
    struct nodo *root;
};

typedef struct nodo nodo;
struct nodo {
    char* cont;
    struct nodo *left;
    struct nodo *right;
    struct nodo *p;
};


#define lengthWord 4

void insertInTree(char *input,struct nodo** root)
{
    char*input1= malloc(sizeof (char)*lengthWord);
    nodo* newNodo= malloc(sizeof (nodo));
    for(int i=0;i<lengthWord;i++)
    {
        input1[i]=input[i];
    }
    newNodo->cont=input1;
    newNodo->left=*root;
    *root=newNodo;
}

void insertSlide(struct T* t,struct nodo* z) {
    //struct nodo* temp=*z;
    struct nodo* y=NULL;
    struct nodo* x=t->root;
    int i=0;
    z->cont[0];
    printf("z %c",z->cont[0]);


    char*input1= malloc(sizeof (char)*lengthWord);
    nodo* newNodo= malloc(sizeof (nodo));
    for(int i=0;i<lengthWord;i++)
    {
        input1[i]=z->cont[i];
    }

    if(t->root->cont==NULL)
    {
        t->root->cont=z->cont;
        printf("z %s t%s",z->cont,t->root->cont);
    }


    while(x!=NULL)
    {
        y=x;
        if(z->cont[0] < x->cont[0])
        {
            x=x->left;
        }
        else
        {
            x=x->right;
        }
    }
    z->p=y;
    if(y==NULL)
    {
        t->root=z;
        printf("ciaasao ");
    }
    else if(z->cont[0] < y->cont[0])
        y->left=z;
    else
        y->right=z;

}

void printLista(nodo* lista)
{
    nodo *temp = lista;
    //iterate the entire linked list and print the data
    while(temp->left != NULL)
    {
        printf("%s->", temp->cont);
        temp = temp->left;
    }
    printf("NULL\n\n");
}

void ric(nodo* lista)
{
    if(lista!=NULL)
    {
        if(lista->left!=NULL)
            ric(lista->left);
        printf("%s->",lista->cont);
        if(lista->right!=NULL)
            ric(lista->right);
    }
}

int main() {

    struct T* t;
    //struct nodo* root= malloc(sizeof (nodo));
    char *tempInput1= malloc(sizeof (char)*(lengthWord+5));
    FILE *f;
    f= fopen("../Input.txt","r");
    t= malloc(sizeof (t));



    if(f!=NULL)
    {
        t->root= malloc(sizeof (nodo));
        do{
            fgets(tempInput1,lengthWord+2,f);
            printf("PAROLA IN INPUT:%s\n",tempInput1);

            insertInTree(tempInput1,&t->root);

            struct nodo* nodeInput= malloc(sizeof (nodo));
            nodeInput->cont=tempInput1;
            //insertSlide(t,nodeInput);
            //fgets(tempInput1,2,f);
        }
        while(!feof(f));

        printf("\n");
        //printLista(t->root);
        //ric(t->root);
        printf("\n");

    }
    else
    {
        printf("Errore in apertura del file\n");
    }
    return 0;
}
