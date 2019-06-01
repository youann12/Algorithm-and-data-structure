//
//  MyDLList.c
//  assignment_1
//
//  Created by youan on 5/3/19.
//  Copyright © 2019 z5193744. All rights reserved.
//
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like

// data structures representing DLList

// data type for nodes
typedef struct DLListNode {
    int  value;  // value (int) of this list item
    struct DLListNode *prev;
    // pointer previous node in list
    struct DLListNode *next;
    // pointer to next node in list
} DLListNode;

//data type for doubly linked lists
typedef struct DLList{
    int  size;      // count of items in list
    DLListNode *first; // first node in list
    DLListNode *last;  // last node in list
} DLList;

// create a new DLListNode
DLListNode *newDLListNode(int it)// the time complexity of this function is O(n)
{
    DLListNode *new;
    new = malloc(sizeof(DLListNode));
    assert(new != NULL);
    new->value = it;
    new->prev = new->next = NULL;
    return new;
}

// create a new empty DLList
DLList *newDLList()// the time complexity of this function is O(n)
{
    DLList *L;
    
    L = malloc(sizeof (struct DLList));
    assert (L != NULL);
    L->size = 0;
    L->first = NULL;
    L->last = NULL;
    return L;
}

// create a DLList from a text file
// put your time complexity analysis for CreateDLListFromFileDlist() here
// DLList *CreateDLListFromFileDlist(const char *filename)
DLList *CreateDLListFromFileDlist(const char *filename)
// the time complexity of this function is O(n), n is the number of integer in the file or stdin, used one loop
{
    FILE *fp = NULL;
    int value;
    DLList *newDLL = newDLList();
    DLListNode *newnode;
    char buffer[255];
    if (strcmp(filename,"stdin") != 0)
    {
        fp = fopen(filename,"r");
        if (fp == NULL)
        {
            perror("Error while opening the file.\n");
            exit(EXIT_FAILURE);
        }
        DLListNode *p = NULL; //the temp node of prev
        while (fscanf(fp,"%d",&value) != EOF) {
            newnode = newDLListNode(value);
            newDLL->size++;
            if (newDLL->first == NULL && p == NULL)
            {
                newDLL->first = newnode;
            }
            else
            {
                newnode->prev = p;
                p->next = newnode;
            }
            p = newnode;
            newDLL->last = newnode;
            //printf("value: %d\n", value);
            }
        fclose(fp);
    }
    else
    {
//        while(value != '/0')
//        {
//            printf("%d",value);
//            scanf("%d",&value);
//        }
        DLListNode *p = NULL;
        while (fgets(buffer, sizeof(buffer), stdin) != NULL)// check whether the enter is empty line, and store in buffer zone
        {

            if (sscanf(buffer, "%d", &value) == 1)// chech the integer in the buffer
            {
                //printf("You enter: %d\n", value);
                newnode = newDLListNode(value);
                newDLL->size++;
                if (newDLL->first == NULL && p == NULL)
                {
                    newDLL->first = newnode;
                }
                else
                {
                    newnode->prev = p;
                    p->next = newnode;
                }
                p = newnode;
                newDLL->last = newnode;
                
            }
            else
            {
                break;
            }
        }

    }
    return newDLL;
    
    // put your code here
    
}

// clone a DLList
// put your time complexity analysis for cloneList() here
DLList *cloneList(DLList *u)// the time complexity of this function is O(n), n is the length of the DLL input
{
    DLList *newDLL = newDLList(); // the clone DLL
    DLListNode *temp = newDLListNode(u->first->value);//the temp node use to connect the DLL
    DLListNode *new;
    DLListNode *old = u->first->next; // the temp node to store the node of old DLL
    for(int a = 0; a < u->size; a++)
    {
        if(newDLL->first == NULL)
        {
            newDLL->first = temp;
        }
        else if(newDLL->first != NULL)
        {
            temp->next = newDLListNode(old->value);
            new = temp;
            temp = temp->next;
            temp->prev = new;
            old = old->next;
        }
    }
    newDLL->last = temp;
    newDLL->size = u->size;
    return newDLL;
}

