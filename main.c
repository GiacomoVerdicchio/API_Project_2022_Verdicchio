#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define caratteriBuf 78
#define offset 45


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


typedef struct nodoL NodoLista;
struct nodoL {
    Node* cont;
    struct NodoLista *next;
};

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

int lengthWord;
int* bufConf;
int* bufConfCopia;
char* out;
char *riferimento;

//Parte per apprendimento vincoli
char *vincC;
int *cont;
int* almeno;
bool** posSbagliata;
bool* esattamente;
bool* nonEsiste;
int strCmpMia(char* c, char* p)
{
    int j=0;
    do
    {
        if(c[j]<p[j])
            return -1;
        else if(c[j]>p[j])
            return 1;
        j++;
    }while(c[j]!=0);
    return 0;
}
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
    return newNode;
}
Node* newNodeFiltr(char* string)
{
    Node* newNode = malloc(sizeof(Node));
    //da verificare questo assegnamento
    newNode->parola= string;
    newNode->father=NULL;
    newNode->left=NULL;
    newNode->right=NULL;
    return newNode;
}

Node *rightRotate(Node *y)
{
    Node *x = y->left;
    Node *T2 = x->right;

    if(x->right != NULL)
    {
        x->right->father = y;
    }

    y->left = T2;
    x->right = y;


    x->father = y->father;
    y->father = x;
    if (x->father != NULL && strCmpMia(y->parola, x->father->parola) < 0) {
        x->father->left = x;
    }
    else{
        if (x->father != NULL)
            x->father->right = x;
    }
    y = x;

    y->height = max(height(y->left),height(y->right))+1;
    x->height = max(height(x->left),height(x->right))+1;
    return x;
}

Node *leftRotate(Node *x)
{
    Node *y = x->right;
    Node *T2 = y->left;

    x->right = T2;

    if(y->left != NULL)
    {
        y->left->father = x;
    }
    y->left = x;

    y->father = x->father;
    x->father = y;

    if (y->father != NULL && strCmpMia(x->parola, y->father->parola) < 0) {
        y->father->left = y;
    }
    else{
        if (y->father != NULL)
            y->father->right = y;
    }


    x->height = max(height(x->left),height(x->right))+1;
    y->height = max(height(y->left),height(y->right))+1;
    return y;
}

