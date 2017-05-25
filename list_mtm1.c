//1

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include "list_mtm1.h"
#include <assert.h>

typedef struct Snode{
    ListElement element;
    struct Snode* next;
}Node;

struct list_t{
    int list_size;
    Node* header;
    Node* iterator;
    CopyListElement copyFunc;
    FreeListElement freeFunc;
};

static Node* createNode(CopyListElement copyElement, ListElement element);

List listCreate(CopyListElement copyElement, FreeListElement freeElement){
    if (copyElement == NULL || freeElement == NULL) return NULL;
    List list = malloc(sizeof(*list));
    if (list == NULL) return NULL;
    list->header = createNode(copyElement,NULL);
    if (list->header == NULL) {
        free(list);
        return NULL;
    }
    list->iterator = NULL;
    list->list_size = 0;
    list->copyFunc = copyElement;
    list->freeFunc = freeElement;
    return  list;
}

static Node* createNode(CopyListElement copyElement, ListElement element){
    assert(copyElement != NULL);
    Node* node = malloc(sizeof(Node));
    if (node==NULL) return NULL;
    if (element == NULL){
        node->element = NULL;
    } else {
        node->element = copyElement(element);
    }
    node->next = NULL;
    return node;
}

static void freeNode(Node* node, FreeListElement freeElement){
    assert(node!=NULL && freeElement!=NULL);
    if (node->element != NULL) {
        freeElement(node->element);
    }
    free(node);
    return;
}

void listDestroy(List list){
    if (list==NULL) return;
    for (int i=0; i < (list->list_size)+1; i++){
        if (list->header->next == NULL) {
            freeNode(list->header, list->freeFunc);
            break;
        }
        list->iterator = list->header->next;
        list->header->next = list->header->next->next;
        freeNode(list->iterator,list->freeFunc);
    }
    list->list_size=0;
    list->iterator=NULL;
    list->header=NULL;
    list->freeFunc=NULL;
    list->copyFunc=NULL;
    free(list);
    return;
}

ListResult listInsertFirst(List list, ListElement element){
    if (list==NULL) return LIST_NULL_ARGUMENT;
    Node* node = createNode(list->copyFunc,element);
    if (node == NULL) return LIST_OUT_OF_MEMORY;
    node->next = list->header->next;
    list->header->next = node;
    list->list_size++;
    return LIST_SUCCESS;
}

ListResult listInsertLast(List list, ListElement element){
    if (list==NULL) return LIST_NULL_ARGUMENT;
    Node* node = createNode(list->copyFunc,element);
    if (node == NULL) return LIST_OUT_OF_MEMORY;
    Node* tmp = list->header;
    for (int i=0 ; i < list->list_size ; i++){
        tmp = tmp->next;
    }
    tmp->next = node;
    list->list_size++;
    return LIST_SUCCESS;
}

List listCopy(List list){
    if (list == NULL) return NULL;
    List new_list = listCreate(list->copyFunc,list->freeFunc);
    if (new_list == NULL) return NULL;
    Node* tmp = list->header->next;
    for (int i=0 ; i < list->list_size; i++){
        char* word = tmp->element;
        ListResult check = listInsertLast(new_list,tmp->element);
        if (check != LIST_SUCCESS) {
            listDestroy(new_list);
            return NULL;
        }
        tmp = tmp->next;
    }
    return  new_list;
}

int listGetSize(List list){
 if (list == NULL) return  -1;
    return list->list_size;
}


List listFilter(List list, FilterListElement filterElement, ListFilterKey key){
    if (list == NULL || filterElement == NULL) return NULL;
    List new_list = listCreate(list->copyFunc,list->freeFunc);
    if (new_list == NULL) return NULL;
    Node* tmp = list->header->next;
    for (int i=0 ; i < list->list_size; i++){
        if (filterElement(tmp->element,key) == 0){
            tmp = tmp->next;
            continue;
        }
        ListResult check = listInsertLast(new_list,tmp->element);
        if (check != LIST_SUCCESS) {
            listDestroy(new_list);
            return NULL;
        }
        tmp = tmp->next;
    }
    return  new_list;
}