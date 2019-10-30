#include <stdio.h>

void bursa (int arr[], int len, int *final_buy, int *final_sell)
{
	int buy = 0;
	int sell = 0;

	for(; sell < len ; sell ++)
	{
		if ((arr[sell]-arr[buy]) > (arr[*final_sell]-arr[*final_buy]))
		{
			*final_buy = buy;
			*final_sell = sell;
		}
		
		if (arr[sell] < arr[buy])
		{
			buy = sell;
		}
	}

	return;
}

void mybursa (int arr[], int len)
{
int i = 0; 
int buy = 0;
int profit = 0;
int sell = 0;

for (;i<len;i++)
{
	if (arr[i] < arr[buy])
	{
		buy = i;
	}
	if(arr[i] - arr[buy] >= profit)
	{
		profit = arr[i] - arr[buy];
		sell = i;
	}
}
printf("\nbuy is %d, sell is %d, profit is %d\n",buy, sell, profit);
return;
}

int main()
{

int str [] = {6, 50	, 3, 5, 1, 4, 9 , 2};
int buy = 0;
int sell = 0;

bursa(str, 8, &buy, &sell);
mybursa(str, 8);
printf("buy index is %d, sell index is %d, profit is %d",buy, sell, (str[sell]-str[buy]));
return 0;
}
