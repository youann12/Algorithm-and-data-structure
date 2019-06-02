//
//  main.c
//  assignment3
//
//  Created by youan on 14/4/19.
//  Copyright © 2019 z5193744. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <strings.h>


// This template is only a guide
// You need to include additional fields, data structures and functions

// data type for heap nodes
typedef struct HeapNode {
    // each node stores the priority (key), name, execution time,
    //  release time and deadline of one task
    int degree;
    int key; //key of this task
    int TaskName;  //task name
    int Etime; //execution time of this task
    int Rtime; // release time of this task
    int Dline; // deadline of this task
    struct HeapNode *Child;
    struct HeapNode *Parent;
    struct HeapNode *Sibling;
    struct HeapNode *same;
    //... // add additional fields here
} HeapNode;

//data type for a priority queue (heap)
typedef struct BinomialHeap{ //this is heap header
    int  size;      // count of items in the heap
    struct HeapNode *head;
   // ... // add additional fields here
} BinomialHeap;

typedef struct
{
    int name;
    char core[10];
    int start;
}corework;
// create a new heap node to store an item (task)
HeapNode *newHeapNode(int k, int n, int c, int r, int d)
{ // k:key, n:task name, c: execution time, r:release time, d:deadline
    // ... you need to define other parameters yourself)
    HeapNode *new;
    new = malloc(sizeof(HeapNode));
    assert(new != NULL);
    new->degree = 1;
    new->key = k;
    new->TaskName = n;
    new->Etime = c;
    new->Rtime = r;
    new->Dline = d;
    new->Child = NULL;
    new->Parent = NULL;
    new->Sibling = NULL;
    new->same = NULL;
    
    //... // initialise other fields
    return new;
}

// create a new empty heap-based priority queue
BinomialHeap *newHeap()
{ // this function creates an empty binomial heap-based priority queue
    BinomialHeap *T;
    T = malloc(sizeof(BinomialHeap));
    assert (T != NULL);
    T->size = 0;
    T->head = NULL;
    //... // initialise all the fields here
    return T;
}

BinomialHeap *merge(BinomialHeap *h1, BinomialHeap *h2)
{
    BinomialHeap *new = newHeap();
    new->size = h1->size + h2->size;
    HeapNode *b1,*b2,*a;
    b1 = h1->head;
    b2 = h2->head;
    a = new->head;
    while(b1 != NULL || b2 != NULL)
    {
        if(b1 != NULL && b2 != NULL && (b1->degree < b2->degree || (b1->degree == b2->degree && b1->key < b2->key)))
        {
            if(new->head == NULL)
            {
                new->head = b1;
                b1 = b1->Sibling;
                a = new->head;
            }
            else
            {
                a->Sibling = b1;
                b1 = b1->Sibling;
                a = a->Sibling;
            }
        }
        else if(b1 != NULL && b2 != NULL && (b1->degree > b2->degree || (b1->degree == b2->degree && b1->key >= b2->key)))
        {
            if(new->head == NULL)
            {
                new->head = b2;
                b2 = b2->Sibling;
                a = new->head;
            }
            else
            {
                a->Sibling = b2;
                b2 = b2->Sibling;
                a = a->Sibling;
            }
        }
        else if(b1 == NULL && b2 != NULL)
        {
            if(new->head == NULL)
            {
                new->head = b2;
                b2 = b2->Sibling;
                a = new->head;
            }
            else
            {
                a->Sibling = b2;
                b2 = b2->Sibling;
                a = a->Sibling;
            }
        }
        else if(b1 != NULL && b2 == NULL)
        {
            if(new->head == NULL)
            {
                new->head = b1;
                b1 = b1->Sibling;
                a = new->head;
            }
            else
            {
                a->Sibling = b1;
                b1 = b1->Sibling;
                a = a->Sibling;
            }
        }
    }
    return new;
}

void U(HeapNode *h1, HeapNode *h2)
{
    if(h1->degree > h2->degree || (h1->degree == h2->degree && h1->key > h2->key))
    {
        HeapNode *c = h2->Child;
        h2->Child = h1;
        h1->Parent = h2;
        h1->Sibling = c;
        h2->degree = h2->degree + h1->degree;
    }
    else if(h1->degree < h2->degree || (h1->degree == h2->degree && h1->key <= h2->key))
    {
        HeapNode *c = h1->Child;
        h1->Sibling = h2->Sibling;
        h1->Child = h2;
        h2->Parent = h1;
        h2->Sibling = c;
        h1->degree = h2->degree + h1->degree;
    }
}

