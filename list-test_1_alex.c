#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


//#include "list.h"
#include "list_mtm1.h"


#define ASSERT(test_number, test_condition)  \
if (!(test_condition)) {printf("\nTEST %s FAILED", test_number); } \
else printf("\nTEST %s OK", test_number);



ListElement copyInt(ListElement number) {
    assert(number);
    int* copy_number = malloc(1 * sizeof(int));
    if (copy_number == NULL) return NULL;

    *copy_number = *((int*)number);
    return copy_number;
}

void freeInt(ListElement number) {
    if (number == NULL) {
        return;
    }
    free(number);
}

int CompareInt(ListElement number1, ListElement number2) {
    return *(int*)number1 - *(int*)number2;
}

bool FilterInt(ListElement element, ListFilterKey key) {
    return (*(int*)element > *(int*)key)? true: false;
}


int main(int argc, char **argv)
{
    // ========================================================
    // ================ TEST #1 - LIST OF INTs ================
    {

       // char* null_str = NULL;
        List null_list = NULL;
        List first_list = listCreate(NULL, freeInt);
        ASSERT("1.1 ", first_list == NULL)      // NULL copyElement

        first_list = listCreate(copyInt, NULL);
        ASSERT("1.2 ", first_list == NULL)      // NULL freeElement

        first_list = listCreate(copyInt, freeInt);
        ASSERT("1.3 ", first_list != NULL)      // list created

        List copied_list = listCopy(null_list);
        ASSERT("1.4 ", copied_list == NULL)     // NULL list to copy

        copied_list = listCopy(first_list);
        ASSERT("1.5 ", copied_list != NULL)     // list copied

        int first_list_size = listGetSize(null_list);
        ASSERT("1.6 ", first_list_size == -1)   // NULL list to size

        first_list_size = listGetSize(first_list);
        ASSERT("1.7 ", first_list_size == 0)   // empty list size 0

        ListElement int_element = listGetFirst(null_list);
        ASSERT("1.8 ", int_element == NULL)   // NULL list

        int_element = listGetFirst(first_list);
        ASSERT("1.9 ", int_element == NULL)   // empty list

        int_element = listGetNext(null_list);
        ASSERT("1.10", int_element == NULL)   // NULL list

        int_element = listGetNext(first_list);
        ASSERT("1.11", int_element == NULL)   // empty list

        int_element = listGetCurrent(null_list);
        ASSERT("1.12", int_element == NULL)   // NULL list

        int_element = listGetCurrent(first_list);
        ASSERT("1.13", int_element == NULL)   // emtpy list

        int number_1 = 14;
        ListResult flag = listInsertFirst(null_list, &number_1);
        ASSERT("1.14", flag == LIST_NULL_ARGUMENT)   // NULL list

        flag = listInsertFirst(first_list, &number_1);
        ASSERT("1.15", flag == LIST_SUCCESS)   // inserted

        ListElement temp_int = listGetFirst(first_list);
        ASSERT("1.16", *(int*)temp_int == 14)   // inserted ok

        int number_2 = 15;
        flag = listInsertLast(null_list, &number_2);
        ASSERT("1.17", flag == LIST_NULL_ARGUMENT)      // NULL list

        flag = listInsertLast(first_list, &number_2);
        ASSERT("1.18", flag == LIST_SUCCESS)    // inserted ok

        temp_int = listGetNext(first_list);
        ASSERT("1.19", *(int*)temp_int == 15)   // inserted ok

        flag = listInsertBeforeCurrent(null_list, &number_1);
        ASSERT("1.20", flag == LIST_NULL_ARGUMENT)      // NULL list

        flag = listInsertAfterCurrent(null_list, &number_1);
        ASSERT("1.21", flag == LIST_NULL_ARGUMENT)      // NULL list

        int_element = listGetNext(first_list);
        flag = listInsertBeforeCurrent(first_list, &number_1);
        ASSERT("1.22", flag == LIST_INVALID_CURRENT)    // iterator in bed place

        flag = listInsertAfterCurrent(first_list, &number_1);
        ASSERT("1.23", flag == LIST_INVALID_CURRENT)    // iterator in bed place

        int_element = listGetFirst(first_list);
        flag = listRemoveCurrent(null_list);
        ASSERT("1.24", flag == LIST_NULL_ARGUMENT)      // NULL list

        flag = listRemoveCurrent(first_list);
        ASSERT("1.25", flag == LIST_SUCCESS)    // removed ok

        flag = listRemoveCurrent(first_list);
        ASSERT("1.26", flag == LIST_INVALID_CURRENT)    // iterator in bed place

        flag = listClear(null_list);
        ASSERT("1.27", flag == LIST_NULL_ARGUMENT)  // NULL list

        flag = listClear(first_list);
        ASSERT("1.28", flag == LIST_SUCCESS)

        listDestroy(null_list);
        listDestroy(first_list);

        List int_list = listCreate(copyInt, freeInt);
        for (int i=20; i >= 0; i--) {           // inserting 21 numbers in in descending order
            listInsertFirst(int_list, &i);
        }
        flag = listSort(int_list, NULL);
        ASSERT("1.29", flag == LIST_NULL_ARGUMENT)       // NULL compere func

        flag = listSort(null_list, CompareInt);
        ASSERT("1.30", flag == LIST_NULL_ARGUMENT)       // NULL list

        flag = listSort(int_list, CompareInt);
        int_element = listGetFirst(int_list);
        ASSERT("1.31", flag == LIST_SUCCESS && *(int*)int_element == 0)     // sorting


        listGetNext(int_list);
        listGetNext(int_list);
        listGetNext(int_list);
        List new_list = listCopy(int_list);
        ASSERT("1.32", *(int*)listGetNext(new_list) == *(int*)listGetNext(int_list))    // copied list the same
        ASSERT("1.33", *(int*)listGetFirst(new_list) == *(int*)listGetFirst(int_list))      // copied list the same

        listGetFirst(int_list);
        listGetNext(int_list);
        for (int i=10; i >= 0; i--) {
            int num = -1;
            listInsertBeforeCurrent(int_list, &num);
        }
        for (int i=10; i >= 0; i--) {
            int num = 10;
            listInsertAfterCurrent(int_list, &num);
        }
        listSort(int_list, CompareInt);
        ASSERT("1.34", *(int*)listGetFirst(int_list) == -1)     // sorting with couple of same elements


//        listSort(int_list, CompareInt);
//        ASSERT("1.35", *(int*)listGetFirst(int_list) == -1)     // sorting with couple of same elements

//        ASSERT("1.34", (int)listGetFirst(new_list) == (int)listGetFirst(new_list)) // getting only the address of element

        List tmp_list = listCreate(copyInt, freeInt);
        for (int i=10; i >= 0; i--) {
            int num = 10;
            listInsertAfterCurrent(tmp_list, &num);
        }

        List new_list1 = listCreate(copyInt, freeInt);              //*
        int* null_number = NULL;                                    //*
        int num = 2;                                                //*
        listInsertFirst(new_list1, &num);                           //*
        ASSERT("1.35", *(int*)listGetFirst(new_list1) == 2)         //* checkes if in case of inserting NULL element...
        ListResult flag1 = listInsertFirst(new_list1, null_number); //* the NODE is created and SUCCESS is returned
        ASSERT("1.36", flag1 == LIST_SUCCESS)                       //* but the element in the new NODE is equal NULL!!
        ASSERT("1.37", (int*)listGetFirst(new_list1) == NULL)       //*
        ASSERT("1.38", *(int*)listGetNext(new_list1) == 2)          //*

        int key = 2;
        List temp_list = listFilter(null_list, FilterInt, &key);
        ASSERT("1.39", temp_list == NULL)       // NULL list

        temp_list = listFilter(int_list, NULL, &key);
        ASSERT("1.40", temp_list == NULL)       // NULL func

        temp_list = listFilter(int_list, FilterInt, &key);
        ASSERT("1.41", temp_list != NULL && *(int*)listGetFirst(int_list) != 3)     // good filter

    }
    // ========================================================
    // ========================================================

    return 0;
}

