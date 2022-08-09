#include <stdio.h>
#include <malloc.h>

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
