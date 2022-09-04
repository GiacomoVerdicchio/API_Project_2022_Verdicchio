#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
//1 921 000 000
//0 568 000 000
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
    int height;
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
    }while(c[j]!='\0');
    return 0;
}

int max(int a, int b)
{
    return (a > b)? a : b;
}
int height(Node *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}
int getBalance(Node *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
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
    newNode->height = 0;
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
    newNode->height = 0;
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
    if (current == NULL)
        return toInsert;

    if (strCmpMia(toInsert->parola , current->parola) < 0)
    {
        current->left = insert(current->left, toInsert);
        current->left->father = current;
    }
    else
    {
        current->right = insert(current->right, toInsert);
        current->right->father = current;
    }

    current->height = max(height(current->left),height(current->right))+1;

    int balance = getBalance(current);

    if (balance > 1 && strCmpMia(toInsert->parola , current->left->parola) < 0 && current->left != NULL)
        return rightRotate(current);
    if (balance < -1 && strCmpMia(toInsert->parola, current->right->parola) > 0 && current->right != NULL)
        return leftRotate(current);
    if (balance > 1 && strCmpMia(toInsert->parola, current->left->parola) > 0 && current->left->right != NULL)
    {
        current->left =  leftRotate(current->left);
        return rightRotate(current);
    }
    if (balance < -1 && strCmpMia(toInsert->parola, current->right->parola) < 0 && current->right->left != NULL)
    {
        current->right = rightRotate(current->right);
        return leftRotate(current);
    }
    return current;
}
Node* insertInDict(dizionario* dizio, char* stringa)
{
    Node* node = newNode(&stringa[0], dizio->k);
    dizio->root = insert(dizio->root, node);
    dizio->size++;
    return node;
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
    y=x->father;
    while(y!=NULL && x==y->right)
    {
        x=y;
        y=y->father;
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

    if(x != NULL)
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

void confronto(char* in,int* buf)
{
    //TODO    da inserire qui ( o meglio ancora prima di chiamarla) la verifica che sia ammissibile
    // , altrimenti not exist
    for(int j=0;j<lengthWord;j++) {
        //Se +
        if (riferimento[j] == in[j]) {
            out[j] = '+';
            buf[in[j] - offset]--;
        }
    }
    for(int j=0;j<lengthWord;j++)
    {
        if (riferimento[j] != in[j])
        {
            if(buf[in[j] -offset]>0)
            {
                out[j]='|';
                buf[in[j] -offset]--;
            }
            else
            {
                out[j]='/';
            }
        }
    }
    out[lengthWord]='\0';
    printf("%s\n",out);
}
void apprendiVinc(char* in, int* buf)
{
    int temp;
    memset(cont, 0, sizeof(int) * caratteriBuf);

    for(int j=0;j<lengthWord;j++)
    {
        cont[in[j]-offset]++;
    }
    for(int j=0;j<lengthWord;j++)
    {
        temp=in[j]-offset;
        if(out[j]=='+')
        {
            //se la scopri adesso aggiorni il vincolo di almeno se era messo a true e l'esattamente
            if(vincC[j]==35)
            {
                if(!esattamente[temp] && cont[temp] > almeno[temp])
                {
                    almeno[temp]+=1;
                    if(almeno[temp] > buf[temp])
                    {
                        almeno[temp]-=1;
                    }
                }
                vincC[j]=in[j];
            }
        }
        else
        {
            if(out[j]=='|' && posSbagliata[j][temp] == false)
            {
                posSbagliata[j][temp]=true;
                if(cont[temp]>almeno[temp])
                {
                    almeno[temp] += 1;
                    if (almeno[temp] > buf[temp])
                    {
                        almeno[temp] -= 1;
                    }
                }
            }
        }
    }


    for(int j=0;j<lengthWord;j++)
    {
        if(out[j]=='/')
        {
            temp=in[j]-offset;
            if(almeno[temp]>0)
            {
                esattamente[temp]=true;
                almeno[temp]=buf[temp];
            }
            else
            {
                nonEsiste[temp]=true;
            }
            posSbagliata[j][temp]=true;
        }

    }
    vincC[lengthWord]='\0';
}

void Confronto_Apprendi(const char* in)
{
    int temp;
    memset(cont, 0, sizeof(int) * caratteriBuf);
    memset(out, 'A', sizeof(char) * lengthWord);
    for(int j = 0; j < lengthWord; j++)
    {
        temp=in[j] - offset;
        if (riferimento[j] == in[j])
        {
            out[j] = '+';
            vincC[j] = in[j];
            bufConfCopia[temp]--;
            cont[temp]=+1;
        }
    }

    for(int j=0; j < lengthWord; j++)
    {
        if(out[j] == 'A')
        {
            temp=in[j] - offset;
            if(bufConfCopia[temp] > 0)
            {
                bufConfCopia[temp]--;
                posSbagliata[j][temp]=1;
                out[j] = '|';
                cont[temp]+=1;
            }
            else
            {
                out[j] = '/';
                posSbagliata[j][temp]=1;
                if(cont[temp] > 0)
                {
                    esattamente[temp] = 1;
                    almeno[temp]=bufConf[temp];
                }
                else
                {
                    nonEsiste[temp]=1;
                }
            }
        }
    }

    for(int j = 0; j < lengthWord; j++)
    {
        temp=in[j] - offset;
        if(cont[temp] > almeno[temp])
        {
            almeno[temp] = cont[temp];
        }
    }

    out[lengthWord]='\0';
    puts(out);
}
_Bool rispettaVincoli(const char* in)
{
    int temp;
    int ref;
    memset(cont, 0, sizeof(int) * caratteriBuf);

    for(int j=0; j< lengthWord; j++)
    {
        temp=in[j]-offset;
        cont[temp]+=1;
        if(nonEsiste[temp])
        {
            return 0;
        }
        else if(vincC[j]!=35)
        {
            if(in[j]!=vincC[j])
            {
                return 0;
            }
        }
        else if(posSbagliata[j][temp])
        {
            return 0;
        }
    }

    for(int j=0;j<lengthWord;j++)
    {
        ref = riferimento[j] - offset;
        //caso in cui ho c### e asdo (c non Ã¨ rispettato)
        if(cont[ref] < almeno[ref])
        {
            return 0;
        }
        else
        {
            if(cont[ref] > almeno[ref] && esattamente[ref])
            {
                return 0;
            }
        }
    }
    return 1;
}

void scorriAlberoGiusta(Node* x, dizionario* dizio, dizionario* filtrate)
{
    if(x!=NULL)
    {
        //TODO da modificare
        if(rispettaVincoli(&x->parola[0]))
        {
            insertInTree(filtrate, x->parola);
        }
        scorriAlberoGiusta(x->left, dizio, filtrate);
        scorriAlberoGiusta(x->right, dizio, filtrate);
    }
}
void deleteTreeFinePartita(Node* x,dizionario* tree,bool delete)
{
    if(x!=NULL)
    {
        deleteTreeFinePartita(x->left, tree, delete);
        deleteTreeFinePartita(x->right, tree, delete);
        //TODO capire come mai non funziona correttamente la delete su parola
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
        riferimento = malloc(sizeof(char) * (lengthWord + 1));
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
            if(fscanf(file, "%c", &first))
            { }

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
                        }
                        else
                        {
                            tentativi-=1;
                            //confronto(&tempInput[0], bufConfCopia);
                            //apprendiVinc(&tempInput[0], bufConf);
                            Confronto_Apprendi(&tempInput[0]);
                            if (firstInsert) {
                                //scorriAlbero(dizio->root, dizio, treeFiltered);
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
                            }
                            //restore buffer ConfCopia (al posto di copiarlo che mi prende O(caratteriBuff) uso questa che
                            //è O(length)
                            memset(bufConfCopia, 0, sizeof(int) * caratteriBuf);
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
                if(fgets(comandi, 15, file))
                { }
                switch (comandi[0]) {
                    case newGame:
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
                                deleteTreeFinePartita(treeFiltered->root,treeFiltered, false);
                            }
                            //Inizializzazione albero
                            treeFiltered->root=NULL;
                            treeFiltered->k=lengthWord;
                            treeFiltered->size=0;
                            //Inizializzazione delle strutture per confronto e apprendimento vincoli

                            memset(bufConf, 0, sizeof(int) * caratteriBuf);
                            memset(bufConfCopia, 0, sizeof(int) * caratteriBuf);
                            memset(vincC, 35, sizeof(char) * lengthWord);
                            memset(cont, 0, sizeof(int) * caratteriBuf);
                            memset(almeno, 0, sizeof(int) * caratteriBuf);
                            memset(esattamente, 0, sizeof(bool) * caratteriBuf);
                            memset(nonEsiste, 0, sizeof(bool) * caratteriBuf);

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
                                if(fgets(comandi, 3, file))
                                { }
                                insertStartB = true;
                                break;
                            case endDic:
                                if(fgets(comandi, 1, file))
                                { }
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
                            stampaRicInOrder(dizio->root);
                        else
                            stampaRicInOrder(treeFiltered->root);
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


