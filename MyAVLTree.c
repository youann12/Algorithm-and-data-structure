//
//  main.c
//  assignment 2
//
//  Created by youan on 22/3/19.
//  Copyright © 2019 youan. All rights reserved.
//
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like

// data type for avl tree nodes
typedef struct AVLTreeNode {
    int key; //key of this item
    int value;  //value (int) of this item
    int height; //height of the subtree rooted at this node
    struct AVLTreeNode *parent; //pointer to parent
    struct AVLTreeNode *left; //pointer to left child
    struct AVLTreeNode *right; //pointer to right child
} AVLTreeNode;

//data type for AVL trees
typedef struct AVLTree{
    int  size;      // count of items in avl tree
    AVLTreeNode *root; // root
} AVLTree;

// create a new AVLTreeNode
AVLTreeNode *newAVLTreeNode(int k, int v )
{
    AVLTreeNode *new;
    new = malloc(sizeof(AVLTreeNode));
    assert(new != NULL);
    new->key = k;
    new->value = v;
    new->height = 0; // height of this new node is set to 0
    new->left = NULL; // this node has no child
    new->right = NULL;
    new->parent = NULL; // no parent
    return new;
}
// create a new empty avl tree
AVLTree *newAVLTree()
{
    AVLTree *T;
    T = malloc(sizeof (AVLTree));
    assert (T != NULL);
    T->size = 0;
    T->root = NULL;
    return T;
}
void h(AVLTreeNode *n)
{
    int ph = n->height;
    while(n->parent != NULL)
    {
        ph++;
        n = n->parent;
        if(n->right != NULL &&  n->left != NULL && ph >= n->right->height + 1 && ph >= n->left->height + 1)
        {
            n->height = ph;
        }
        else if(n->right == NULL && n->left != NULL && ph >= n->left->height + 1)
        {
            n->height = ph;
        }
        else if(n->right != NULL && ph >= n->right->height + 1 && n->left == NULL)
        {
            n->height = ph;
        }
    }
    //    int tl = n->left->height;
    //    int tr = n->right->height;
    //ph++;
    
    //n->height = ph;
}
AVLTreeNode *ck(AVLTreeNode *n)
{
    AVLTreeNode *nn = n;
    int lh,rh = -1;
    while(nn != NULL)
    {
        if(nn->right == NULL)
        {
            rh = -1;
        }
        else
        {
            rh = nn->right->height;
        }
        if(nn->left == NULL)
        {
            lh = -1;
        }
        else
        {
            lh = nn->left->height;
        }
        if(abs(rh - lh) > 1)
        {
            return nn;
        }
        nn = nn->parent;
    }
    return NULL;
}


