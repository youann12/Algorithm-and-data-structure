#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#define  INF  10000000


// A vertex is a 2D point
typedef struct Vertex {
    int x; // x-coordinate
    int y; // y-coordinate
    
} Vertex;
// each edge is a pair of vertices (end-points)
typedef struct Edge {
    Vertex *p1; // first end point
    Vertex *p2; // second end point

} Edge;

typedef struct adjnode {
    Vertex *v;
    struct adjnode *adj;
    int weight;
} adjnode;

typedef struct VertexNode {
    int id; // to identify the vertex
    Vertex *v;
    struct VertexNode *next_node;
    adjnode *adj;
    int visited;
    struct HeapNode *heapnode;
    
} VertexNode;

typedef struct queue {
    VertexNode *head;
    VertexNode *tail;
} queue;

typedef struct GraphRep *Graph;
typedef struct GraphRep { // graph header
    VertexNode *first; // an array of vertices or a linked list of vertices
    int nV; // #vertices
    int nE; // #edges
} GraphRep;



typedef struct HeapNode {
    int degree;
    int key;
    Vertex *v;
    struct HeapNode *Child;
    struct HeapNode *Parent;
    struct HeapNode *Sibling;
    struct HeapNode *before;
    VertexNode *vnode;


} HeapNode;

//data type for a priority queue (heap)
typedef struct BinomialHeap{ //this is heap header
    int  size;      // count of items in the heap
    struct HeapNode *head;

} BinomialHeap;


HeapNode *newHeapNode(int k, Vertex *vn, VertexNode *vnd)
{
    HeapNode *new;
    new = malloc(sizeof(HeapNode));
    assert(new != NULL);
    new->degree = 1;
    new->key = k;
    new->v = vn;
    new->Child = NULL;
    new->Parent = NULL;
    new->Sibling = NULL;
    new->vnode = vnd;
    new->before = NULL;
    vnd->heapnode = new;
    
    return new;
}