Node* insert(Node* current, Node* toInsert)
{
    Node* y = NULL;
    Node* x = T->root;
    while(x!=NULL)
    {
        y = x;
        //TODO da riscrivere la strcmp
        if(strCmpMia(nodo->parola, x->parola) < 0)
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
    else if(strCmpMia(nodo->parola, y->parola) < 0)
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
void insertInTree(dizionario *tree, char *stringa)
{
    Node* node = newNodeFiltr(stringa);
    tree->root = insert(tree->root, node);
    tree->size++;
}
//da modificare
bool isInDiz(Node *x, char* parolaF)
{
    if(x==NULL)
        return false;
    else if(strCmpMia(parolaF, x->parola) == 0)
        return true;
    if(strCmpMia(parolaF, x->parola) < 0)
        return isInDiz(x->left, parolaF);
    else
        return isInDiz(x->right, parolaF);
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
void stampaRicInOrder(Node* x)
{
    if(x!=NULL)
    {
        stampaRicInOrder(x->left);
        printf("%s\n",x->parola);
        stampaRicInOrder(x->right);
    }
}
void stampaRicPREOrder(Node* x)
{
    if(x!=NULL)
    {
        printf("%s->",x->parola);
        stampaRicPREOrder(x->left);
        stampaRicPREOrder(x->right);
    }
}


NodoLista* inserisciTestaLista(NodoLista* lista, Node* x)
{
    NodoLista * newNodo= malloc(sizeof (NodoLista));
    newNodo->cont=x;
    newNodo->next= (struct NodoLista *) lista;
    lista=newNodo;
    return lista;
}
Node* TOGLI2(NodoLista** lista)
{
    NodoLista *top = *lista;
    Node *res = top->cont;
    *lista = (NodoLista *) top->next;
    free(top);
    return res;
}
void stampaIterativaInOrder(Node *tree)
{
    bool fine=false;
    NodoLista * lista= malloc(sizeof (NodoLista));
    lista->next=NULL;
    lista->cont=NULL;
    Node *curr=tree;

    while(!fine)
    {
        if(curr==NULL)
        {
            if(lista->cont!=NULL)
            {
                curr= TOGLI2(&lista);
                printf("%s\n", curr->parola);
                curr=curr->right;
            }
            else
                fine=true;
        }
        else
        {
            lista=inserisciTestaLista(lista, curr);
            curr=curr->left;
        }
    }
    free(lista);
}

void resetCont(char* in)
{
    for(int j=0;j<lengthWord;j++)
    {
        cont[in[j]-offset]=0;
    }
}
void confrontoEApprendi(char* in)
{
    int temp;
    for(int j=0;j<lengthWord;j++)
    {
        cont[in[j]-offset]++;
    }
    for(int j=0;j<lengthWord;j++) {
        temp=in[j]-offset;
        if (riferimento[j] == in[j])
        {
            out[j] = '+';
            bufConfCopia[in[j] - offset]--;
            if(vincC[j]==35)
            {
                if(!esattamente[temp] && cont[temp] > almeno[temp])
                {
                    almeno[temp]+=1;
                    if(almeno[temp] > bufConf[temp])
                    {
                        almeno[temp]-=1;
                    }
                }
                vincC[j]=in[j];
            }
        }
    }
    for(int j=0;j<lengthWord;j++)
    {
        if (riferimento[j] != in[j])
        {
            temp=in[j]-offset;
            if(bufConfCopia[in[j] -offset]>0)
            {
                out[j]='|';
                bufConfCopia[in[j] -offset]--;
                if(posSbagliata[j][temp] == false)
                {
                    posSbagliata[j][temp]=true;
                    if(cont[temp]>almeno[temp])
                    {
                        almeno[temp] += 1;
                        if (almeno[temp] > bufConf[temp])
                        {
                            almeno[temp] -= 1;
                        }
                    }
                }
            }
            else
            {
                out[j]='/';
                if(almeno[temp]>0)
                {
                    esattamente[temp]=true;
                    almeno[temp]=bufConf[temp];
                }
                else
                {
                    nonEsiste[temp]=true;
                }
                posSbagliata[j][temp]=true;
            }
        }
    }
    resetCont(in);
    for(int j=0;j<lengthWord;j++)
    {
        bufConfCopia[riferimento[j] - offset ]=0;
    }
    out[lengthWord]='\0';
    printf("%s\n",out);
    vincC[lengthWord]='\0';
}

bool rispettaVincoli(char* in)
{
    int temp;
    memset(cont, 0, sizeof(int) * caratteriBuf);
    for(int j=0;j<lengthWord;j++)
    {
        temp=in[j]-offset;

        //caso in cui ho c### e asdo (c non è rispettato)
        if(nonEsiste[temp])
        {
            resetCont(in);
            return false;
        }
        else if(vincC[j]!=35)
        {
            if(in[j]!=vincC[j])
            {
                resetCont(in);
                return false;
            }
        }
        //carattere in pos sbagliata
        else if(posSbagliata[j][temp])
        {
            resetCont(in);
            return false;
        } //troppi caratteri
        cont[temp]+=1;
    }

    for(int j=0;j<lengthWord;j++)
    {
        temp=in[j]-offset;
        if(almeno[temp]>0)
        {
            if(esattamente[temp])
            {
                if(cont[temp]!=almeno[temp])
                {
                    resetCont(in);
                    return false;
                }
            }
            else
            {
                if(cont[temp]<almeno[temp])
                {
                    resetCont(in);
                    return false;
                }
            }
        }
    }
    resetCont(in);
    return true;
}

void scorriAlberoGiusta(Node* x, dizionario* dizio, dizionario* filtrate)
{
    if(x!=NULL)
    {
        //TODO da modificare
        scorriAlberoGiusta(x->left, dizio, filtrate);
        if(rispettaVincoli(&x->parola[0]))
        {
            insertInTree(filtrate, x->parola);
        }
        scorriAlberoGiusta(x->right, dizio, filtrate);

    }
}
void deleteTreeFinePartita(Node* x,dizionario* tree,bool delete)
{
    if(x!=NULL)
    {
        deleteTreeFinePartita(x->left, tree, delete);
        deleteTreeFinePartita(x->right, tree, delete);
        if(delete)
        {
            free(x->parola);
        }
        free(x);
    }
}
void scorriFiltrateDelete(Node* x, dizionario* tree)
{
    if(x!=NULL)
    {
        scorriFiltrateDelete(x->left, tree);
        scorriFiltrateDelete(x->right, tree);
        if (!rispettaVincoli(&x->parola[0]))
        {
            rightX = x->right;
            Node* daTogliere=delete(tree, x);
            free(daTogliere);
            tree->size--;
        }
    }
}

void liberaTutto(char *comandi,char* tempInput,dizionario* dizio,dizionario* treeFiltered)
{
    free(comandi);
    free(tempInput);
    free(riferimento);
    free(vincC);
    free(cont);
    free(almeno);
    deleteTreeFinePartita(dizio->root,dizio, true);
    free(treeFiltered);
    free(dizio);
    for(int i=0;i<lengthWord;i++)
    {
        free(posSbagliata[i]);
    }
    free(posSbagliata);
    free(esattamente);
    free(nonEsiste);
    free(bufConf);
    free(bufConfCopia);
    free(out);
}

int main() {
    FILE *file;
    file = stdin;
    lengthWord = 0;

    if (file != NULL)
    {
        if(fscanf(file, "%d\n", &lengthWord))
        { }

        //Inizializzazione variabili
        char *comandi= malloc(sizeof(char) * 15);
        char first;
        char *tempInput = malloc(sizeof(char) * (lengthWord + 1));
        char *riferimento = malloc(sizeof(char) * (lengthWord + 1));
        bool insertStartB = true;
        //firstInsert mi serve per capire se inserire direttamente nel tree oppure prima devo vedere se filtrarlo
        bool firstInsert=true;
        bool beforeEveryPartita=true;
        bool nuovaPartitaB = false;
        int tentativi;


        //per Confronto: bufConf conta i caratteri e li salva sul buffer, out salva l'output da mandare
        //TODO implementazioni future:   bufCont implementato come lista (di char, occorenze, puntatore)
        bufConf= malloc(sizeof (int) * caratteriBuf);
        bufConfCopia= malloc(sizeof (int) * caratteriBuf);
        out= malloc(sizeof (char)*lengthWord+1);

        memset(bufConf, 0, sizeof(int) * caratteriBuf);
        memset(bufConfCopia, 0, sizeof(int) * caratteriBuf);

        //Parte per apprendimento vincoli
        vincC= malloc(sizeof (char) * (lengthWord + 5));
        cont= malloc(sizeof (int)*caratteriBuf);
        almeno= malloc(sizeof (int) * caratteriBuf);
        posSbagliata= malloc(sizeof (bool*) * lengthWord);
        esattamente= malloc(sizeof (bool)*caratteriBuf);
        nonEsiste= malloc(sizeof (bool)*caratteriBuf);
        for(int j=0; j < lengthWord; j++)
        {
            posSbagliata[j]=(bool*) malloc(sizeof(bool) * caratteriBuf);
        }

        //alberi
        dizionario* dizio = malloc(sizeof(dizionario));
        dizio->root = NULL;
        dizio->k=lengthWord;
        dizio->size = 0;

        dizionario* treeFiltered=malloc(sizeof(dizionario));
        treeFiltered->root=NULL;
        treeFiltered->k=lengthWord;
        treeFiltered->size=0;


        do
        {
            //LETTURA CARATTERI
            fscanf(file, "%c", &first);

            //CASO DI PAROLE NON STD
            if (first != '+') {
                if(fgets(comandi, lengthWord, file))
                {}
                fgetc(file);
                //merging stringa finale
                tempInput[0] = first;
                for (int i = 1; i < lengthWord; i++) {
                    tempInput[i] = comandi[i - 1];
                }
                tempInput[lengthWord] = '\0';


                //INVIO PAROLA IN INPUT A FUNZIONE CONFRONTO O INSERIMENTO
                if (insertStartB)
                {
                    Node* newNode = insertInDict(dizio,&tempInput[0]);
                    if(nuovaPartitaB && rispettaVincoli(&tempInput[0]))
                    {
                        insertInTree(treeFiltered, newNode->parola);
                    }
                } else if (nuovaPartitaB) {
                    if(isInDiz(dizio->root,&tempInput[0]))
                    {
                        if(strCmpMia(riferimento,tempInput)==0)
                        {
                            printf("ok\n");
                            nuovaPartitaB = false;
                            for(int j=0;j<lengthWord;j++)
                            {
                                bufConf[riferimento[j] - offset ]=0;
                                bufConfCopia[riferimento[j] - offset ]=0;
                            }
                        }
                        else
                        {
                            tentativi-=1;
                            confronto(riferimento, &tempInput[0], bufConfCopia, out, lengthWord);
                            apprendiVinc(&tempInput[0]);
                            //TODO scorro il dizionario o l'albero delle filtrate a seconda del boolean
                            // e stampo il numero del size dell'albero delle filtrate
                            if (firstInsert) {
                                scorriAlberoGiusta(dizio->root, dizio, treeFiltered);
                                firstInsert = false;
                            } else {
                                scorriFiltrateDelete(treeFiltered->root, treeFiltered);
                            }

                            printf("%d\n", treeFiltered->size);
                            if(tentativi==0)
                            {
                                printf("ko\n");
                                nuovaPartitaB = false;
                                //restore dei buffer non nella nuova partita perché dovrei farlo in carB
                                for(int j=0;j<lengthWord;j++)
                                {
                                    bufConf[riferimento[j] - offset ]=0;
                                    bufConfCopia[riferimento[j] - offset ]=0;
                                }
                            }
                            //restore buffer ConfCopia (al posto di copiarlo che mi prende O(caratteriBuff) uso questa che
                            //è O(length)
                            //memset(bufConfCopia, 0, sizeof(int) * caratteriBuf);
                            for(int j=0;j<lengthWord;j++) {
                                bufConfCopia[riferimento[j] - offset] = 0;
                            }
                            for(int j=0;j<lengthWord;j++)
                            {
                                bufConfCopia[riferimento[j] - offset ]+=1;
                            }
                        }
                    }
                    else
                    {
                        printf("not_exists\n");
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
                            if(fgets(riferimento, lengthWord +1, file))
                            { }
                            if(fscanf(file, "%d\n", &tentativi))
                            { }
                            comandi[0] = '\0';
                            nuovaPartitaB = true;
                            insertStartB = false;
                            firstInsert = true;

                            if(beforeEveryPartita)
                            { beforeEveryPartita=false;
                            }
                            else
                            {   //Eliminazione Tree Precedente
                                deleteTreeFinePartita(treeFiltered->root,treeFiltered);
                            }
                            //Inizializzazione albero
                            treeFiltered->root=NULL;
                            treeFiltered->k=lengthWord;
                            treeFiltered->size=0;
                            //Inizializzazione delle strutture per confronto e apprendimento vincoli

                            //memset(bufConf, 0, sizeof(int) * caratteriBuf);
                            //memset(bufConfCopia, 0, sizeof(int) * caratteriBuf);
                            memset(vincC, 35, sizeof(char) * lengthWord);
                            memset(cont, 0, sizeof(int) * caratteriBuf);
                            memset(almeno, 0, sizeof(int) * caratteriBuf);
                            memset(esattamente, 0, sizeof(bool) * caratteriBuf);
                            memset(nonEsiste, 0, sizeof(bool) * caratteriBuf);

                            memset(bufConf, 0, sizeof(int) * caratteriBuf);
                            memset(bufConfCopia, 0, sizeof(int) * caratteriBuf);
                            for(int j=0;j<lengthWord;j++)
                            {
                                bufConf[riferimento[j] - offset ]+=1;
                                bufConfCopia[riferimento[j] - offset]+=1;
                            }
                            for(int j=0; j < lengthWord; j++)
                            {
                                memset(posSbagliata[j], false, sizeof(bool) * caratteriBuf );
                            }
                        }
                        else { printf("------Errore nuova partita in nuova partita\n"); }
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
                        if(fgets(comandi, 2, file))
                        { }
                        break;
                    case filtered:
                        //se faccio una stampa filtrate prima di apprendere constraint allora
                        //devo stampare il dizionario e non il tree
                        if(firstInsert)
                            stampaIterativaInOrder(dizio->root);
                        else
                            stampaIterativaInOrder(treeFiltered->root);
                        if(fgets(comandi,3,file))
                        { }
                        break;
                }
            }
            //clear delle stringhe
            tempInput[0] = '\0';
            comandi[0] = '\0';
        } while (!feof(file));

        bool debug=0;
        if(debug==0)
            liberaTutto(comandi,tempInput,dizio,treeFiltered);
    //TODO spostare il temp a prima
    }else
    {
        printf("Errore di lettura");
    }
    return 0;
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