void rr(AVLTree *newAVL, AVLTreeNode *n)
{
    if(n != newAVL->root)
    {
        AVLTreeNode *np,*nr;
        nr = n->right;
        np = n->parent;
        if(np->key > nr->key || (np->key == nr->key && np->value >nr->value))
        {
            np->left = nr;
        }
        else
        {
            np->right = nr;
        }
        n->right = nr->left;
        nr->left = n;
        nr->parent = np;
        n->parent =nr;
        if(n->right != NULL && n->left != NULL)
        {
            int r = n->right->height;
            int l = n->left->height;
            if(r > l)
            {
                n->height = r + 1;
            }
            else
            {
                n->height = l + 1;
            }
        }
        else if (n->right != NULL && n->left == NULL)
        {
            n->height = n->right->height + 1;
        }
        else if(n->left != NULL && n->right == NULL)
        {
            n->height = n->left->height + 1;
        }
        else if(n->right == NULL && n->left == NULL)
        {
            n->height = 0;
        }
        
        h(n);
    }
    else if(n == newAVL->root)
    {
        AVLTreeNode *nr;
        nr = n->right;
        newAVL->root = nr;
        nr->parent = NULL;
        n->right = nr->left;
        nr->left = n;
        n->parent = nr;
        if(n->right != NULL && n->left != NULL)
        {
            int r = n->right->height;
            int l = n->left->height;
            if(r > l)
            {
                n->height = r + 1;
            }
            else
            {
                n->height = l + 1;
            }
        }
        else if (n->right != NULL && n->left == NULL)
        {
            n->height = n->right->height + 1;
        }
        else if(n->left != NULL && n->right == NULL)
        {
            n->height = n->left->height + 1;
        }
        else if(n->right == NULL && n->left == NULL)
        {
            n->height = 0;
        }
        h(n);
    }
}
void ll(AVLTree *newAVL, AVLTreeNode *n)
{
    if(n != newAVL->root)
    {
        AVLTreeNode *np,*nl;
        nl = n->left;
        np = n->parent;
        if(np->key > nl->key || (np->key == nl->key && np->value >nl->value))
        {
            np->left = nl;
        }
        else
        {
            np->right = nl;
        }
        n->left = nl->right;
        nl->right = n;
        nl->parent = np;
        n->parent =nl;
        if(n->right != NULL && n->left != NULL)
        {
            int r = n->right->height;
            int l = n->left->height;
            if(r > l)
            {
                n->height = r + 1;
            }
            else
            {
                n->height = l + 1;
            }
        }
        else if (n->right != NULL && n->left == NULL)
        {
            n->height = n->right->height + 1;
        }
        else if(n->left != NULL && n->right == NULL)
        {
            n->height = n->left->height + 1;
        }
        else if(n->right == NULL && n->left == NULL)
        {
            n->height = 0;
        }
        h(n);
    }
    else if(n == newAVL->root)
    {
        AVLTreeNode *nl;
        nl = n->left;
        newAVL->root = nl;
        n->left = nl->right;
        nl->right = n;
        n->parent = nl;
        nl->parent = NULL;
        if(n->right != NULL && n->left != NULL)
        {
            int r = n->right->height;
            int l = n->left->height;
            if(r > l)
            {
                n->height = r + 1;
            }
            else
            {
                n->height = l + 1;
            }
        }
        else if (n->right != NULL && n->left == NULL)
        {
            n->height = n->right->height + 1;
        }
        else if(n->left != NULL && n->right == NULL)
        {
            n->height = n->left->height + 1;
        }
        else if(n->right == NULL && n->left == NULL)
        {
            n->height = 0;
        }
        h(n);
    }
}
void lr(AVLTree *newAVL, AVLTreeNode *n)
{
    rr(newAVL,n->left);
    ll(newAVL,n);
}

void rl(AVLTree *newAVL, AVLTreeNode *n)
{
    ll(newAVL,n->right);
    rr(newAVL,n);
}
// at most go to the deepest of the tree which is log n so time complexity is O(log n)
AVLTreeNode *Search(AVLTree *T, int k, int v)
{
    AVLTreeNode *n = T->root;
    while(n->key != k || n->value != v)
    {
        if(k > n->key || (k == n->key && v > n->value))
        {
            if(n->right != NULL)
            {
                n = n->right;
            }
            else
            {
                return NULL;
            }
        }
        else if(k < n->key || (k == n->key && v < n->value))
        {
            if(n->left != NULL)
            {
                n = n->left;
            }
            else
            {
                return NULL;
            }
        }
    }
    //printf("%d\n%d\n",n->key,n->value);
    return n;
}
int loc(AVLTreeNode *s, AVLTreeNode *i)
{
    AVLTreeNode *p = s;
    int a = 0;
    if(i->key > s->key || (i->key == s->key && i->value > s->value))
    {
        p = s->right;
        a += 10;
    }
    else
    {
        p = s->left;
        a += 20;
    }
    if(i->key > p->key || (i->key == p->key && i->value > p->value))
    {
        p = p->right;
        a += 1;
    }
    else
    {
        p = p->left;
        a += 2;
    }
    return a;
}
void r(AVLTree *T, AVLTreeNode *n)
{
    AVLTreeNode *ckn = ck(n);
    if(ckn != NULL)
    {
        int a = loc(ckn,n);
        if(a == 11)
        {
            rr(T,ckn);
        }
        else if(a == 22)
        {
            ll(T,ckn);
        }
        else if(a == 12)
        {
            rl(T,ckn);
        }
        else if(a == 21)
        {
            lr(T,ckn);
        }
        
    }
    //    T->size++;
    h(n);
}


