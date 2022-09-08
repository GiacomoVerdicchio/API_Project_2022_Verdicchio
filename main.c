#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//stuff per tempo
#include <time.h>

int caratteriBuf=64;

struct nodo
{
    char* parola;
    struct nodo* father;
    struct nodo* left;
    struct nodo* right;
    _Bool color;
};

typedef struct nodo Node;


struct dizionario
{
    Node* root;
    int size;
    int k;
};

typedef struct dizionario dizionario;

int lengthWord;
int* bufConf;
int* bufConfCopia;
char* out;
char *riferimento;

//Parte per apprendimento vincoli
char *vincC;
int *cont;
int* almeno;
_Bool** posSbagliata;
_Bool* esattamente;
_Bool* nonEsiste;

int strCmpMia(const char* c, const char* p,dizionario* T)
{
    for(int j=0;j<T->k;j++)
    {
        if(c[j]>p[j])
            return 1;
        else if(c[j]<p[j])
            return -1;

    }
    return 0;
}

void LeftRotate(dizionario * T,Node* x)
{
    Node* y = x->right;
    x->right = y->left;

    if(y->left!=NULL)
        y->left->father = x;

    y->father = x->father;

    if(x->father == NULL)
        T->root = y;

    else if(x == x->father->left)
        x->father->left = y;

    else
        x->father->right = y;

    y->left = x;

    x->father = y;

}
void RightRotate(dizionario * T,Node* x)
{
    Node* y = x->left;
    x->left = y->right;

    if(y->right!=NULL)
        y->right->father = x;

    y->father = x->father;

    if(x->father==NULL)
        T->root = y;

    else if(x== x->father->left)
        x->father->left = y;

    else
        x->father->right = y;

    y->right = x;
    x->father = y;

}
void rbInsertFIXUP(dizionario* dizio, Node* nodo)
{
    Node* grandparent = NULL;
    Node* parentpt = NULL;

    while (nodo!=dizio->root && (nodo->color != 0) && (nodo->father->color == 1))
    {
        parentpt = nodo->father;
        grandparent = nodo->father->father;

        if (parentpt == grandparent->left)
        {
            Node* uncle = grandparent->right;

            if (uncle!=NULL && uncle->color == 1)
            {
                grandparent->color=1;
                parentpt->color = 0;
                uncle->color= 0;
                nodo = grandparent;
            }
            else if (nodo == parentpt->right)
            {
                LeftRotate(dizio, parentpt);
                nodo = parentpt; // CASE 2
                parentpt=nodo->father;
            }
            else
            {
                RightRotate(dizio, grandparent);
                _Bool t=parentpt->color;
                parentpt->color =grandparent->color; // CASE 3, BLACK
                grandparent->color= t;
                nodo=parentpt;

            }
        }
        else {
            Node* uncle=grandparent->left;
            if (uncle!=NULL && uncle->color==1)
            {
                grandparent->color = 1;
                parentpt->color = 0; // CASE 4
                uncle->color=0;
                nodo = grandparent;
            }
            else if (nodo == parentpt->left)
            {
                RightRotate(dizio, parentpt);
                nodo = parentpt; // CASE 5
                parentpt=nodo->father;
            }
            else
            {
                LeftRotate(dizio, grandparent);
                _Bool t=parentpt->color;
                parentpt->color=grandparent->color;
                grandparent->color=t;
                nodo=parentpt;
            }
        }
    }
    dizio->root->color = 0;// BLACK
}
void rbInsertion(dizionario* T, Node* nodo)
{
    Node* y = NULL;
    Node* x = T->root;
    while(x!=NULL)
    {
        y = x;
        if(strCmpMia(nodo->parola, x->parola,T) < 0)
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
    else if(strCmpMia(nodo->parola, y->parola,T) < 0)
    {
        y->left=nodo;
    }
    else
    {
        y->right=nodo;
    }
    nodo->left= NULL;
    nodo->right=NULL;
    T->size++;

    rbInsertFIXUP(T, nodo);
}
Node* insertInDict(dizionario* dizio, char* stringa)
{
    int size=dizio->k;
    char* block = malloc(sizeof(Node)+ sizeof(char)* size + 1);
    Node* newNode = (Node*) &block[0];
    newNode->parola= &block[sizeof(Node)];
    strcpy(newNode->parola,stringa);
    newNode->parola[size]='\0';
    newNode->father=NULL;
    newNode->left=NULL;
    newNode->right=NULL;
    newNode->color=1;

    rbInsertion(dizio,newNode);
    return newNode;
}
void insertInTree(dizionario *tree, char *stringa)
{
    Node* newNode = malloc(sizeof(Node));
    newNode->parola= stringa;
    newNode->father=NULL;
    newNode->left=NULL;
    newNode->right=NULL;
    newNode->color=1;

    rbInsertion(tree, newNode);
}

_Bool isInDiz(Node *x, char* parolaF,dizionario*T)
{
    int te;
    if(x==NULL)
        return 0;
    else
    {
        te=strCmpMia(parolaF, x->parola,T);
        if(te!=0)
        {
            if (te > 0)
                return isInDiz(x->right, parolaF, T);
            else
                return isInDiz(x->left, parolaF,T);
        }
        else
            return 1;
    }


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

void StampaRicInOrder(Node* node)
{
    if(node != NULL)
    {
        StampaRicInOrder(node->left);
        puts(node->parola);
        StampaRicInOrder(node->right);
    }
}

int calcoloLettera(char c)
{
    if(c==45)
        return 0;
    else if(47<c && c<58)
        return c-47;
    else if(64<c && c<91)
        return c-54;
    else if(96<c && c<123)
        return c-59;
    else if(c==95)
        return 37;
    else
        return -1;
}
void Confronto_Apprendi(const char* in)
{
    memset(cont, 0, sizeof(int) * caratteriBuf);
    memset(out, 'A', sizeof(char) * lengthWord);

    int temp;
    char c;
    for(int j = 0; j < lengthWord; j++)
    {
        //temp=in[j] - offset;
        c=in[j];
        temp= calcoloLettera(c);
        if (riferimento[j] == c)
        {
            out[j] = '+';
            vincC[j] = c;
            bufConfCopia[temp]--;
            cont[temp]=+1;
        }
    }

    for(int j=0; j < lengthWord; j++)
    {
        if(out[j] == 'A')
        {
            //temp=in[j] - offset;
            temp= calcoloLettera(in[j]);
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
                //
                //esattamente[temp] = 1;
                //almeno[temp] = bufConf[temp];
            }
        }
    }

    int co;
    for(int j = 0; j < lengthWord; j++)
    {
        //temp=in[j] - offset;
        temp= calcoloLettera(in[j]);
        co=cont[temp];
        if(co > almeno[temp])
        {
            almeno[temp] = co;
        }
    }
    out[lengthWord]='\0';
    puts(out);
}
_Bool rispettaVincoli(const char* in)
{
    memset(cont, 0, sizeof(int) * caratteriBuf);

    char c,v;
    int temp;

    for(int j=0; j< lengthWord; j++)
    {
        //temp=in[j] - offset;
        c=in[j];
        v=vincC[j];
        temp= calcoloLettera(c);
        cont[temp]+=1;

        if(nonEsiste[temp])
        {
            return 0;
        }
        else if(v!=35)
        {
            if(c!=v)
            {
                return 0;
            }
        }
        else if(posSbagliata[j][temp])
        {
            return 0;
        }
    }
    int a,ref,co;
    for(int j=0;j<lengthWord;j++)
    {
        ref= calcoloLettera(riferimento[j]);
        co=cont[ref];
        a=almeno[ref];
        //ref = riferimento[j] - offset;
        //caso in cui ho c### e asdo (c non Ã¨ rispettato)
        if(co < a)
        {
            return 0;
        }
        else
        {

            if(co > a && esattamente[ref])
            {
                return 0;
            }
            /*
            if(esattamente[ref])
            {
                if((a==0 && co>a) || co>a)
                    return 0;
            }*/
        }
    }
    return 1;
}

void scorriAlberoGiusta(Node* x,dizionario* filtrate)
{
    if(x!=NULL)
    {
        if(rispettaVincoli(x->parola))
        {
            insertInTree(filtrate, x->parola);
        }
        scorriAlberoGiusta(x->left, filtrate);
        scorriAlberoGiusta(x->right,  filtrate);
    }
}
void deleteTreeFinePartita(Node* x,dizionario* tree)
{
    if(x!=NULL)
    {
        deleteTreeFinePartita(x->left, tree);
        deleteTreeFinePartita(x->right, tree);
        free(x);
    }
}
void scorriFiltrateDelete(Node* x, dizionario* tree)
{
    if(x!=NULL)
    {
        scorriFiltrateDelete(x->left, tree);
        scorriFiltrateDelete(x->right, tree);
        if (!rispettaVincoli(x->parola))
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
    free(cont);
    deleteTreeFinePartita(dizio->root,dizio);
    deleteTreeFinePartita(treeFiltered->root, treeFiltered);
    free(treeFiltered);
    free(dizio);
    for(int i=0;i<lengthWord;i++)
    {
        free(posSbagliata[i]);
    }
    free(posSbagliata);
    free(esattamente);
    free(bufConf);
    free(out);

}


int main() {

    //stuff per tempo
/*
    clock_t start, end;
    double execution_time;
    start = clock();
*/

    FILE *file;
    file = stdin;
    lengthWord = 0;

    if (file != NULL)
    {
        if(fscanf(file, "%d\n", &lengthWord))
        { }


        //Inizializzazione variabili
        char *comandi= malloc(sizeof(char) * 15);
        char *tempInput = malloc(sizeof(char) * (lengthWord + 1));
        riferimento = malloc(sizeof(char) * (lengthWord + 1));
        _Bool inserimentoParole = 1;
        //firstInsert mi serve per capire se inserire direttamente nel tree oppure prima devo vedere se filtrarlo
        _Bool firstInsert=1;
        //_Bool beforeEveryPartita=1;
        _Bool nuovaPartita = 0;
        int tentativi;


        int* block1 = malloc(2*caratteriBuf*sizeof (int*));
        bufConf= block1;
        bufConfCopia= &block1[caratteriBuf];

        //Parte per apprendimento vincoli
        char* block2 = malloc(2*(lengthWord +1)*sizeof (char*));
        out=block2;
        vincC= &block2[lengthWord+1];

        int* block3 = malloc(2*caratteriBuf*sizeof (int*));
        cont=block3;
        almeno=&block3[caratteriBuf];

        _Bool* block4 = malloc(sizeof (_Bool*)*2*caratteriBuf);
        esattamente=block4;
        nonEsiste=&block4[caratteriBuf];

        posSbagliata= malloc(sizeof (_Bool*) * lengthWord);
        for(int j=0; j < lengthWord; j++)
        {
            posSbagliata[j]=(_Bool*) malloc(sizeof(_Bool) * caratteriBuf);
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


        char c;
        int pos;
        do
        {
            memset(comandi,'\0',15);
            //LETTURA CARATTERI
            pos=0;
            do {
                c= getc(file);
                if(c!='\n')
                {
                    comandi[pos]=c;
                    pos++;
                }
            }while(c!='\n' && !feof(file));

            //comandi[pos]='\0';

            //CASO DI PAROLE NON STD
            if (comandi[0] != '+')
            {
                //INVIO PAROLA IN INPUT A FUNZIONE CONFRONTO O INSERIMENTO
                if (inserimentoParole)
                {
                    Node* newNode = insertInDict(dizio,comandi);
                    if(nuovaPartita && rispettaVincoli(comandi))
                    {
                        insertInTree(treeFiltered, newNode->parola);
                    }
                }
                else if (nuovaPartita)
                {
                    if(isInDiz(dizio->root,comandi,dizio))
                    {
                        if(strCmpMia(riferimento,comandi,dizio)==0)
                        {
                            puts("ok");
                            nuovaPartita = 0;
                        }
                        else
                        {
                            tentativi-=1;
                            Confronto_Apprendi(comandi);
                            //posso anche farlo con length e copiare il bunfConf
                            memcpy(bufConfCopia, bufConf, sizeof(int) * caratteriBuf);

                            int cT1;
                            for (int j = 0; j < lengthWord; j++)
                            {
                                cT1 = calcoloLettera(riferimento[j]);
                                if(bufConfCopia[cT1]!=0)
                                    bufConfCopia[cT1] =bufConf[cT1];
                            }


                            if (firstInsert) {
                                //scorro il dizionario e copio le parole da dizio a tree
                                scorriAlberoGiusta(dizio->root,  treeFiltered);
                                //Morris(dizio, treeFiltered);
                                firstInsert = 0;
                            } else
                            {
                                //tolgo le parole che non rispettano i vincoli
                                scorriFiltrateDelete(treeFiltered->root, treeFiltered);
                                //MorrisPerScorriDelete(treeFiltered);
                            }
                            printf("%d\n", treeFiltered->size);

                            if(tentativi==0)
                            {
                                puts("ko");
                                nuovaPartita = 0;
                                //se così funziona sarà da togliere l'assegnamento nella nuova partita
                                firstInsert = 0;
                            }
                        }
                    }
                    else
                    {
                        puts("not_exists");
                    }
                }
            }
                //CASO DI PAROLE STD
            else
            {
                char com=comandi[1];
                if(com=='n')
                {
                    if (nuovaPartita == 0)
                    {
                        if (fgets(riferimento, lengthWord + 1, file)) {}
                        if (fscanf(file, "%d\n", &tentativi)) {}
                        //comandi[0] = '\0';
                        nuovaPartita = 1;
                        inserimentoParole = 0;
                        firstInsert = 1;

                        deleteTreeFinePartita(treeFiltered->root, treeFiltered);
                        //Inizializzazione albero
                        treeFiltered->root = NULL;
                        treeFiltered->k = lengthWord;
                        treeFiltered->size = 0;
                        //Inizializzazione delle strutture per confronto e apprendimento vincoli


                        for (int i = 0; i < caratteriBuf; i++)
                        {
                            bufConf[i] = 0;
                            bufConfCopia[i] = 0;
                            almeno[i] = 0;
                            esattamente[i] = 0;
                            nonEsiste[i] = 0;
                        }
                        /*
                        memset(bufConf, 0, sizeof(int) * caratteriBuf);
                        memset(bufConfCopia, 0, sizeof(int) * caratteriBuf);
                        memset(almeno, 0, sizeof(int) * caratteriBuf);
                        memset(esattamente, 0, sizeof(_Bool) * caratteriBuf);
                        memset(nonEsiste, 0, sizeof(_Bool) * caratteriBuf);*/


                        //memset(vincC, 35, sizeof(char) * lengthWord);

                        int cT;
                        for (int j = 0; j < lengthWord; j++)
                        {
                            cT = calcoloLettera(riferimento[j]);
                            //bufConf[riferimento[j] - offset ]+=1;
                            bufConf[cT] += 1;
                            bufConfCopia[cT] += 1;
                            memset(posSbagliata[j], 0, sizeof(_Bool) * caratteriBuf);
                            vincC[j]=35;
                        }
                        //memcpy(bufConfCopia, bufConf, sizeof(int) * caratteriBuf);
                    } else {
                        printf("------Errore nuova partita in nuova partita\n");
                    }
                }
                else if(com=='i')
                {
                    //forse si può ulteriormente ottimizzare
                    char c1=comandi[11];
                    if(c1=='i')
                        inserimentoParole = 1;
                    else if(c1=='f')
                        inserimentoParole = 0;
                }
                else
                {
                    //STAMPA FILTRATE
                    //se faccio una stampa filtrate prima di apprendere constraint allora
                    //devo stampare il dizionario e non il tree
                    if(!firstInsert)
                        StampaRicInOrder(treeFiltered->root);
                    else
                        StampaRicInOrder(dizio->root);
                }
            }
        } while (!feof(file));

        _Bool debug=0;
        if(debug==0)
            liberaTutto(comandi,tempInput,dizio,treeFiltered);
    }
    else
    {
        printf("Errore di lettura");
    }


    //stuff per tempo
/*
    end = clock();
    execution_time = ((double)(end - start))/CLOCKS_PER_SEC;
    printf("%f\n", execution_time);
*/
    return 0;
}