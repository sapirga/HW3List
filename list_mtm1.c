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
    Node* header; //a dummy first node
    Node* iterator; //the iterator of the list
    CopyListElement copyFunc;
    FreeListElement freeFunc;
};

/**
* Adds a new Node to the list.
*
* @param copyElement Function pointer to be used for copying elements into
* the list or when copying the list.
* @param element The element to insert. A copy of the element will be
* inserted as supplied by the copying function which is stored in the list
* @return
* NULL if an allocation failed (Meaning the function for copying
* an element failed)
* Node_ptr if the node has been created successfully
*/
static Node* createNode(CopyListElement copyElement, ListElement element);

/**
* Frees a Node.
*
* @param node pointer to the node that will be freed.
* @param freeElement Function pointer to be used for removing elements from
* the list.
* @return
* NULL if an allocation failed (Meaning the function for copying
* an element failed)
* Node_ptr if the node has been created successfully
*/
static void freeNode(Node* node, FreeListElement freeElement);

/**
* Inserts a new Node to the list in a specific place.
*
* @param list The list that will updated.
* @param element The element to insert. A copy of the element will be
* inserted as supplied by the copying function which is stored in the list
* @param index The specific place in the list that the node will be add
* @return
* LIST_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
* an element failed)
* LIST_SUCCESS if the node has been added successfully
*/
static ListResult insertByIndex(List list, ListElement element, int index);

/**
* Finds the place of the iterator in a list.
*
* @param list The list that will updated.
* @return
* -1 if the iterator is in invalid state
* index of the place that the iterator points to a node in the list
*/
static int indexOfIterator(List list);

/**
* Moves the iterator to a specific place in the list.
*
* @param list The relevant list.
* @param index The specific place in the list that the iterator will point to.
*/
static void moveIteratorByIndex(List list, int index);

/**
* Inserts a new Node to a sorted list.
*
* @param clone_list The sorted list that will updated.
* @param compareElement A comparison function as defined in the type
* CompareListElements. This function should return an integer indicating the
* relation between two elements in the list
* @param new_element The element that will be added to the sorted list
* @return
* LIST_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
* an element failed)
* LIST_SUCCESS if the node has been added successfully
*/
static ListResult insertNextSortElement(List clone_list, CompareListElements
            compareElement, ListElement new_element);

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
    if(list == NULL || list->iterator == NULL){
        return NULL;
    }
    list->iterator = list->iterator->next;
    if (list->iterator == NULL) return NULL;
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
        tmp = tmp->next;
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
    for(int i=0; i < listGetSize(list)+1; i++){
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
            compareElement, ListElement new_element) {
    assert((clone_list != NULL) && (new_element != NULL));
    ListElement old_element = listGetFirst(clone_list);
    while ((old_element != NULL) &&
            (compareElement(new_element, old_element) <= 0)) {
        old_element = listGetNext(clone_list);
    }
    ListResult result;
    if(old_element == NULL){
        result = listInsertLast(clone_list,new_element);
    } else {
        result = listInsertBeforeCurrent(clone_list, new_element);
    }
    return result;
}

ListResult listSort(List list, CompareListElements compareElement){
    if(list == NULL || compareElement == NULL) return LIST_NULL_ARGUMENT;
    ListResult result;
    int original_index = indexOfIterator(list);
    List clone = listCreate(list->copyFunc,list->freeFunc);
    if (clone == NULL) return LIST_OUT_OF_MEMORY;
    result = listInsertFirst(clone, listGetFirst(list));
    if (result !=LIST_SUCCESS) {
        listDestroy(clone);
        return result;
    }
    for (int i = 1; i < listGetSize(list); i++) {
        ListElement new_element = listGetNext(list);
        result = insertNextSortElement(clone,compareElement,new_element);
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