// the worst case is that visit all height of the tree so time complexity is O(log(n))
int InsertNode(AVLTree *T, int k, int v)
{
    AVLTreeNode *newnode = newAVLTreeNode(k,v);
    if (T->root == NULL)
    {
        T->root = newnode;
        T->size++;
    }
    else
    {
        AVLTreeNode *mid,*p,*x;
        x = Search(T,k,v);
        if(x == NULL)
        {
            mid = T->root;
            while(mid != NULL)
            {
                p = mid;
                if(k > mid->key || (k == mid->key && v > mid->value))
                {
                    mid = mid->right;
                    if(mid == NULL)
                    {
                        p->right = newnode;
                        newnode->parent = p;
                        h(newnode);
                        r(T,newnode);
                        T->size++;
                        return 1;
                    }
                }
                else if(k < mid->key || (k == mid->key && v < mid->value))
                {
                    mid = mid->left;
                    if(mid == NULL)
                    {
                        p->left = newnode;
                        newnode->parent = p;
                        h(newnode);
                        r(T,newnode);
                        T->size++;
                        return 1;
                    }
                }
            }
        }
        
    }
    return 0;
    //put your code here
}

// for n node, insert, check if it already in the tree (O(log(n))), then insert the node(O(log(n)) and if it unbalance, rotate it O(1), So total complexity is O(nlog(n))
AVLTree *CreateAVLTree(const char *filename)
{
    FILE *fp = NULL;
    int k;
    int v;
    //    int rh,lh = -1;
    AVLTree *newAVL = newAVLTree();
    AVLTreeNode *newnode;
    if (strcmp(filename,"stdin") != 0)
    {
        fp = fopen(filename,"r");
        if (fp == NULL)
        {
            perror("Error while opening the file.\n");
            exit(EXIT_FAILURE);
        }
        while(fscanf(fp," %*c%d%*c%d%*c ",&k, &v) != EOF)
        {
            newnode = newAVLTreeNode(k,v);
            InsertNode(newAVL, k, v);
            //                printf("key===%d,height===%d\n",newAVL->root->key,newAVL->root->height);
            
        }
        fclose(fp);
    }
    else
    {
        int k,v;
        char buffer[255];
        printf("please enter the node\n");
        while (fgets(buffer, sizeof(buffer), stdin) != NULL){
            int len = (int)strlen(buffer);
            int s = 0;
            int fk = 0;
            int sk = 0;
            if(buffer[0]=='\n'||buffer[0]=='\0')
            {
                break;
            }
            for (int i = 0; i < len; ++i)
            {
                if(buffer[i] == '(' && s < 1)
                {
                    fk = i;
                    s++;
                }
                else if((buffer[i] == '(' || i == len - 1) && s == 1)
                {
                    char the_p[255];
                    sk = i;
                    strncpy(the_p,buffer + fk, sk - fk);
                    sscanf(the_p, "%*[^0-9|-]%i%*[^0-9|-]%i",&k,&v);
                    InsertNode(newAVL, k, v);
                    fk = i;
                    s = 1;
                }
                
            }
            
        }
        
    }
    //printf("%d\n%d\n", newAVL->root->left->right->left->key,newAVL->root->left->right->key);
    //printf("%d\n", newAVL->root->right->height - newAVL->root->left->height);
    
    return newAVL;
}
AVLTreeNode *clone(AVLTreeNode *n)
{
    
    if(n == NULL)
    {
        return n;
    }
    else
    {
        AVLTreeNode *mid = newAVLTreeNode(n->key,n->value);
        mid->height = n->height;
        mid->left = clone(n->left);
        mid->right = clone(n->right);
        if(mid->left != NULL)
        {
            mid->left->parent = mid;
        }
        if(mid->right != NULL)
        {
            mid->right->parent = mid;
        }
        //mid->parent = clone(n->parent);
        return mid;
    }
}
//visit each node once so time complexity is O(n)
AVLTree *CloneAVLTree(AVLTree *T)
{
    AVLTree *nT = newAVLTree();
    nT->root = newAVLTreeNode(T->root->key, T->root->value);
    nT->size = T->size;
    nT->root->height = T->root->height;
    nT->root->right = clone(T->root->right);
    nT->root->left = clone(T->root->left);
    nT->root->right->parent = nT->root;
    nT->root->left->parent = nT->root;
    // put your code here
    return nT;
}
//
// put your time complexity for ALVTreesUNion() here
void lorder(AVLTree *T, AVLTree *T2, AVLTreeNode *n)
{
    if(n->left != NULL)
    {
        int k = n->key;
        int v = n->value;
        lorder(T, T2, n->left);
        n = Search(T, k, v);
    }
    
    AVLTreeNode *awm = Search(T2, n->key, n->value);
    if(awm == NULL)
    {
        //printf("%d\n",n->key);
        InsertNode(T2,n->key, n->value);
    }
    if(n->right != NULL)
    {
        lorder(T,T2,n->right);
    }
}
//visit each node in when search each node in T1, also visit each node in T2 so time complexity is O((m+n)log(m+n))
AVLTree *AVLTreesUnion(AVLTree *T1, AVLTree *T2)
{
    AVLTree *tree = CloneAVLTree(T2);
    lorder(T1,tree,T1->root);
    return tree;
}
//
// put your time complexity for ALVTreesIntersection() here
void lorder2(AVLTree *T, AVLTree *T2, AVLTree *T3, AVLTreeNode *n)
{
    if(n->left != NULL)
    {
        int k = n->key;
        int v = n->value;
        lorder2(T, T2, T3, n->left);
        n = Search(T, k, v);
    }
    
    AVLTreeNode *awm = Search(T2, n->key, n->value);
    if(awm != NULL)
    {
        //printf("%d\n",n->key);
        InsertNode(T3,n->key, n->value);
    }
    if(n->right != NULL)
    {
        lorder2(T,T2,T3,n->right);
    }
}
//visit each node in when search each node in T1, also visit each node in T2 and add node in the new tree so time complexity is O((m+n+k)log(m+n+k))
AVLTree *AVLTreesIntersection(AVLTree *T1, AVLTree *T2)
{
    AVLTree *nT = newAVLTree();
    lorder2(T1,T2,nT,T1->root);
    return nT;
}