BinomialHeap *Hunion(BinomialHeap *h1,BinomialHeap *h2)
{
    BinomialHeap *new = merge(h1,h2);
    HeapNode *prev;
    HeapNode *now;
    HeapNode *next;
    if(h1->head == NULL && h2->head != NULL)
    {
        return h2;
    }
    else if(h1->head != NULL && h2->head == NULL)
    {
        return h1;
    }
    else if(h1->head == NULL && h2->head == NULL)
    {
        return h1;
    }
    prev = new->head;
    now = new->head;
    next = now->Sibling;
    while(next != NULL)
    {
        if(now->degree != next->degree)
        {
            prev = now;
            now = now->Sibling;
            next = next->Sibling;
        }
        else if(next->Sibling != NULL && now->degree == next->degree && next->degree != next->Sibling->degree)
        {
            now->Sibling = next->Sibling;
            U(now,next);
            while(now->Parent != NULL)
            {
                now = now->Parent;
            }
            next = now->Sibling;
//            while(now->Parent != NULL)
//            {
//                now = now->Parent;
//            }
        }
        else if(next->Sibling != NULL && now->degree == next->degree && next->degree == next->Sibling->degree)
        {
            prev = now;
            now = now->Sibling;
            next = next->Sibling;
        }
        else if(next->Sibling == NULL && now->degree == next->degree)
        {
            if(prev == now)
            {
                U(now,next);
                while(now->Parent != NULL)
                {
                    now = now->Parent;
                }
                next = now->Sibling;
                
            }
            else
            {
                now->Sibling = next->Sibling;
                U(now,next);
                while(now->Parent != NULL)
                {
                    now = now->Parent;
                }
                next = now->Sibling;
                
                prev = now;
                now = NULL;
                next = NULL;
            }
        }
    }
    HeapNode *he = new->head;
    while(he->Parent != NULL)
    {
        he = he->Parent;
    }
    new->head = he;
    return new;
}

// the time complexity for Insert() is logn
void Insert(BinomialHeap *T, int k, int n, int c, int r, int d)
{ // k: key, n: task name, c: execution time, r: release time, d:deadline
    // You don't need to check if this task already exists in T
    //put your code here
    HeapNode *new = newHeapNode(k,n,c,r,d);
    BinomialHeap *newheapnode = newHeap();
    newheapnode->head = new;
    newheapnode->size = 1;
    
    if(T->head == NULL)
    {
        T->head = new;
        T->size++;
    }
    else
    {
        BinomialHeap *n = Hunion(T,newheapnode);
        T->head = n->head;
        T->size = n->size;
    }
}

BinomialHeap *Reverse(HeapNode* heap)
{
    HeapNode* next, *f;
    f = NULL;
    HeapNode* final = NULL;
    BinomialHeap *r = newHeap();
    HeapNode* sizeofr = heap;
    if(heap == NULL)
        return r;
    heap->Parent = NULL;
    while(sizeofr != NULL)
    {
        r->size = r->size + sizeofr->degree;
        f = sizeofr;
        sizeofr = sizeofr->Sibling;
    }
//    r->size = heap->degree;
    while (heap->Sibling)
    {
        next  = heap->Sibling;
        heap->Sibling = final;
        final = heap;
        heap = next;
    }
    heap->Sibling = final;
    r->head = f;
    return r;
}

HeapNode *Delete(BinomialHeap *T, HeapNode *node)
{
    HeapNode *prev, *tag;
    prev = NULL;
    tag = T->head;
    while(tag != node)
    {
        prev = tag;
        tag = tag->Sibling;
    }
    if(prev != NULL)
        prev->Sibling = node->Sibling;
    else
        T->head = node->Sibling;
    HeapNode *ch = node->Child;
    HeapNode *ch1 = node->Child;
    while(ch1 != NULL)
    {
        ch1->Parent = NULL;
        ch1 = ch1->Sibling;
    }
    node->Child = NULL;
    T->size = T->size - node->degree;
    node->degree = 1;
    BinomialHeap *NN = Hunion(T, Reverse(ch));
    T->size = NN->size;
    T->head = NN->head;
    
    return node;
}

// the time complexity of removemin is logn
HeapNode *RemoveMin(BinomialHeap *T)
{
    int a;
    HeapNode *n = T->head;
    HeapNode *s = n;
    a = n->key;
    while(n->Sibling != NULL)
    {
        n = n->Sibling;
        if(a > n->key)
        {
            a = n->key;
            s = n;
        }
    }
    if(a > n->key)
    {
        a = n->key;
        s = n;
    }
    return Delete(T,s);
}
//
// the time complexity of Min is logn
int Min(BinomialHeap *T)
{
//    if(T->head == NULL)
//    {
//        return
//    }
    if(T->head == NULL)
    {
        return -1;
    }
    int a;
    HeapNode *n = T->head;
    a = n->key;
    while(n->Sibling != NULL)
    {
        n = n->Sibling;
        if(a > n->key)
        {
            a = n->key;
        }
    }
    if(a >= n->key)
    {
        a = n->key;
    }
    return a;
}

