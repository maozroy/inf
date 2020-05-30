
#include "avl.h"
#include <stdio.h>

#define PRINT_TEST(test, name, num) \
printf("Function: %-17sTest #%d  %s\n", \
(name), (num), (test ? "\033[0;32mPassed\033[0m" : "\033[0;31mFailed\033[0m")) 

int my_cmp (const void *new_data, const void *src_data);
int action_func (void *data, void *for_each_param);
/*
disp *(int*)tree->root->data
disp *(int*)tree->root->children[0]->data
disp *(int*)tree->root->children[1]->data
disp *(int*)tree->root->children[0]->children[0]->data
disp *(int*)tree->root->children[0]->children[1]->data
disp *(int*)tree->root->children[1]->children[0]->data
disp *(int*)tree->root->children[1]->children[1]->data
disp *(int*)tree->root->children[0]->children[0]->children[0]->data
disp *(int*)tree->root->children[0]->children[0]->children[1]->data
disp *(int*)tree->root->children[0]->children[1]->children[0]->data
disp *(int*)tree->root->children[0]->children[1]->children[1]->data
disp *(int*)tree->root->children[1]->children[0]->children[0]->data
disp *(int*)tree->root->children[1]->children[0]->children[1]->data
disp *(int*)tree->root->children[1]->children[1]->children[0]->data
disp *(int*)tree->root->children[1]->children[1]->children[1]->data

disp *(int*)node->data
disp *(int*)node->children[1]->data
disp *(int*)node->children[0]->data
disp *(int*)node->children[1]->children[1]->data
disp *(int*)node->children[1]->children[0]->data
disp tree->root->height
disp tree->root->children[0]->height
disp tree->root->children[1]->height
disp tree->root->children[1]->children[1]->height
disp tree->root->children[1]->children[0]->height
disp tree->root->children[0]->children[1]->height
disp tree->root->children[0]->children[0]->height

disp *(int*)node_to_connect->data
disp *(int*)node_to_connect->children[1]->data
disp *(int*)node_to_connect->children[0]->data

disp *(int*)pivot->data
disp *(int*)pivot->children[1]->data
disp *(int*)pivot->children[0]->data


disp *(int*)new_avl->root->data
disp *(int*)new_avl->root->children[0]->data
disp *(int*)new_avl->root->children[1]->data
disp *(int*)new_avl->root->children[0]->children[0]->data
disp *(int*)new_avl->root->children[0]->children[1]->data
disp *(int*)new_avl->root->children[1]->children[0]->data
disp *(int*)new_avl->root->children[1]->children[1]->data
disp *(int*)new_avl->root->children[0]->children[0]->children[0]->data
disp *(int*)new_avl->root->children[0]->children[0]->children[1]->data
disp *(int*)new_avl->root->children[0]->children[1]->children[0]->data
disp *(int*)new_avl->root->children[0]->children[1]->children[1]->data
disp *(int*)new_avl->root->children[1]->children[0]->children[0]->data
disp *(int*)new_avl->root->children[1]->children[0]->children[1]->data
disp *(int*)new_avl->root->children[1]->children[1]->children[0]->data
disp *(int*)new_avl->root->children[1]->children[1]->children[1]->data

*/
void TestCreateAndInsert(void);

int main()
{
	TestCreateAndInsert();

	return 0;
}


void TestCreateAndInsert(void)
{
	int arr[10] = {10, 20, 15, 30, 25, 50, 60, 65, 55, 40};
	int i = 0;
	int action_param = 20;
	
	avl_t *tree = AVLCreate(my_cmp);

	for (i = 0 ; i < 10 ; i++)
	{
		AVLInsert(tree, &arr[i]);
	}
	/*AVLForEach(tree, action_func, NULL);
	AVLRemove(tree, &arr[7]);
	PRINT_TEST(AVLFind(tree, &action_param) == &arr[9], "find", 0);*/
	printf("%ld\n",AVLSize(tree));
	printf("%ld\n",AVLGetHeight(tree));
	
	AVLForEach(tree, action_func, NULL);
	
	AVLDestroy(tree);


	
}

int action_func (void *data, void *for_each_param)
{
	printf("value is %d\n", *(int*)data);
	return 0;
}


int my_cmp (const void *new_data, const void *src_data)
{
	if (*(int *)new_data < *(int*)src_data)
	{
		return -1;
	}
	else if (*(int *)new_data > *(int*)src_data)
	{
		return 1;
	}
	return 0;

}