AVLTreeNode *ld(AVLTreeNode *n)
{
    while(n->left != NULL)
    {
        n = n->left;
    }
    return n;
}

AVLTreeNode *rd(AVLTreeNode *n)
{
    while(n->right != NULL)
    {
        n = n->right;
    }
    return n;
}

int balance(AVLTreeNode *n)
{
    int a = 0;
    if(n->right != NULL && n->left != NULL)\
    {
        a = n->left->height - n->right->height;
    }
    else if(n->right != NULL && n->left == NULL)
    {
        a = -n->right->height + 1;
    }
    else if(n->right == NULL && n->left != NULL)
    {
        a = n->left->height + 1;
    }
    return a;
}
//
// search the node cause at most O(log n) time and route cause O(1) time so time complesity is O(logn)
int DeleteNode(AVLTree *T, int k, int v)
{
    AVLTreeNode *n = Search(T, k, v);
    if(n == NULL)
    {
        return 0;
    }
    else
    {
        if(n->right == NULL && n->left == NULL)
        {
            if(n != T->root)
            {
                n->height--;
                h(n);
                AVLTreeNode *ckn = ck(n);
                if(k > n->parent->key || (k == n->parent->key && v > n->parent->value))
                {
                    n->parent->right = NULL;
                }
                else
                {
                    n->parent->left = NULL;
                }
                if(ckn != NULL)
                {
                    int bal = balance(ckn);
                    if((k > ckn->key || (k == ckn->key && v > ckn->value)) && bal == 2 && (balance(ckn->left) == 0 || balance(ckn->left) == 1 ))
                    {
                        ll(T,ckn);
                    }
                    else if((k > ckn->key || (k == ckn->key && v > ckn->value)) && bal == 2 && balance(ckn->left) == -1)
                    {
                        lr(T,ckn);
                    }
                    else if((k < ckn->key || (k == ckn->key && v < ckn->value)) && bal == -2 && (balance(ckn->right) == 0 || balance(ckn->right) == 1 ))
                    {
                        rr(T,ckn);
                    }
                    else if((k < ckn->key || (k == ckn->key && v < ckn->value)) && bal == -2 && balance(ckn->right) == -1)
                    {
                        rl(T,ckn);
                        
                    }
                }
                T->size--;
            }
            else
            {
                n = NULL;
                T->size--;
            }
        }
        else if((n->right != NULL && n->left == NULL) || (n->right == NULL && n->left != NULL))
        {
            //           printf("xxx\n");
            AVLTreeNode *c;
            if(n->right != NULL && n->left == NULL)
            {
                c = n->right;
            }
            else
            {
                c = n->left;
            }
            if(n != T->root)
            {
                AVLTreeNode *p = n->parent;
                if(n->key > p->key || (n->key == p->key && n->value > p->value))
                {
                    c->parent = p;
                    p->right = c;
                    n = NULL;
                    T->size--;
                    h(c);
                    AVLTreeNode *ckn = ck(c);
                    //                    if(k > n->parent->key || (k == n->parent->key && v > n->parent->value))
                    //                    {
                    //                        n->parent->right = NULL;
                    //                    }
                    //                    else
                    //                    {
                    //                        n->parent->left = NULL;
                    //                    }
                    if(ckn != NULL)
                    {
                        int bal = balance(ckn);
                        if((k > ckn->key || (k == ckn->key && v > ckn->value)) && bal == 2 && (balance(ckn->left) == 0 || balance(ckn->left) == 1 ))
                        {
                            ll(T,ckn);
                        }
                        else if((k > ckn->key || (k == ckn->key && v > ckn->value)) && bal == 2 && balance(ckn->left) == -1)
                        {
                            lr(T,ckn);
                        }
                        else if((k < ckn->key || (k == ckn->key && v < ckn->value)) && bal == 2 && (balance(ckn->right) == 0 || balance(ckn->right) == 1 ))
                        {
                            rr(T,ckn);
                        }
                        else if((k < ckn->key || (k == ckn->key && v < ckn->value)) && bal == 2 && balance(ckn->right) == -1)
                        {
                            rl(T,ckn);
                        }
                    }
                    
                }
                else
                {
                    c->parent = p;
                    p->left = c;
                    h(c);
                    n = NULL;
                    AVLTreeNode *ckn = ck(c);
                    if(k > n->parent->key || (k == n->parent->key && v > n->parent->value))
                    {
                        n->parent->right = NULL;
                    }
                    else
                    {
                        n->parent->left = NULL;
                    }
                    if(ckn != NULL)
                    {
                        int bal = balance(ckn);
                        if((k > ckn->key || (k == ckn->key && v > ckn->value)) && bal == 2 && (balance(ckn->left) == 0 || balance(ckn->left) == 1 ))
                        {
                            ll(T,ckn);
                        }
                        else if((k > ckn->key || (k == ckn->key && v > ckn->value)) && bal == 2 && balance(ckn->left) == -1)
                        {
                            lr(T,ckn);
                        }
                        else if((k < ckn->key || (k == ckn->key && v < ckn->value)) && bal == 2 && (balance(ckn->right) == 0 || balance(ckn->right) == 1 ))
                        {
                            rr(T,ckn);
                        }
                        else if((k < ckn->key || (k == ckn->key && v < ckn->value)) && bal == 2 && balance(ckn->right) == -1)
                        {
                            rl(T,ckn);
                        }
                    }
                    T->size--;
                }
            }
            else
            {
                T->root = c;
                c->parent = NULL;
                n = NULL;
                T->size--;
            }
        }
        else if(n->left != NULL && n->right != NULL)//with two children
        {
            int hr = n->right->height;
            int hl = n->left->height;
            AVLTreeNode *rs,*mid;
            if(hr > hl)
            {
                rs = ld(n->right);
            }
            else
            {
                rs = rd(n->left);
            }
            if(n != T->root)
            {
                if(hr < hl)
                {
                    n->key = rs->key;
                    n->value = rs->value;
                    mid = rs->parent;
                    AVLTreeNode *rsr = rs->left;
                    if(rsr != NULL)
                    {
                        rsr->parent = rs->parent;
                    }
                    if(rs->parent == n)
                    {
                        n->left = rsr;
                    }
                    else
                    {
                        rs->parent->right = rsr;
                    }
                    int a = 0, b = 0;
                    if(mid->left != NULL)
                    {
                        a = mid->left->height;
                    }
                    if(mid->right != NULL)
                    {
                        b = mid->right->height;
                    }
                    if(a > b)
                    {
                        mid->height = a + 1;
                    }
                    else
                    {
                        mid->height = b + 1;
                    }
                    h(mid);
                    rs= NULL;
                    T->size--;
                }
                else
                {
                    n->key = rs->key;
                    n->value = rs->value;
                    mid = rs->parent;
                    AVLTreeNode *rsr = rs->right;
                    if(rsr != NULL)
                    {
                        rsr->parent = rs->parent;
                    }
                    if(rs->parent == n)
                    {
                        n->right = rsr;
                    }
                    else
                    {
                        rs->parent->left = rsr;
                    }
                    int a = 0, b = 0;
                    if(mid->left != NULL)
                    {
                        a = mid->left->height;
                    }
                    if(mid->right != NULL)
                    {
                        b = mid->right->height;
                    }
                    if(a > b)
                    {
                        mid->height = a + 1;
                    }
                    else
                    {
                        mid->height = b + 1;
                    }
                    h(mid);
                    rs = NULL;
                    T->size--;
                }
            }
            else
            {
                if(hr < hl)
                {
                    n->key = rs->key;
                    n->value = rs->value;
                    mid = rs->parent;
                    AVLTreeNode *rsr = rs->left;
                    if(rsr != NULL)
                    {
                        rsr->parent = rs->parent;
                    }
                    if(rs->parent == n)
                    {
                        n->left = rsr;
                    }
                    else
                    {
                        rs->parent->right = rsr;
                    }
                    int a = 0, b = 0;
                    if(mid->left != NULL)
                    {
                        a = mid->left->height;
                    }
                    if(mid->right != NULL)
                    {
                        b = mid->right->height;
                    }
                    if(a > b)
                    {
                        mid->height = a + 1;
                    }
                    else
                    {
                        mid->height = b + 1;
                    }
                    h(mid);
                    rs= NULL;
                    T->size--;
                }
                else
                {
                    n->key = rs->key;
                    n->value = rs->value;
                    mid = rs->parent;
                    AVLTreeNode *rsr = rs->right;
                    if(rsr != NULL)
                    {
                        rsr->parent = rs->parent;
                    }
                    if(rs->parent == n)
                    {
                        n->right = rsr;
                    }
                    else
                    {
                        rs->parent->left = rsr;
                    }
                    int a = 0, b = 0;
                    if(mid->left != NULL)
                    {
                        a = mid->left->height;
                    }
                    if(mid->right != NULL)
                    {
                        b = mid->right->height;
                    }
                    if(a > b)
                    {
                        mid->height = a + 1;
                    }
                    else
                    {
                        mid->height = b + 1;
                    }
                    h(mid);
                    rs = NULL;
                    T->size--;
                }
            }
        }
        return 1;
    }
}



