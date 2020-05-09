#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <locale.h>

#define ARRAY_MAX_SIZE 7

CRITICAL_SECTION criticalSection;
HANDLE* threads;
int tls_index;
void launch_threads(int count);
DWORD WINAPI thread_function(LPVOID arr);
void compute_array(int* main_array, int* result_array);
int find_largest_divisor(int num);
int array_sum(int* array);
void print_array(int* arr);

int main()
{
	int count;
	printf("Input count streams:");
	scanf("%i", &count);

	threads = new HANDLE[count];
	srand(time(0));
	InitializeCriticalSection(&criticalSection);
	launch_threads(count);
	WaitForMultipleObjects(count, threads, TRUE, INFINITE);
	DeleteCriticalSection(&criticalSection);
	system("pause");
	return 0;
}

void launch_threads(int count)
{
	InitializeCriticalSection(&criticalSection);
	int** arrays = new int*[count];
	tls_index = TlsAlloc();

	for (int i = 0; i < count; i++)
	{
		arrays[i] = new int[ARRAY_MAX_SIZE];
		for (int j = 0; j < ARRAY_MAX_SIZE; j++)
			arrays[i][j] = rand() % 90 + 10;
		threads[i] = CreateThread(NULL, 0, thread_function, arrays[i], NULL, NULL);
	}
}

DWORD WINAPI thread_function(LPVOID param)
{
	EnterCriticalSection(&criticalSection);
	printf("Stream id: %u\n", GetCurrentThreadId());
	int* array = (int*)param;
	TlsSetValue(tls_index, (LPVOID)(new int[ARRAY_MAX_SIZE]));
	int sum = 0;

	printf("Stream array: ");
	print_array(array);

	compute_array(array, (int*)TlsGetValue(tls_index));

	printf("\nCounted array: ");
	print_array((int*)TlsGetValue(tls_index));

	sum = array_sum((int*)TlsGetValue(tls_index));
	printf("\nFinal amount: %d\n\n", sum);
	LeaveCriticalSection(&criticalSection);
	return 0;
}

void compute_array(int* main_array, int* result_array)
{
	for (int i = 0; i < ARRAY_MAX_SIZE; i++)
	{
		result_array[i] = find_largest_divisor(main_array[i]);
	}
}

int find_largest_divisor(int num)
{
	int j = num / 2;
	for (int i = j; i >= 2; i--)
	{
		if (num % i == 0)
			return i;
	}
}

int array_sum(int* array)
{
	int result = 0;
	for (int i = 0; i < ARRAY_MAX_SIZE; i++)
	{
		result += array[i];
	}
	return result;
}

void print_array(int* arr)
{
	for (int i = 0; i < ARRAY_MAX_SIZE; i++) {
		printf("%i ", arr[i]);
	}
}