//// compute the union of two DLLists u and v
DLList *setUnion(DLList *u, DLList *v)// the time complexity of this function is O(n*m), use two for loop, and n is the size of u, m is the length of v
{
    DLList *tempu = cloneList(u); //the clone DLL of u
    DLList *tempv = cloneList(v); //the clone DLL of v
    DLList *newDLL = newDLList(); // the union DLL
    DLListNode *temp = newDLListNode(u->first->value);//the temp node use to connect the DLL
    DLListNode *new = temp;
    DLListNode *old = tempu->first;
    int t = 0;//count the times that the value appeared in connected DLL
    tempu->last->next = tempv->first;
    tempv->first->prev = tempu->last;//connect the u and c to made up a new list
    newDLL->first = temp;
    newDLL->size = 1;
    for(int a = 0; a < tempv->size + tempu->size; a++)
    {
        DLListNode *ck = newDLL->first;
        for (int b = 0; b < newDLL->size; b++)// if the value already in the newDLL
        {
            if(ck->value == old->value)
            {
                t++;//if so, record the times
            }
            if (ck->next != NULL)
            {
                ck = ck->next;
            }
        }
        if(t == 0)
        {
            temp = newDLListNode(old->value);
            new->next = temp;
            temp->prev = new;
            new = temp;
            newDLL->size ++;
            old = old->next;
        }
        else
        {
            old = old->next;
            t = 0;
        }
    }
    return newDLL;
}
//
//// compute the insection of two DLLists u and v
//// put your time complexity analysis for intersection() here
DLList *setIntersection(DLList *u, DLList *v)// the time complexity of this function is O(n*m), use two for loop, and n is the size of u, m is the length of v
{
    DLListNode *pu = u->first;
    DLListNode *pv = v ->first;
    DLListNode *temp = newDLListNode(u->first->value);
    DLListNode *new;
    DLList *newDLL = newDLList(); // the Intersection DLL
    for (int a = 0; a < u->size; a++)
    {
        for (int b = 0; b < v->size; b++)
        {
            if(pv->value == pu->value)
            {
                if(newDLL->first == NULL)
                {
                    temp = newDLListNode(pu->value);
                    newDLL->first = temp;
                    pv = pv->next;
                    newDLL->size ++;
                    //printf("%d\n", pu->value);
                }
                else if(temp != NULL)
                {
                    new = newDLListNode(pu->value);
                    temp->next = new;
                    new->prev = temp;
                    temp = new;
                    pv = pv->next;
                    newDLL->size ++;
                    newDLL->last = new;
                    //printf("%d\n", pu->value);
                }
            }
            else
            {
                pv = pv->next;
            }
        }
        pv = v ->first;
        pu = pu->next;
    }
    return newDLL;
}
//
//// free up all space associated with list
//// put your time complexity analysis for freeDLList() here
void freeDLList(DLList *L)// the time complexity of this function is O(n), n is the size of input DLL, used one for loop
{
    DLListNode *temp = L->first;
    DLListNode *new;
    for (int a = 0; a < L->size; a++)
    {
        new = temp->next;
        free(temp);
        temp = new;
    }
    free(L);
}
//
//
//// display items of a DLList
//// put your time complexity analysis for printDDList() here
void printDLList(DLList *u)// the time complexity of this function is O(n), n is the size of input DLL, used one for loop
{
    DLListNode *temp = u->first;
    for(int a = 0; a < u->size; a++)
 {
     if(temp != NULL)
     {
         printf("%d\n",temp->value);
         temp = temp->next;
         
     }
 }
//    printf("%d\n",u->last->value);
//    printf("%d\n",u->last->prev->value);
//    printf("%d\n",u->last->prev->prev->value);
}

int main()
{
    DLList *list1, *list2, *list3, *list4;

    CreateDLListFromFileDlist("File1.txt");

    list1 = CreateDLListFromFileDlist("File1.txt");
    printDLList(list1);
    //
    list2=CreateDLListFromFileDlist("File2.txt");
    printDLList(list2);
    //
    list3=setUnion(list1, list2);
    printDLList(list3);
    //
    list4=setIntersection(list1, list2);
    printDLList(list4);
    //
    freeDLList(list1);
    freeDLList(list2);
    freeDLList(list3);
    freeDLList(list4);
    //
    printf("please type all the integers of list1\n");
    list1=CreateDLListFromFileDlist("stdin");
    //
    printf("please type all the integers of list2\n");
    list2=CreateDLListFromFileDlist("stdin");
    //
    list3=cloneList(list1);
    printDLList(list3);
    list4=cloneList(list2);
    printDLList(list4);
    //
     freeDLList(list1);
     freeDLList(list2);
     freeDLList(list3);
     freeDLList(list4);

    return 0;
}

