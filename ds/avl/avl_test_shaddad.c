
#include <assert.h> /* assert */

#include "avl.h"

int CmpFunc(const void *new_data, const void *src_data)
{
    if (*(int *)new_data > *(int *)src_data)
    {
        return 1;
    }
    else if (*(int *)new_data < *(int *)src_data)
    {
        return - 1;
    }

    return 0;
}

int Action(void *data, void *param)
{
    int *int_data = (int *)data;

    ++(*int_data);

    return 0;
}

void AVLCreateDestroyTest();
void AVLInsertTest();
void AVLRemoveTest();
void AVLGetHeightTest();
void AVLFindTest();
void AVLIsEmptyTest();
void AVLForEachTest();

int main()
{
    AVLCreateDestroyTest();
    AVLInsertTest();
    AVLRemoveTest();
    AVLGetHeightTest();
    AVLFindTest();
    AVLIsEmptyTest();
    AVLForEachTest();

    return 0;
}

void AVLCreateDestroyTest()
{
    avl_t *avl = AVLCreate(&CmpFunc);

    AVLDestroy(avl);
}

void AVLInsertTest()
{
    avl_t *avl = AVLCreate(&CmpFunc);
    int arr[] = {2, 4, 6, 8};
    size_t arr_size = 4;
    size_t i = 0;
    int status = 0;

    assert(0 == AVLGetHeight(avl));
    assert(0 == AVLSize(avl));
    
    for (i = 0; i < arr_size; ++i)
    {
        status = AVLInsert(avl, &arr[i]);
        assert(0 == status);
    }

    assert(3 == AVLGetHeight(avl));
    assert(4 == AVLSize(avl));
    AVLDestroy(avl);
}

void AVLRemoveTest()
{
    avl_t *avl = AVLCreate(&CmpFunc);
    int arr[] = {2, 4, 3, 6, 8};
    size_t arr_size = 5;
    size_t dynamic_arr_size = 5;
    size_t i = 0;
    int status = 0;

    assert(0 == AVLGetHeight(avl));
    assert(0 == AVLSize(avl));
    
    for (i = 0; i < arr_size; ++i)
    {
        status = AVLInsert(avl, &arr[i]);
        assert(0 == status);
    }

    printf("%lu\n", AVLSize(avl));
    /* Remove node with 2 children */
    AVLRemove(avl, &arr[1]); /* remove 4 */
    printf("%lu\n", AVLSize(avl));
    assert(AVLSize(avl) == 4);
    assert(2 == AVLGetHeight(avl));

    /* Remove node with 1 child */
    AVLRemove(avl, &arr[0]);
    printf("%lu\n", AVLSize(avl));
    assert(AVLSize(avl) == 3);    
    assert(1 == AVLGetHeight(avl));

    /* Remove leaf */
    AVLRemove(avl, &arr[4]);
    printf("%lu\n", AVLSize(avl));
    assert(AVLSize(avl) == 2);
    assert(1 == AVLGetHeight(avl));

    AVLDestroy(avl);
}

void AVLGetHeightTest()
{
    avl_t *avl = AVLCreate(&CmpFunc);
    int arr[] = {20, 15, 25, 12, 13};
    size_t arr_size = 5;
    size_t i = 0;
    int status = 0;

    assert(0 == AVLGetHeight(avl));
    assert(0 == AVLSize(avl));
    
    for (i = 0; i < arr_size; ++i)
    {
        status = AVLInsert(avl, &arr[i]);
        assert(0 == status);
    }
    printf("height %lu\n", AVLGetHeight(avl));
    assert(3 == AVLGetHeight(avl));
    assert(5 == AVLSize(avl));
    AVLDestroy(avl);    
}

void AVLFindTest()
{
    avl_t *avl = AVLCreate(&CmpFunc);
    int arr[] = {10, 5, 15, 13, 16};
    size_t arr_size = 5;
    size_t i = 0;
    int status = 0;
    void *data = NULL;

    for (i = 0; i < arr_size; ++i)
    {
        status = AVLInsert(avl, &arr[i]);
        assert(0 == status);
    }

    for (i = 0; i < arr_size; ++i)
    {
        data = AVLFind(avl, &arr[i]);
        assert(*(int *)data == arr[i]);
    }

    i = 20;
    data = AVLFind(avl, &i);
    assert(NULL == data);

    AVLDestroy(avl);
}

void AVLIsEmptyTest()
{
    avl_t *avl = AVLCreate(&CmpFunc);
    int arr[] = {10, 5, 15, 13, 16};
    size_t arr_size = 5;
    size_t i = 0;
    int status = 0;
    void *data = NULL;

    assert(1 == AVLIsEmpty(avl));

    for (i = 0; i < arr_size; ++i)
    {
        status = AVLInsert(avl, &arr[i]);
        assert(0 == status);
    }

    for (i = 0; i < arr_size; ++i)
    {
        data = AVLFind(avl, &arr[i]);
        assert(*(int *)data == arr[i]);
    }

    i = 20;
    data = AVLFind(avl, &i);
    assert(NULL == data);

    assert(0 == AVLIsEmpty(avl));

    AVLDestroy(avl);
}
void AVLForEachTest()
{
    avl_t *avl = AVLCreate(&CmpFunc);
    int arr[] = {10, 5, 15, 13, 16};
    int arr_for_each[] = {11, 6, 16, 14, 17};
    size_t arr_size = 5;
    size_t i = 0;
    int status = 0;
    int for_each_res = 0;
    void *data = NULL;

    assert(1 == AVLIsEmpty(avl));

    for (i = 0; i < arr_size; ++i)
    {
        status = AVLInsert(avl, &arr[i]);
        assert(0 == status);
    }

    for (i = 0; i < arr_size; ++i)
    {
        data = AVLFind(avl, &arr[i]);
        assert(*(int *)data == arr[i]);
    }

    for_each_res = AVLForEach(avl, &Action, NULL);
    assert(0 == for_each_res);

    for (i = 0; i < arr_size; ++i)
    {
        data = AVLFind(avl, &arr_for_each[i]);
        assert(*(int *)data == arr_for_each[i]);
    }

    assert(0 == AVLIsEmpty(avl));

    AVLDestroy(avl);
}