BinomialHeap *newHeap()
{
    BinomialHeap *T;
    T = malloc(sizeof(BinomialHeap));
    assert (T != NULL);
    T->size = 0;
    T->head = NULL;

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

void Insert(BinomialHeap *T, int k, Vertex *v, VertexNode *nd)
{
    HeapNode *new = newHeapNode(k,v,nd);
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
int Min(BinomialHeap *T)
{

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

void decrease(BinomialHeap *T, HeapNode *h, int k)
{
    h->key = k;
    if(h->Parent == NULL)
    {
        h->key = k;
    }
    else
    {
        while(h->Parent != NULL && h->Parent->key > h->key)
        {
            VertexNode *vnp = h->Parent->vnode;
            VertexNode *vn = h->vnode;
            HeapNode *pbefore = h->Parent->before;
            HeapNode *bef = h->before;
            vnp->heapnode = h;
            vn->heapnode = h->Parent;
            h->Parent->vnode = vn;
            h->vnode = vnp;
            HeapNode *p = h->Parent;
            Vertex *pv = p->v;
            int pk = p->key;
            p->before = bef;
            h->before = pbefore;
            p->key = h->key;
            p->v = h->v;
            h->key = pk;
            h->v = pv;
            h = p;
        }
    }
}

// A vertex node stores a vertex and other information, and you need to expand this type

//The above types serve as a starting point only. You need to expand them and add more types.
// Watch the lecture video between 7:50pm-8:20 or so for my discussion about this assignment

float length(Vertex *v1, Vertex *v2)
{
    float l = sqrtf((v1->x - v2->x) * (v1->x - v2->x) + (v1->y - v2->y) * (v1->y - v2->y));
    return l;
}

Vertex *createVertex(int x1, int y1)
{
    Vertex *v;
    v = malloc(sizeof(Vertex));
    v->x = x1;
    v->y = y1;
    return v;
    
}

queue *Createqueue()
{
    queue *q;
    q = malloc(sizeof(queue));
    q->head = NULL;
    q->tail = NULL;
    return q;
}

void insertqueue(queue *q, VertexNode *v)
{
    if(q->head == NULL)
    {
        q->head = v;
        q->tail = v;
    }
    else
    {
        VertexNode *h = q->head;
        while(h->next_node != NULL)
        {
            h = h->next_node;
        }
        h->next_node = v;
        q->tail = v;
    }
}
VertexNode *pop(queue *q)
{
    if(q->head == q->tail)
    {
        VertexNode *qh = q->head;
        q->head = NULL;
        q->tail = NULL;
        return qh;
    }
    VertexNode *f = q->head;
    q->head = f->next_node;
    f->next_node = NULL;
    return f;
}



// the create graph just initialized the graph, time complexity is O(1)
Graph CreateEmptyGraph()
{
    GraphRep *G;
    G = malloc(sizeof(Graph));
    G->nE = 0;
    G->nV = 0;
    G->first = NULL;
    
    return G;
}

VertexNode *CreateVertexNode(Vertex *ve, int id)
{
    VertexNode *V;
    V = malloc(sizeof(VertexNode));
    V->id = id;
    V->v = ve;
    V->next_node = NULL;
    V->adj = NULL;
    V->visited = 0;
    V->heapnode = NULL;
    
    return V;
}

adjnode *Createadjnode(Vertex *ve, int weight)
{
    adjnode *a;
    a = malloc(sizeof(adjnode));
    a->v = ve;
    a->weight = weight;
    a->adj = NULL;
    return a;
}

VertexNode *findvn(Graph g, Vertex *ve)
{
    VertexNode *a = g->first;

    while(a != NULL)
    {
        if(a == NULL)
            return NULL;
        if(a->v->x == ve->x && a->v->y == ve->y)
            return a;
        a = a->next_node;
    }
    return a;
}

adjnode *findadj(VertexNode *vn, Vertex *ve)
{
    adjnode *a = vn->adj;

    while(a != NULL)
    {
        if(a == NULL)
            return NULL;
        if(a->v->x == ve->x && a->v->y == ve->y)
            return a;
        a = a->adj;
    }
    return a;
}



// insert edge is just insert two vertex and two adjnode, the time complexity is O(m + n)
int InsertEdge(Graph g, Edge *e)
{
    if(g->first == NULL)
    {
        g->first = CreateVertexNode(e->p1,0);
        g->first->next_node = CreateVertexNode(e->p2,1);
        adjnode *a1 = Createadjnode(e->p2,length(e->p1,e->p2));
        adjnode *a2 = Createadjnode(e->p1,length(e->p1,e->p2));
        g->first->adj = a1;
        g->first->next_node->adj = a2;
        g->nE++;
        g->nV++;
        g->nV++;
        return 1;
    }
    else if(findvn(g,e->p1) != NULL && findvn(g,e->p2) != NULL)
    {
        VertexNode *pf = findvn(g,e->p1);
        VertexNode *ps = findvn(g,e->p2);
        if(findadj(pf,ps->v) != NULL && findadj(ps,pf->v) != NULL)
            return 0;
        else
        {
            adjnode *a1 = Createadjnode(e->p2,length(e->p1,e->p2));
            adjnode *a2 = Createadjnode(e->p1,length(e->p1,e->p2));
            
            adjnode *add1 = findvn(g,e->p1)->adj;
            adjnode *add2 = findvn(g,e->p2)->adj;
            if(add1 == NULL)
                findvn(g,e->p1)->adj = a1;
            else
            {
                while(add1->adj != NULL)
                    add1 = add1->adj;
                add1->adj = a1;
            }
            if(add2 == NULL)
                findvn(g,e->p2)->adj = a2;
            else
            {
                while(add2->adj != NULL)
                    add2 = add1->adj;
                add2->adj = a2;
            }
            g->nE++;
        }
        return 1;
    }
    else if(findvn(g,e->p1) == NULL && findvn(g,e->p2) != NULL)
    {
        VertexNode *in1 = CreateVertexNode(e->p1,g->nV + 1);
        VertexNode *a = g->first;
        while(a->next_node != NULL)
        {
            a = a->next_node;
        }
        a->next_node = in1;
        g->nV++;
        
        
        adjnode *a1 = Createadjnode(e->p2,length(e->p1,e->p2));
        adjnode *a2 = Createadjnode(e->p1,length(e->p1,e->p2));
        adjnode *add1 = findvn(g,e->p1)->adj;
        adjnode *add2 = findvn(g,e->p2)->adj;
        if(add1 == NULL)
            findvn(g,e->p1)->adj = a1;
        else
        {
            while(add1->adj != NULL)
                add1 = add1->adj;
            add1->adj = a1;
        }
        if(add2 == NULL)
            findvn(g,e->p2)->adj = a2;
        else
        {
            while(add2->adj != NULL)
                add2 = add1->adj;
            add2->adj = a2;
        }
        g->nE++;
        return 1;
    }
    else if(findvn(g,e->p1) != NULL && findvn(g,e->p2) == NULL)
    {
        VertexNode *in2 = CreateVertexNode(e->p2,g->nV + 1);
        VertexNode *a = g->first;
        while(a->next_node != NULL)
        {
            a = a->next_node;
        }
        a->next_node = in2;
        g->nV++;
        adjnode *a1 = Createadjnode(e->p2,length(e->p1,e->p2));
        adjnode *a2 = Createadjnode(e->p1,length(e->p1,e->p2));
        adjnode *add1 = findvn(g,e->p1)->adj;
        adjnode *add2 = findvn(g,e->p2)->adj;
        if(add1 == NULL)
            findvn(g,e->p1)->adj = a1;
        else
        {
            while(add1->adj != NULL)
                add1 = add1->adj;
            add1->adj = a1;
        }
        if(add2 == NULL)
            findvn(g,e->p2)->adj = a2;
        else
        {
            while(add2->adj != NULL)
                add2 = add1->adj;
            add2->adj = a2;
        }
        g->nE++;
        return 1;
    }
    else if(findvn(g,e->p1) == NULL && findvn(g,e->p2) == NULL)
    {
        VertexNode *in1 = CreateVertexNode(e->p1,g->nV + 1);
        VertexNode *aa = g->first;
        while(aa->next_node != NULL)
        {
            aa = aa->next_node;
        }
        aa->next_node = in1;
        g->nV++;
        
        VertexNode *in2 = CreateVertexNode(e->p2,g->nV + 1);
        VertexNode *a = g->first;
        while(a->next_node != NULL)
        {
            a = a->next_node;
        }
        a->next_node = in2;
        g->nV++;
        
        adjnode *a1 = Createadjnode(e->p2,length(e->p1,e->p2));
        adjnode *a2 = Createadjnode(e->p1,length(e->p1,e->p2));
        adjnode *add1 = findvn(g,e->p1)->adj;
        adjnode *add2 = findvn(g,e->p2)->adj;

        if(add1 == NULL)
            findvn(g,e->p1)->adj = a1;
        else
        {
            while(add1->adj != NULL)
            {
                add1 = add1->adj;
            }
            add1->adj = a1;
        }
        if(add2 == NULL)
            findvn(g,e->p2)->adj = a2;
        else
        {
            while(add2->adj != NULL)
            {
                add2 = add1->adj;
            }
            add2->adj = a2;
        }
        g->nE++;
        return 1;
    }
    return 0;
}


void printgraph(Graph g)
{
    VertexNode *aa = g->first;
    while(aa != NULL)
    {
        printf("(%d, %d) node = ", aa->v->x,aa->v->y);
        adjnode *ad = aa->adj;
        while(ad != NULL)
        {
            printf("(%d, %d) ", ad->v->x, ad->v->y);
            ad = ad->adj;
        }
        printf("\n");
        aa = aa->next_node;
    }
}



// the delete edge is just find the vertex and delete the adjnode, so is O(m + n)
void DeleteEdge(Graph g, Edge *e)
{
    VertexNode *pf = findvn(g,e->p1);
    VertexNode *ps = findvn(g,e->p2);
    VertexNode *temp = g->first;
    if(findvn(g,e->p1) != NULL && findvn(g,e->p2) != NULL && findadj(pf,ps->v) != NULL && findadj(ps,pf->v) != NULL)
    {
        g->nE--;
        if(pf->adj->v->x == ps->v->x && pf->adj->v->y == ps->v->y)
        {
            pf->adj = pf->adj->adj;
        }
        else
        {
            adjnode *adjpre = pf->adj;
            while(adjpre->adj->v->x != ps->v->x && adjpre->adj->v->y != ps->v->y)
            {
                adjpre = adjpre->adj;
            }
            adjpre->adj = adjpre->adj->adj;
        }
        if(ps->adj->v->x == pf->v->x && ps->adj->v->y == pf->v->y)
        {
            ps->adj = ps->adj->adj;
        }
        else
        {
            adjnode *adjpre = ps->adj;
            while(adjpre->adj->v->x != pf->v->x && adjpre->adj->v->y != pf->v->y)
            {
                adjpre = adjpre->adj;
            }
            adjpre->adj = adjpre->adj->adj;
        }
        if(pf->adj == NULL)
        {
            if(pf == g->first)
            {
                g->first = pf->next_node;
            }
            else
            {
                while(temp->next_node != pf)
                {
                    temp = temp->next_node;
                }
                temp->next_node = pf->next_node;
            }
            g->nV--;
        }
        if(ps->adj == NULL)
        {
            if(ps == g->first)
            {
                g->first = ps->next_node;
            }
            else
            {
                while(temp->next_node != ps)
                {
                    temp = temp->next_node;
                }
                temp->next_node = ps->next_node;
            }
            g->nV--;
        }
        
    }
}

void setnotvisit(Graph g)
{
    VertexNode *s = g->first;
    while(s != NULL)
    {
        s->visited = 0;
        s = s->next_node;
    }
}

void Reachable(Graph g, Vertex *v)
{

    //    int vx = v->x;
    //    int vy = v->y;
    VertexNode *VV = findvn(g, v);  // n
    if(VV->visited == 0)
    {
        VV->visited = 1;    // 1
        adjnode *a = VV->adj; // 1
        while(a != NULL) // m
        {
            VertexNode *V1 = findvn(g, a->v);  //
            if(V1->visited != 1)
                printf("(%d, %d) ", a->v->x, a->v->y);
            Reachable(g,a->v); 
            a = a->adj;
        }
    }

}
// this function uses the queue and recursive function to find all vertces, so the time complexity is O(mn)


void ReachableVertices(Graph g, Vertex *v)
{

    setnotvisit(g);
    Reachable(g, v);
    printf("\n");
}

BinomialHeap *sethp(Graph g, Vertex *s)
{
    BinomialHeap *BH = newHeap();
    VertexNode *f = g->first;
    while(f != NULL)
    {
        if(f->v->x != s->x || f->v->y != s->y)
        {
            Insert(BH, INF, f->v,f);
        }
        else if(f->v->x == s->x && f->v->y == s->y)
        {
            Insert(BH, 0, f->v, f);
        }
        f = f->next_node;
    }
    return BH;
}
// this function uses BinomialHeap to find the smallest path node, so time complexity is O((m+n)log n).
void ShortestPath(Graph g, Vertex *goal, Vertex *u)
{
    setnotvisit(g);
    if(findvn(g, u) != NULL && findvn(g, goal) != NULL)
    {
        BinomialHeap *BH = sethp(g, u);
        while(BH->size != 0)
        {
            HeapNode *s = RemoveMin(BH);
            s->vnode->visited = 1;
            VertexNode *vs = s->vnode;
            adjnode *adjvs = vs->adj;
            while(adjvs != NULL)
            {
                if(adjvs->weight + s->key < findvn(g, adjvs->v)->heapnode->key && findvn(g, adjvs->v)->visited == 0)
                {
                    decrease(BH, findvn(g, adjvs->v)->heapnode, adjvs->weight + s->key);
                    findvn(g, adjvs->v)->heapnode->before = s;
                }
                adjvs = adjvs->adj;
            }
        }
        HeapNode *prin = findvn(g, goal)->heapnode;
        if(prin->before != NULL)
        {
            while(prin != NULL)
            {
                printf("(%d, %d), ", prin->v->x,prin->v->y);
                prin = prin->before;
            }
        }
    }
    printf("\n");
    
}

// this function should free all the vertex and edge, so time complexity is O(n + m) which n is the number of vertex and m is number of edge;
void FreeGraph(Graph g)
{
    VertexNode *aa = g->first;
    VertexNode *temp;
    adjnode *tempadj;
    while(aa != NULL)
    {
        temp = aa->next_node;
        adjnode *ad = aa->adj;
        while(ad != NULL)
        {
            tempadj = ad->adj;
            free(ad);
            ad = tempadj;
        }
        free(aa);
        aa = temp;
    }
}

int searchqueue(queue *q, Vertex *vn)
{
    VertexNode *f = q->head;
    while(f != NULL)
    {
        if(f->v->x == vn->x && f->v->y == vn->y)
        {
            return 1;
        }
        f = f->next_node;
    }
    return 0;
}

void visit(Graph g)
{
    VertexNode *fir = g->first;
    while(fir != NULL)
    {
        queue *q = Createqueue();
        Vertex *v = createVertex(fir->v->x, fir->v->y);
        VertexNode *vn = CreateVertexNode(v, 0);
        findvn(g, v)->visited = 1;
        insertqueue(q, vn);
        

        while(q->head != NULL)
        {
            VertexNode *out = pop(q);
            findvn(g, out->v)->visited = 1;
            adjnode *a = findvn(g, out->v)->adj;
            while(a != NULL)
            {
                v = createVertex(a->v->x, a->v->y);
                if(findvn(g, v)->visited == 0)
                {
                    printf("(%d, %d), (%d, %d) ", out->v->x, out->v->y, a->v->x, a->v->y);

                    v = createVertex(a->v->x, a->v->y);
                    vn = CreateVertexNode(v, 0);
                    if(findvn(g, v)->visited == 0 && searchqueue(q, v) != 1)
                    {
                        insertqueue(q, vn);
                    }
                }
                a = a->adj;
            }

        }
        fir = fir->next_node;
    }
}

// this function should print all the vertex and edge, so time complexity is O(n + m) which n is the number of vertex and m is number of edge;

void ShowGraph(Graph g)
{
    setnotvisit(g);
    visit(g);
    printf("\n");
}

int main() //sample main for testing
{
    Graph g1;
    Edge *e_ptr;
    Vertex *v1, *v2;
    
    // Create an empty graph g1;
    g1=CreateEmptyGraph();
    
    // Create first connected component
    // Insert edge (0,0)-(0,10)
    e_ptr = (Edge*) malloc(sizeof(Edge));
    assert(e_ptr != NULL);
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=0;
    v1->y=0;
    v2->x=0;
    v2->y=10;
    e_ptr->p1=v1;
    e_ptr->p2=v2;
    if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
    
    // Insert edge (0,0)-(5,6)
    e_ptr = (Edge*) malloc(sizeof(Edge));
    assert(e_ptr != NULL);
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=0;
    v1->y=0;
    v2->x=5;
    v2->y=6;
    e_ptr->p1=v1;
    e_ptr->p2=v2;
    if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
    
    // Insert edge (0, 10)-(10, 10)
    e_ptr = (Edge*) malloc(sizeof(Edge));
    assert(e_ptr != NULL);
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=0;
    v1->y=10;
    v2->x=10;
    v2->y=10;
    e_ptr->p1=v1;
    e_ptr->p2=v2;
    if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
    
    // Insert edge (0,10)-(5,6)
    e_ptr = (Edge*) malloc(sizeof(Edge));
    assert(e_ptr != NULL);
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=0;
    v1->y=10;
    v2->x=5;
    v2->y=6;
    e_ptr->p1=v1;
    e_ptr->p2=v2;
    if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
    
    // Insert edge (0,0)-(5,4)
    e_ptr = (Edge*) malloc(sizeof(Edge));
    assert(e_ptr != NULL);
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=0;
    v1->y=0;
    v2->x=5;
    v2->y=4;
    e_ptr->p1=v1;
    e_ptr->p2=v2;
    if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
    
    // Insert edge (5, 4)-(10, 4)
    e_ptr = (Edge*) malloc(sizeof(Edge));
    assert(e_ptr != NULL);
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=5;
    v1->y=4;
    v2->x=10;
    v2->y=4;
    e_ptr->p1=v1;
    e_ptr->p2=v2;
    if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
    
    // Insert edge (5,6)-(10,6)
    e_ptr = (Edge*) malloc(sizeof(Edge));
    assert(e_ptr != NULL);
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=5;
    v1->y=6;
    v2->x=10;
    v2->y=6;
    e_ptr->p1=v1;
    e_ptr->p2=v2;
    if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
    
    // Insert edge (10,10)-(10,6)
    e_ptr = (Edge*) malloc(sizeof(Edge));
    assert(e_ptr != NULL);
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=10;
    v1->y=10;
    v2->x=10;
    v2->y=6;
    e_ptr->p1=v1;
    e_ptr->p2=v2;
    if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
    
    // Insert edge (10, 6)-(10, 4)
    e_ptr = (Edge*) malloc(sizeof(Edge));
    assert(e_ptr != NULL);
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=10;
    v1->y=6;
    v2->x=10;
    v2->y=4;
    e_ptr->p1=v1;
    e_ptr->p2=v2;
    if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
    
    // Create second connected component
    // Insert edge (20,4)-(20,10)
    e_ptr = (Edge*) malloc(sizeof(Edge));
    assert(e_ptr != NULL);
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=20;
    v1->y=4;
    v2->x=20;
    v2->y=10;
    e_ptr->p1=v1;
    e_ptr->p2=v2;
    if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
    
    // Insert edge (20,10)-(30,10)
    e_ptr = (Edge*) malloc(sizeof(Edge));
    assert(e_ptr != NULL);
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=20;
    v1->y=10;
    v2->x=30;
    v2->y=10;
    e_ptr->p1=v1;
    e_ptr->p2=v2;
    if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
    
    // Insert edge (25,5)-(30,10)
    e_ptr = (Edge*) malloc(sizeof(Edge));
    assert(e_ptr != NULL);
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=25;
    v1->y=5;
    v2->x=30;
    v2->y=10;
    e_ptr->p1=v1;
    e_ptr->p2=v2;
    if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
    
    //Display graph g1
    ShowGraph(g1);
    
    // Find the shortest path between (0,0) and (10,6)
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=0;
    v1->y=0;
    v2->x=10;
    v2->y=6;
    ShortestPath(g1, v1, v2);
    free(v1);
    free(v2);
    
    // Delete edge (0,0)-(5, 6)
    e_ptr = (Edge*) malloc(sizeof(Edge));
    assert(e_ptr != NULL);
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=0;
    v1->y=0;
    v2->x=5;
    v2->y=6;
    e_ptr->p1=v1;
    e_ptr->p2=v2;
    DeleteEdge(g1, e_ptr);
    free(e_ptr);
    free(v1);
    free(v2);
    
    // Display graph g1
    ShowGraph(g1);
    
    // Find the shortest path between (0,0) and (10,6)
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=0;
    v1->y=0;
    v2->x=10;
    v2->y=6;
    ShortestPath(g1, v1, v2);
    free(v1);
    free(v2);
    
    // Find the shortest path between (0,0) and (25,5)
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=0;
    v1->y=0;
    v2->x=25;
    v2->y=5;
    ShortestPath(g1, v1, v2);
    free(v1);
    free(v2);
    
    // Find reachable vertices of (0,0)
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v1->x=0;
    v1->y=0;
    ReachableVertices(g1, v1);
    free(v1);
    
    // Find reachable vertices of (20,4)
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v1->x=20;
    v1->y=4;
    ReachableVertices(g1, v1);
    free(v1);
    
    // Free graph g1
    FreeGraph(g1);
    
    return 0;
}