void freet(AVLTreeNode *n)
{
    if(n == NULL)
    {
        return;
    }
    freet(n->left);
    freet(n->right);
    free(n);
    // printf("%d, %d\n", n->key,n->value);
}
// Free all node so it is O(n)
void FreeAVLTree(AVLTree *T)
{
    freet(T->root);
    //    free(T->size);
    //   free(T->root);
    free(T);
}
//

void inorder(AVLTree *T, AVLTreeNode *n)
{
    if(n->left != NULL)
    {
        int k = n->key;
        int v = n->value;
        inorder(T,n->left);
        n = Search(T, k, v);
    }
    printf("(%d, %d), %d\n", n->key,n->value,n->height);
    if(n->right != NULL)
    {
        inorder(T,n->right);
    }
}
// visit each node once so time complexity is O(n)
void PrintAVLTree(AVLTree *T)
{
    AVLTreeNode *n = T->root;
    inorder(T,n);
}

int main() //sample main for testing
{ int i,j;
    AVLTree *tree1, *tree2, *tree3, *tree4, *tree5, *tree6, *tree7, *tree8;
    AVLTreeNode *node1;
    
    tree1=CreateAVLTree("stdin");
    PrintAVLTree(tree1);
    FreeAVLTree(tree1);
    //you need to create the text file file1.txt
    // to store a set of items without duplicate items
    tree2=CreateAVLTree("file1.txt");
    PrintAVLTree(tree2);
    tree3=CloneAVLTree(tree2);
    PrintAVLTree(tree3);
    FreeAVLTree(tree2);
    FreeAVLTree(tree3);
    //    //Create tree4
    tree4=newAVLTree();
    j=InsertNode(tree4, 10, 10);
    for (i=0; i<15; i++)
    {
        j=InsertNode(tree4, i, i);
        if (j==0) printf("(%d, %d) already exists\n", i, i);
    }
    PrintAVLTree(tree4);
    node1=Search(tree4,20,20);
    if (node1!=NULL)
        printf("key= %d value= %d\n",node1->key,node1->value);
    else
        printf("Key 20 does not exist\n");
    
    for (i=17; i>0; i--)
    {
        j=DeleteNode(tree4, i, i);
        if (j==0)
            printf("Key %d does not exist\n",i);
        PrintAVLTree(tree4);
    }
    FreeAVLTree(tree4);
    //Create tree5
    tree5=newAVLTree();
    j=InsertNode(tree5, 6, 25);
    j=InsertNode(tree5, 6, 10);
    j=InsertNode(tree5, 6, 12);
    j=InsertNode(tree5, 6, 20);
    j=InsertNode(tree5, 9, 25);
    j=InsertNode(tree5, 10, 25);
    PrintAVLTree(tree5);
    //Create tree6
    tree6=newAVLTree();
    j=InsertNode(tree6, 6, 25);
    j=InsertNode(tree6, 5, 10);
    j=InsertNode(tree6, 6, 12);
    j=InsertNode(tree6, 6, 20);
    j=InsertNode(tree6, 8, 35);
    j=InsertNode(tree6, 10, 25);
    PrintAVLTree(tree6);
    tree7=AVLTreesIntersection(tree5, tree6);
    tree8=AVLTreesUnion(tree5,tree6);
    PrintAVLTree(tree7);
    PrintAVLTree(tree8);
    return 0;
}
