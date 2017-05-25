#include <stdbool.h>
#include <malloc.h>
#include "list_mtm1.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define ASSERT(test_number, test_condition)  \
if (!(test_condition)) {printf("\nTEST %s FAILED", test_number); } \
else printf("\nTEST %s PASSED", test_number);


static ListElement copyString(ListElement str){
    assert(str);
    char* copy = malloc(strlen(str)+1);
    return copy != NULL ? strcpy(copy, str) : NULL;
}

static void freeString(ListElement str){
    free(str);
}

bool isShorterThan(ListElement str, ListFilterKey length) {
    return strlen(str) < *(int*) length;
}

bool isLongerThan(ListElement string, ListFilterKey key) {
    return strlen(string) > *(int*)key;
}






int main(int argc, char **argv){
    //Check listCreate Func
    List list1 = listCreate(NULL, freeString);
    ASSERT("1.1", list1 == NULL);
    list1 = listCreate(copyString, NULL);
    ASSERT("1.2", list1 == NULL);
    list1 = listCreate(copyString, freeString);
    ASSERT("1.3", list1 != NULL);
    //Check listInsertFirst and listInsertLast Func
    char* father = "father";
    char* mother = "mother";
    ListResult result1 = listInsertFirst(NULL,father);
    ASSERT("2.1", result1 == LIST_NULL_ARGUMENT);
    result1 = listInsertFirst(list1,father);
    ASSERT("2.2", result1 == LIST_SUCCESS);
    result1 = listInsertFirst(list1,mother);
    ASSERT("2.3", result1 == LIST_SUCCESS);
    char* brother = "brother";
    char* sister = "sister";
    result1 = listInsertLast(NULL,brother);
    ASSERT("2.4", result1 == LIST_NULL_ARGUMENT);
    result1 = listInsertLast(list1,brother);
    ASSERT("2.5", result1 == LIST_SUCCESS);
    result1 = listInsertLast(list1,sister);
    ASSERT("2.6", result1 == LIST_SUCCESS);
    //Check listGetSize Func
    int result2 = listGetSize(NULL);
    ASSERT("3.1", result2 == -1);
    result2 = listGetSize(list1);
    ASSERT("3.2", result2 == 4);
    //Check listCopy Func
    List list2 = listCopy(NULL);
    ASSERT("4.1", list2 == NULL);
    list2 = listCopy(list1);
    ASSERT("4.2", list2 != NULL && (listGetSize(list1) == listGetSize(list2)));
    //Check listFilter Func
    int key = 7;
    List list3 = listFilter(NULL, isShorterThan,&key);
    ASSERT("5.1", list3 == NULL);
    list3 = listFilter(list2, NULL, &key);
    ASSERT("5.2", list3 == NULL);
    list3 = listFilter(list2, isShorterThan,&key);
    ASSERT("5.3", list3 != NULL && (listGetSize(list3) == 3));
    key = -1;
    List list4 = listFilter(list2, isShorterThan,&key);
    ASSERT("5.4", list4 == NULL);
    key = 6;
    list4 = listFilter(list2, isLongerThan,&key);
    ASSERT("5.5", list4 != NULL && (listGetSize(list4) == 1));

    listDestroy(list1);
    listDestroy(list2);
    listDestroy(list3);
    listDestroy(list4);
    return 0;
}



