#include "test_utilities.h"
#include "list.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>



static ListElement copyString(ListElement str){
	assert(str);
	char* copy = malloc(strlen(str)+1);
	return copy != NULL ? strcpy(copy, str) : NULL;
}

static void freeString(ListElement str){
	free(str);
}

static bool isLongerThan(ListElement element,ListFilterKey number) {
	char* string = element;
    return strlen(string) > *(int*)number;
}

static int compareString(ListElement str1, ListElement str2){
    return strcmp(str1, str2);
}

static bool testListCreate() {
	ASSERT_TEST(listCreate(NULL,NULL) == NULL);
	ASSERT_TEST(listCreate(NULL,NULL) == NULL);
	ASSERT_TEST(listCreate(copyString,NULL) == NULL);
	ASSERT_TEST(listCreate(NULL,freeString) == NULL);
	ASSERT_TEST(listCreate(NULL,freeString) == NULL);
	List list1 = listCreate(copyString, freeString);
	ASSERT_TEST(list1 != NULL);
    listDestroy(list1);
	return true;
}
static bool testListFilter() {
	char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
	List list = listCreate(copyString,freeString);
	for (int i=0;i <5; ++i){
		listInsertFirst(list,a[i]);
	}
	int key = 5;
    List filtered = listFilter(NULL,isLongerThan, &key);
    ASSERT_TEST(filtered == NULL);
    ASSERT_TEST(listFilter(list,NULL, &key) == NULL);
    filtered = listFilter(list,isLongerThan, &key);
	ASSERT_TEST(listGetSize(filtered) == 1);
	ASSERT_TEST(strcmp(listGetFirst(filtered),a[3])==0);
	listDestroy(list);
	listDestroy(filtered);
	return true;
}
static bool testListCopy() {
    char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
    List list = listCreate(copyString,freeString);
    for (int i=0;i <5; ++i) {
        listInsertLast(list, a[i]);
    }
    List copied = listCopy(NULL);
    ASSERT_TEST(copied == NULL);
    copied = listCopy(list);
    ASSERT_TEST(listGetSize(copied) == 5);
    ASSERT_TEST(strcmp(listGetFirst(copied),a[0])==0);
    listDestroy(list);
    listDestroy(copied);
	return true;
}

static bool testListGetSize() {
    char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
    List list = listCreate(copyString,freeString);
    ASSERT_TEST(listGetSize(NULL) == -1);
    ASSERT_TEST(listGetSize(list) == 0);
    for (int i=0;i <5; ++i){
		listInsertFirst(list,a[i]);
        ASSERT_TEST(listGetSize(list) == i+1);
	}
    listDestroy(list);
	return true;
}

static bool testListGetFirst() {
    char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
    List list = listCreate(copyString,freeString);
    ASSERT_TEST(listGetFirst(list) == NULL);
    for  (int i=0;i <5; ++i){
        listInsertFirst(list,a[i]);
        ASSERT_TEST(strcmp(listGetFirst(list),a[i]) == 0);
     }
    listDestroy(list);
    return true;
}

static bool testListGetNext() {
    char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
    List list = listCreate(copyString,freeString);
    ASSERT_TEST(listGetNext(NULL) == NULL);
    ASSERT_TEST(listGetNext(list) == NULL);
    for  (int i=0;i <5; ++i){
        listInsertLast(list,a[i]);
    }
    ASSERT_TEST(listGetNext(list) == NULL);
    ASSERT_TEST(strcmp(listGetFirst(list),a[0]) == 0);
    for  (int i=1;i <5; ++i){
        ASSERT_TEST(strcmp(listGetNext(list),a[i]) == 0);
    }
    ASSERT_TEST(listGetNext(list) == NULL);
    listDestroy(list);
    return true;
}

static bool testListInsertFirst() {
    char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
    List list = listCreate(copyString,freeString);
    ASSERT_TEST(listGetSize(list) == 0);
    ASSERT_TEST(listInsertFirst(NULL,a[0]) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertFirst(list,NULL) == LIST_SUCCESS);
    for (int i=0;i <5; ++i){
        ASSERT_TEST(listInsertFirst(list,a[i]) == LIST_SUCCESS);
        ASSERT_TEST(listGetSize(list) == i+2);
        ASSERT_TEST(strcmp(listGetFirst(list),a[i]) == 0);
    }
    listDestroy(list);
    return true;
}

static bool testListInsertLast() {
    char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
    List list = listCreate(copyString,freeString);
    ASSERT_TEST(listGetSize(list) == 0);
    ASSERT_TEST(listInsertLast(NULL,a[0]) == LIST_NULL_ARGUMENT);
    for (int i=0;i <5; ++i){
        ASSERT_TEST(listInsertLast(list,a[i]) == LIST_SUCCESS);
        ASSERT_TEST(listGetSize(list) == i+1);
        ASSERT_TEST(strcmp(listGetFirst(list),a[0]) == 0);
    }
    ASSERT_TEST(listInsertLast(list,NULL) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 6);
    listDestroy(list);
	return true;
}

