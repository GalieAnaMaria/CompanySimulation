#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hierarchy.h"

typedef struct node{
    TreeNode* node;
    struct node* next;

}Node, *List;

List createList(){
    
    List list = NULL;
    list = (List)calloc(1, sizeof(Node));

    return list;
}

Node* createNode(TreeNode *node){
    Node *listNode = (Node*)calloc(1, sizeof(Node));

    listNode->node = node;
    listNode->next = NULL;
    

    return listNode;
}

void insertNode(List head, TreeNode* node){

    if(head->node == NULL){

        head->node= node;
        return;
    }

    Node* newNode = createNode(node); 

    Node* temp = head;

    while(temp->next != NULL){
        temp = temp->next;
    }

    temp->next = newNode;
}

void constructList(List listEmployee, TreeNode* currentManager){

    if(listEmployee == NULL)return;
    List head = listEmployee;
    if(currentManager == NULL){
       return;
    }
    //printf("Nod current %s\n",currentManager->name);

    insertNode(head, currentManager);
    
    int employeesNr = currentManager->direct_employees_no;

    for(int i = 0; i < employeesNr; i++)
        constructList(head, currentManager->team[i]);   
     
}

void freeList(List list){
    
    List current;

    if(list == NULL)return;

    if(list->next == NULL){
        free(list);
        return;
    }

    while(list->next != NULL){
        current = list;
        list = list->next; 
        free(current); 
    }
    
    current = list;
    free(current);
}


#endif