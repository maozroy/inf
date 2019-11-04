#include "vector.h"

struct d_vector_t
{
	size_t capacity;
	size_t element_size;
	size_t size;
	void *head;
};



void TestCreate();

int main()
{
	int a = 30000000;
	int b = 35;
	int *c = NULL;
	d_vector_t *vector = VectorCreate(1, 4);
	a = 0;
	VectorPushBack(vector, &a);
	a = 1;
	VectorPushBack(vector, &a);
	b = 2;
	VectorPushBack(vector, &a);
	c = VectorGetItemAddress(vector, 0);
	
	printf("%d", *c);

VectorDestroy(vector);
return 0;
}


void TestCreate()
{

}
