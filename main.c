#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//stuff per tempo
#include <time.h>

#define caratteriBuf 64
#define offset 45

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
_Bool** posSbagliata;
_Bool* esattamente;
_Bool* nonEsiste;

int strCmpMia(const char* c, const char* p)
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
void rbInsertFIXUP2(dizionario* dizio, Node* nodo)
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
    nodo->left= NULL;
    nodo->right=NULL;
    T->size++;
    rbInsertFIXUP2(T, nodo);
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
    newNode->color=NULL;
    Node* nodeF=newNode;

    rbInsertion(dizio,nodeF);
    return nodeF;
}
void insertInTree(dizionario *tree, char *stringa)
{
    Node* newNode = malloc(sizeof(Node));
    newNode->parola= stringa;
    newNode->father=NULL;
    newNode->left=NULL;
    newNode->right=NULL;
    newNode->color=NULL;
    Node* nodeF=newNode;

    rbInsertion(tree, nodeF);
}

_Bool isInDiz(Node *x, char* parolaF)
{
    if(x==NULL)
        return 0;
    else if(strCmpMia(parolaF, x->parola) == 0)
        return 1;
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
    else if(48<=c && c<=57)
        return c-47;
    else if(65<=c && c<=90)
        return c-54;
    else if(97<=c && c<=122)
        return c-59;
    else if(c==95)
        return 37;
    else
        return -1;
}


void Confronto_Apprendi(const char* in)
{
    int temp;
    memset(cont, 0, sizeof(int) * caratteriBuf);
    memset(out, 'A', sizeof(char) * lengthWord);
    for(int j = 0; j < lengthWord; j++)
    {
        //temp=in[j] - offset;
        temp= calcoloLettera(in[j]);
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
            }
        }
    }

    for(int j = 0; j < lengthWord; j++)
    {
        //temp=in[j] - offset;
        temp= calcoloLettera(in[j]);
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
        //temp=in[j] - offset;
        temp= calcoloLettera(in[j]);
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
        ref= calcoloLettera(riferimento[j]);
        //ref = riferimento[j] - offset;
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
        if(rispettaVincoli(&x->parola[0]))
        {
            insertInTree(filtrate, x->parola);
        }
        scorriAlberoGiusta(x->left, dizio, filtrate);
        scorriAlberoGiusta(x->right, dizio, filtrate);
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
        if (!rispettaVincoli(&x->parola[0]))
        {
            Node* daTogliere=delete(tree, x);
            free(daTogliere);
            tree->size--;
        }
    }
}


void Morris(dizionario *tree, dizionario* filtrate)
{
    Node *current, *pre, *root;
    root=tree->root;
    if (root == NULL)
        return;

    current = root;
    while (current != NULL) {

        if (current->left == NULL) {
            if (rispettaVincoli(&current->parola[0]))
            {
                insertInTree(filtrate, &current->parola[0]);
            }
            current = current->right;
        }
        else {

            /* Find the inorder predecessor of current */
            pre = current->left;
            while (pre->right != NULL
                   && pre->right != current)
                pre = pre->right;

            /* Make current as the right child of its
               inorder predecessor */
            if (pre->right == NULL) {
                pre->right = current;
                current = current->left;
            }

                /* Revert the changes made in the 'if' part to
                   restore the original tree i.e., fix the right
                   child of predecessor */
            else {
                pre->right = NULL;
                if (rispettaVincoli(&current->parola[0]))
                {
                    insertInTree(filtrate, &current->parola[0]);
                }
                current = current->right;
            } /* End of if condition pre->right == NULL */
        } /* End of if condition current->left == NULL*/
    } /* End of while */
}
void MorrisPerScorriDelete(dizionario *tree)
{
    Node *current, *pre, *root;
    root=tree->root;
    if (root == NULL)
        return;

    current = root;
    while (current != NULL) {

        if (current->left == NULL) {
            if (!rispettaVincoli(&current->parola[0]))
            {
                Node* daTogliere=delete(tree, current);
                free(daTogliere);
                tree->size--;
            }
            current = current->right;
        }
        else {

            /* Find the inorder predecessor of current */
            pre = current->left;
            while (pre->right != NULL
                   && pre->right != current)
                pre = pre->right;

            /* Make current as the right child of its
               inorder predecessor */
            if (pre->right == NULL) {
                pre->right = current;
                current = current->left;
            }

                /* Revert the changes made in the 'if' part to
                   restore the original tree i.e., fix the right
                   child of predecessor */
            else {
                pre->right = NULL;
                if (!rispettaVincoli(&current->parola[0]))
                {
                    Node* daTogliere=delete(tree, current);
                    free(daTogliere);
                    tree->size--;
                }
                current = current->right;
            } /* End of if condition pre->right == NULL */
        } /* End of if condition current->left == NULL*/
    } /* End of while */
}

