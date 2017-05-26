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
static void freeNode(Node* node, FreeListElement freeElement);
static ListResult insertByIndex(List list, ListElement element, int index);
static int indexOfIterator(List list);
static void moveIteratorByIndex(List list, int index);
static ListResult insertNextSortElement(List clone_list, CompareListElements
                        compareElement, ListElement element1);

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

static ListResult insertByIndex(List list, ListElement element, int index){
    assert(list != NULL);
    assert((index >= 0) && (index <= listGetSize(list)));
    Node* tmp = list->header;
    Node* new_node = createNode(list->copyFunc,element);
    if (new_node == NULL) return LIST_OUT_OF_MEMORY;
    for(int i=0; i < listGetSize(list)+1; i++){
        if(i == index){
            new_node->next =tmp->next;
            tmp->next = new_node;
            break;
        }
        tmp = tmp->next;
    }
    (list->list_size)++;
    return LIST_SUCCESS;
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
    int iter_index = -1;
    for (int i=0 ; i < list->list_size; i++){
        ListResult check = listInsertLast(new_list,tmp->element);
        if (check != LIST_SUCCESS) {
            listDestroy(new_list);
            return NULL;
        }
        if(tmp == list->iterator){ //search iterator index in the list
            iter_index = i;
        }
        tmp = tmp->next;
    }
    tmp = new_list->header->next;
    //initiates the new_list iterator same as original list
    if(iter_index != -1){
        for(int i=0; i < iter_index; i++) {
            tmp = tmp->next;
        }
        new_list->iterator = tmp;
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
    new_list->iterator = new_list->header->next;
    return  new_list;
}

ListElement listGetFirst(List list){
    if (!list) return NULL;
    if(listGetSize(list) == 0) return NULL;
    list->iterator = list->header->next;
    return list->iterator->element;
}

ListElement listGetNext(List list){
    if(list == NULL || list->iterator == NULL ||
            list->iterator->next == NULL) return NULL;
    list->iterator = list->iterator->next;
    return list->iterator->element;
}

ListElement listGetCurrent(List list){
    if(list == NULL || list->iterator == NULL) return NULL;
    return list->iterator->element;
}

ListResult listInsertBeforeCurrent(List list, ListElement element){
    if(list == NULL) return LIST_NULL_ARGUMENT;
    if(list->iterator == NULL) return LIST_INVALID_CURRENT;
    Node* tmp = list->header;
    int index = 0;
    ListResult reuslt;
    for(int i=0; i < listGetSize(list); i++){
        if(tmp->next == list->iterator){
            index = i;
            reuslt = insertByIndex(list,element,index);
            break;
        }
        tmp = tmp->next;
    }
    return reuslt;
}

ListResult listInsertAfterCurrent(List list, ListElement element){
    if(list == NULL) return LIST_NULL_ARGUMENT;
    if(list->iterator == NULL) return LIST_INVALID_CURRENT;
    Node* tmp = list->header;
    int index = 0;
    ListResult reuslt;
    for(int i=0; i < listGetSize(list); i++){
        if(tmp->next == list->iterator){
            index = i+1;
            reuslt = insertByIndex(list,element,index);
            break;
        }
        tmp = tmp->next;
    }
    return reuslt;
}

ListResult listRemoveCurrent(List list){
    if(list == NULL) return LIST_NULL_ARGUMENT;
    if(list->iterator == NULL) return LIST_INVALID_CURRENT;
    Node* tmp = list->header;
    for(int i=0; i < listGetSize(list); i++){
        if(tmp->next == list->iterator){
            tmp->next = list->iterator->next;
            freeNode(list->iterator,list->freeFunc);
            (list->list_size)--;
            break;
        }
    }
    list->iterator = NULL;
    return LIST_SUCCESS;
}

ListResult listClear(List list){
    if(list == NULL) return LIST_NULL_ARGUMENT;
    Node* tmp = list->header->next;
    for(int i=0; i < listGetSize(list); i++){
        list->freeFunc(tmp->element);
        tmp = tmp->next;
    }
    return LIST_SUCCESS;
}

static int indexOfIterator(List list){
    assert(!list);
    int index = -1;
    if (list->iterator == NULL) return index;
    Node* tmp = list->header;
    for(int i=0; i < listGetSize(list); i++){
        if(tmp == list->iterator){
            index = i;
            break;
        }
        tmp = tmp->next;
    }
    return index;
}

static void moveIteratorByIndex(List list, int index){
    assert((list != NULL));
    assert((list->list_size > index));
    if(index == -1){
        list->iterator = NULL;
    }
    listGetFirst(list);
    for(int i=0; i < index-1; i++) {
            listGetNext(list);
    }
}
static ListResult insertNextSortElement(List clone_list, CompareListElements
            compareElement, ListElement element1) {
    assert(clone_list->iterator != NULL);
    ListElement element2 = listGetFirst(clone_list);
    while ((element2 != NULL) && (compareElement(element1, element2) <= 0)) {
        element2 = listGetNext(clone_list);
    }
    ListResult result;
    Node *new_node = createNode(clone_list->copyFunc, element1);
    if (new_node == NULL) return LIST_OUT_OF_MEMORY;
    if(clone_list->iterator == NULL){
        result = listInsertLast(clone_list,element1);
    } else {
        result = listInsertAfterCurrent(clone_list, element1);
    }
    return result;
}

ListResult listSort(List list, CompareListElements compareElement){
    if(list == NULL || compareElement == NULL) return LIST_NULL_ARGUMENT;
    ListResult result;
    int original_index = indexOfIterator(list);//TODO what if -1?
    List clone = listCreate(list->copyFunc,list->freeFunc);
    if (clone == NULL) return LIST_OUT_OF_MEMORY;
    listInsertFirst(clone, listGetFirst(list));
    int original_size = listGetSize(list);
    for (int i = 0; i < original_size; i++) {
        ListElement element = listGetFirst(list);
        result = insertNextSortElement(clone, compareElement,element);
        if (result !=LIST_SUCCESS) {
            listDestroy(clone);
            return result;
        }
    }
    //swap the original list header with the clone list header
    Node* tmp =  list->header;
    list->header = clone->header;
    clone->header = tmp;
    listDestroy(clone);
    moveIteratorByIndex(list,original_index);
    return result;
}