static bool testListInsertBeforeCurrent() {
    char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
    List list = listCreate(copyString,freeString);
    listInsertFirst(list,a[0]);
    ASSERT_TEST(listInsertBeforeCurrent(NULL, a[1]) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertBeforeCurrent(list, a[1]) == LIST_INVALID_CURRENT);
    listGetFirst(list);
    ASSERT_TEST(listInsertBeforeCurrent(list, a[1]) == LIST_SUCCESS);
    ASSERT_TEST(strcmp(listGetFirst(list),a[1]) == 0);
    ASSERT_TEST(listGetSize(list) == 2);
    listInsertFirst(list,a[2]);
    ASSERT_TEST(listInsertBeforeCurrent(list, a[3]) == LIST_SUCCESS);
    ASSERT_TEST(strcmp(listGetFirst(list),a[2]) == 0);
    ASSERT_TEST(strcmp(listGetNext(list),a[3]) == 0);
    ASSERT_TEST(strcmp(listGetNext(list),a[1]) == 0);
    ASSERT_TEST(strcmp(listGetNext(list),a[0]) == 0);
    ASSERT_TEST(listGetSize(list) == 4);
    listDestroy(list);
	return true;
}

static bool testListInsertAfterCurrent() {
    char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
    List list = listCreate(copyString,freeString);
    listInsertFirst(list,a[0]);
    ASSERT_TEST(listInsertAfterCurrent(NULL, a[1]) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertAfterCurrent(list, a[1]) == LIST_INVALID_CURRENT);
    listGetFirst(list);
    ASSERT_TEST(listInsertAfterCurrent(list, a[1]) == LIST_SUCCESS);
    ASSERT_TEST(strcmp(listGetFirst(list),a[0]) == 0);
    ASSERT_TEST(listGetSize(list) == 2);
    listInsertFirst(list,a[2]);
    ASSERT_TEST(listInsertAfterCurrent(list, a[3]) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 4);
    ASSERT_TEST(strcmp(listGetFirst(list),a[2]) == 0);
    ASSERT_TEST(strcmp(listGetNext(list),a[0]) == 0);
    ASSERT_TEST(strcmp(listGetNext(list),a[3]) == 0);
    ASSERT_TEST(strcmp(listGetNext(list),a[1]) == 0);
    listDestroy(list);
    return true;
}

static bool testListRemoveCurrent() {
    char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
    List list = listCreate(copyString,freeString);
    for (int i=0;i <5; ++i) {
        ASSERT_TEST(listInsertLast(list, a[i]) == LIST_SUCCESS);
    }
    ASSERT_TEST(listGetSize(list) == 5);
    ASSERT_TEST(listRemoveCurrent(list) == LIST_INVALID_CURRENT);
    ASSERT_TEST(listRemoveCurrent(NULL) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(strcmp(listGetFirst(list),a[0]) == 0);
    ASSERT_TEST(listRemoveCurrent(list) == LIST_SUCCESS);
    ASSERT_TEST(listRemoveCurrent(list) == LIST_INVALID_CURRENT);
    ASSERT_TEST(strcmp(listGetFirst(list),a[1]) == 0);
    ASSERT_TEST(strcmp(listGetNext(list),a[2]) == 0);
    ASSERT_TEST(listRemoveCurrent(list) == LIST_SUCCESS);
    listGetFirst(list);
    ASSERT_TEST(strcmp(listGetNext(list),a[3]) == 0);
    ASSERT_TEST(listGetSize(list) == 3);
    return true;
}

static bool testListSort() {
    char* a[5] = {"a","asa","aa","aaadd","abcs"};
    List list = listCreate(copyString,freeString);
    for (int i=0;i < 5 ; ++i) {
        ASSERT_TEST(listInsertLast(list, a[i]) == LIST_SUCCESS);
    }
    listSort(list,compareString);
    ASSERT_TEST(strcmp(listGetFirst(list),a[0]) == 0);
    ASSERT_TEST(strcmp(listGetNext(list),a[2]) == 0);
    ASSERT_TEST(strcmp(listGetNext(list),a[1]) == 0);
    ASSERT_TEST(strcmp(listGetNext(list),a[4]) == 0);
    ASSERT_TEST(strcmp(listGetNext(list),a[3]) == 0);
    return true;
}

//static bool testListClear() {
//	return true;
//}
//
//static bool testListDestroy() {
//	return true;
//}

int main (int argv, char** arc){
	RUN_TEST(testListCreate);
	RUN_TEST(testListFilter);
	RUN_TEST(testListCopy);
	RUN_TEST(testListGetSize);
	RUN_TEST(testListGetFirst);
	RUN_TEST(testListGetNext);
	RUN_TEST(testListInsertFirst);
	RUN_TEST(testListInsertLast);
	RUN_TEST(testListInsertBeforeCurrent);
	RUN_TEST(testListInsertAfterCurrent);
	RUN_TEST(testListRemoveCurrent);
	RUN_TEST(testListSort);
//	RUN_TEST(testListClear);
//	RUN_TEST(testListDestroy);
	return 0;
}