// it build 3 binomioalheap and use the priorty queue to slove this problem, the time complexity is O(n*log n)
int TaskScheduler(char *f1, char *f2, int m )
{
    FILE *fp = NULL;
    FILE *fpx = NULL;
    int n,c,r,d,schedule,co;
    co = 1;
    BinomialHeap *core = newHeap();
    while(co <= m)//use a heap as linked list to store the core and their ready time
    {
        Insert(core, 0, co, 0, 0, 0);
        co++;
    }
    BinomialHeap *RH = newHeap();
    BinomialHeap *DH = newHeap();
    fp = fopen(f1,"r");
    char buffer[255];
    int zzz;
    if (fp == NULL)
    {
        perror("file1 does not exist.\n");
        exit(EXIT_FAILURE);
    }
    fpx = fopen(f2,"wb");
    if (fpx == NULL)
    {
        perror("file2 open error.\n");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        int flag = fscanf(fp," %d %d %d %d ",&n,&c,&r,&d);
        if(flag == EOF)
        {break;}
        else if(flag != 4)
        {
            printf("input error when reading the attribute of the task %d\n", n);
            exit(EXIT_FAILURE);
        }
    }
    fp = fopen(f1,"r");
    while (fscanf(fp," %d %d %d %d ",&n,&c,&r,&d) != EOF)
    {
        Insert(RH, r, n, c, r, d);
    }
    
    
    

    while(RH->size > 0 || DH->size > 0)
    {
        schedule = Min(RH);
        while(Min(RH) <= Min(core))
        {
            if(RH->size > 0)
            {
                HeapNode *r = RemoveMin(RH);
                Insert(DH, r->Dline, r->TaskName, r->Etime, r->Rtime, r->Dline);
            }
            else
            {
                break;
            }
        }
        if(DH->size > 0)
        {
            HeapNode *ddd = RemoveMin(DH);
            HeapNode *the_core = RemoveMin(core);
            int ssss = the_core->key;
            if(ssss >= ddd->Rtime)
            {
                ssss = the_core->key;
            }
            else
            {
                ssss = ddd->Rtime;
            }
            the_core->key = ssss + ddd->Etime;
            //printf("%d\n", ddd->TaskName);
            if(ssss + ddd->Etime > ddd->Dline)
            {
                zzz = sprintf(buffer,"%d core%d %d ", ddd->TaskName, the_core->TaskName, ssss);
                fwrite (buffer,strlen(buffer),1,fpx);
                zzz = sprintf(buffer,"...\n\nwhere task %d misses its deadline.", ddd->TaskName);
                fwrite (buffer,strlen(buffer),1,fpx);
                return 0;
            }
            zzz = sprintf(buffer,"%d core%d %d ", ddd->TaskName, the_core->TaskName, ssss);
            Insert(core, the_core->key, the_core->TaskName, 0, 0, 0);
            fwrite (buffer,strlen(buffer),1,fpx);
        }
        else 
        {
            while(Min(RH) > Min(core))
            {
                HeapNode *mcore = RemoveMin(core);
                Insert(core, Min(RH), mcore->TaskName, 0, 0, 0);
            }
        }

        
    }
    
    
//    while(RH->size > 0)
//    {
//        HeapNode *del = RemoveMin(RH);
//    }
    fclose(fp);
    return 1;
}

int main() //sample main for testing
{ int i;
    i=TaskScheduler("samplefile1.txt", "feasibleschedule1.txt", 4);
    if (i==0) printf("No feasible schedule!\n");
    /* There is a feasible schedule on 4 cores */
    i=TaskScheduler("samplefile1.txt", "feasibleschedule2.txt", 3);
    if (i==0) printf("No feasible schedule!\n");
    /* There is no feasible schedule on 3 cores */
    i=TaskScheduler("samplefile2.txt", "feasibleschedule3.txt", 5);
    if (i==0) printf("No feasible schedule!\n");
    /* There is a feasible schedule on 5 cores */
    i=TaskScheduler("samplefile2.txt", "feasibleschedule4.txt", 4);
    if (i==0) printf("No feasible schedule!\n");
    /* There is no feasible schedule on 4 cores */
    i=TaskScheduler("samplefile3.txt", "feasibleschedule5.txt", 2);
    if (i==0) printf("No feasible schedule!\n");
    /* There is no feasible schedule on 2 cores */
    i=TaskScheduler("samplefile4.txt", "feasibleschedule6.txt", 2);
    if (i==0) printf("No feasible schedule!\n");
     /*There is a feasible schedule on 2 cores*/
    return 0;
}