void liberaTutto(char *comandi,char* tempInput,dizionario* dizio,dizionario* treeFiltered)
{
    free(comandi);
    free(tempInput);
    free(riferimento);
    free(vincC);
    free(cont);
    free(almeno);
    deleteTreeFinePartita(dizio->root,dizio);
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
        char first;
        char *tempInput = malloc(sizeof(char) * (lengthWord + 1));
        riferimento = malloc(sizeof(char) * (lengthWord + 1));
        _Bool insertStartB = 1;
        //firstInsert mi serve per capire se inserire direttamente nel tree oppure prima devo vedere se filtrarlo
        _Bool firstInsert=1;
        _Bool beforeEveryPartita=1;
        _Bool nuovaPartitaB = 0;
        int tentativi;


        bufConf= malloc(sizeof (int) * caratteriBuf);
        bufConfCopia= malloc(sizeof (int) * caratteriBuf);
        out= malloc(sizeof (char)*lengthWord+1);

        //Parte per apprendimento vincoli
        vincC= malloc(sizeof (char) * (lengthWord + 1));
        cont= malloc(sizeof (int)*caratteriBuf);
        almeno= malloc(sizeof (int) * caratteriBuf);
        posSbagliata= malloc(sizeof (_Bool*) * lengthWord);
        esattamente= malloc(sizeof (_Bool)*caratteriBuf);
        nonEsiste= malloc(sizeof (_Bool)*caratteriBuf);
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
                            puts("ok");
                            nuovaPartitaB = 0;
                        }
                        else
                        {
                            tentativi-=1;
                            Confronto_Apprendi(&tempInput[0]);

                            if (firstInsert) {
                                scorriAlberoGiusta(dizio->root, dizio, treeFiltered);
                                //Morris(dizio, treeFiltered);
                                firstInsert = 0;
                            } else {
                                scorriFiltrateDelete(treeFiltered->root, treeFiltered);
                                //MorrisPerScorriDelete(treeFiltered);
                            }

                            printf("%d\n", treeFiltered->size);
                            if(tentativi==0)
                            {
                                puts("ko");
                                nuovaPartitaB = 0;
                            }
                            //restore buffer ConfCopia (al posto di copiarlo che mi prende O(caratteriBuff) uso questa che
                            //Ã¨ O(length)
                            memcpy(bufConfCopia, bufConf, sizeof(int) * caratteriBuf);
                        }
                    }
                    else
                    {
                        puts("not_exists");
                    }
                }
            }
                //CASO DI PAROLE STD
            else {
                if(fgets(comandi, 15, file))
                { }
                switch (comandi[0]) {
                    case newGame:
                        if (nuovaPartitaB == 0) {
                            if(fgets(riferimento, lengthWord +1, file))
                            { }
                            if(fscanf(file, "%d\n", &tentativi))
                            { }
                            comandi[0] = '\0';
                            nuovaPartitaB = 1;
                            insertStartB = 0;
                            firstInsert = 1;

                            if(beforeEveryPartita)
                            { beforeEveryPartita=0;
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


                            memset(vincC, 35, sizeof(char) * lengthWord);
                            for(int i = 0; i < caratteriBuf; i++)
                            {
                                bufConf[i]=0;
                                almeno[i]=0;
                                esattamente[i]=0;
                                nonEsiste[i]=0;
                            }
                            /*
                            memset(bufConf, 0, sizeof(int) * caratteriBuf);
                            memset(almeno, 0, sizeof(int) * caratteriBuf);
                            memset(esattamente, 0, sizeof(bool) * caratteriBuf);
                            memset(nonEsiste, 0, sizeof(bool) * caratteriBuf);
                            */
                            for(int j=0;j<lengthWord;j++)
                            {
                                //bufConf[riferimento[j] - offset ]+=1;
                                bufConf[calcoloLettera(riferimento[j])]+=1;
                            }
                            memcpy(bufConfCopia, bufConf, sizeof(int) * caratteriBuf);
                            for(int j=0; j < lengthWord; j++)
                            {
                                memset(posSbagliata[j], 0, sizeof(_Bool) * caratteriBuf );
                            }
                        }
                        else { printf("------Errore nuova partita in nuova partita\n"); }
                        break;
                    case startDic:
                        switch (comandi[10]) {
                            case startDic:
                                if(fgets(comandi, 3, file))
                                { }
                                insertStartB = 1;
                                break;
                            case endDic:
                                if(fgets(comandi, 1, file))
                                { }
                                insertStartB = 0;
                                break;
                        }
                        if(fgets(comandi, 2, file))
                        { }
                        break;
                    case filtered:
                        //se faccio una stampa filtrate prima di apprendere constraint allora
                        //devo stampare il dizionario e non il tree
                        if(firstInsert)
                            StampaRicInOrder(dizio->root);
                        else
                            StampaRicInOrder(treeFiltered->root);
                        if(fgets(comandi,3,file))
                        { }
                        break;
                }
            }
            //clear delle stringhe
            tempInput[0] = '\0';
            comandi[0] = '\0';
        } while (!feof(file));

        _Bool debug=0;
        if(debug==0)
            liberaTutto(comandi,tempInput,dizio,treeFiltered);
        //TODO spostare il temp a prima
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