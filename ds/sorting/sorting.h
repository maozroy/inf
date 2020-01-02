typedef int (*is_before_t)(const void *new_data, 
							const void *src_data,
							void *compare_param);

void BubbleSort(int *arr, size_t len);

void SelectionSort(int *arr, size_t len);

void InsertionSort(int *arr, size_t len);

int CountingSort(const int *arr, size_t arr_size, 
				int min_val, int max_val, int *result);
				
int RadixSort(int *arr, size_t arr_size);

int MergeSort(int *arr, size_t arr_size);

void HeapSort(void *arr, size_t arr_size, size_t elem_size, is_before_t func, void *param);

void QuickSort(void *base, size_t nmemb, size_t size, is_before_t func);
