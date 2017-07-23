#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#define		MAX_ARRAY	4096	
#define		Swap(a, b) { int tmp = a; a = b; b = tmp; } 
#define		toggle(a)	( a = 1 - a )

void	init_array		(int *a, int kosu);
void	disp_array		(int *a, int start, int end);
void	bubble_sort   	(int *array, int kosu);
void	selection_sort	(int *array, int kosu);
void	insert_sort   	(int *array, int kosu);
void	quick_sort    	(int *array, int left, int right);
void	heap_sort     	(int *array, int kosu);
void	down_heap     	(int *array, int root, int bottom);

void	mergeSort     	(int numbers[], int temp[], int array_size);
void 	m_sort        	(int numbers[], int temp[], int left, int right);
void 	merge         	(int numbers[], int temp[], int left, int mid, int right);
void	natural_merge  	(int numbers[], int kosu);


int	main(int argc, char **argv)
{
	int		array[MAX_ARRAY];
	int		sorted[MAX_ARRAY];
	int		tmpary[MAX_ARRAY];
	int		kosu;

	if( argc != 2 ) {
		printf("usage: %s kosu_of_rand \n", argv[0]);
		exit( 1 );
	}

	kosu	=	atoi( argv[1] );
	if( kosu > MAX_ARRAY ) {
		printf("kosu_of_rand exceed %d, set lower\n", MAX_ARRAY);
		exit( 1 );
	}

	init_array( array, kosu );
	printf("Before sort\n");
	disp_array( array, 0, kosu - 1 );

	memcpy( (void *)sorted, (void *)array, (size_t)( sizeof(int) * kosu) );
	printf("Bubble sort : right to left -start-\n");
	disp_array( sorted, 0, kosu - 1 );
	bubble_sort( sorted, kosu);
	printf("Bubble sort -end-\n");
	disp_array( sorted, 0, kosu - 1 );

	memcpy( (void *)sorted, (void *)array, (size_t)( sizeof(int) * kosu) );
	printf("Selection sort : find minimum -start-\n");
	disp_array( sorted, 0, kosu - 1 );
	selection_sort( sorted, kosu);
	printf("Selection sort -end-\n");
	disp_array( sorted, 0, kosu - 1 );

	memcpy( (void *)sorted, (void *)array, (size_t)( sizeof(int) * kosu) );
	printf("Insert sort -start-\n");
	disp_array( sorted, 0, kosu - 1 );
	insert_sort( sorted, kosu);
	printf("Insert sort -end-\n");
	disp_array( sorted, 0, kosu - 1 );

	memcpy( (void *)sorted, (void *)array, (size_t)( sizeof(int) * kosu) );
	printf("Quick sort -start-\n");
	disp_array( sorted, 0, kosu - 1);
	quick_sort( sorted, 0, kosu - 1);
	printf("Quick sort -end-\n");
	disp_array( sorted, 0, kosu - 1);

	memcpy( (void *)sorted, (void *)array, (size_t)( sizeof(int) * kosu) );
	printf("Heap sort -start-\n");
	disp_array( sorted, 0, kosu - 1);
	heap_sort( sorted, kosu);
	printf("Heap sort -end-\n");
	disp_array( sorted, 0, kosu - 1);

	memcpy( (void *)sorted, (void *)array, (size_t)( sizeof(int) * kosu) );
	printf("Merge sort -start-\n");
	disp_array( sorted, 0, kosu - 1);
	mergeSort( sorted, tmpary, kosu);
	printf("Merge sort -end-\n");
	disp_array( sorted, 0, kosu - 1);

	memcpy( (void *)sorted, (void *)array, (size_t)( sizeof(int) * kosu) );
	printf("Natural sort -start-\n");
	disp_array( sorted, 0, kosu - 1);
	natural_merge( sorted, kosu);
	printf("Natural sort -end-\n");
	disp_array( sorted, 0, kosu - 1);

	return 0;
}

void	init_array( int *array, int narray )
{
	int	tmp;
	int	i,j;
	int	ck;		/* 1:ok 0:ng */

	srand( 97 );
	array[0]	=	rand()%narray ;

	for(i = 1; i < narray; i++) {
		do {
			tmp	=	rand()%narray ;
			ck	=	0;
			for(j = 0; j < i; j++) {
				if( array[j] != tmp) {
					ck	=	ck + 0;
				} else {
					ck	=	1;
					break;
				}
			}
		} while( ck != 0 ) ;
		array[i]	=	tmp;
	}
}

void	disp_array( int *array, int start , int end )
{
	int	i;

	printf("%d", array[start]);
	for(i = start + 1; i <= end ; i++) {
		if( array[i-1] > array[i] ) {
			printf("!");
		} else {
			printf(" ");
		}
		printf("%d", array[i]);
	}
	printf("\n");
}

void	bubble_sort( int *array, int narray)
{
	int	i,j;

	for(i = 0; i < narray - 1; i++) {
		for(j = narray - 1; j > i; j--) {
			if( array[j] < array[j-1] ) {
				Swap( array[j], array[j - 1] );
			}
		}
		disp_array( array, 0, narray - 1);
	}
}

void	selection_sort( int *array, int narray)
{
	int	i,j;
	int	min;

	for(i=0; i < narray; i++) {
		min	=	i;	
		for(j = i + 1; j < narray; j++) {
			if( array[min] > array[j] ) min = j;
		}
		Swap( array[min], array[i] );
		disp_array( array, 0, narray - 1);
	}
}

void	insert_sort( int *array, int narray)
{
	int	i,j;

	for(i = 1; i < narray; i++) {
		for(j = i; j >= 1 && array[j - 1] > array[j]; --j) {
			Swap( array[j], array[j - 1] );
		}
		disp_array( array, 0, narray - 1);
	}
}

/* http://www1.cts.ne.jp/~clab/hsample/Sort/Sort9.html */
void	quick_sort( int *array, int left, int right)
{
	int	i,j;
	int	pivot;

	i	=	left;
	j	=	right;
	pivot	=	array[ (left + right)/2 ];

	/* array[]を pivot より小さい列, pivot より大きい列 に２分割する */
	while( 1 ) {
		while( array[i] < pivot ) i++;	/* iより左は、pivotより小 */
		while( array[j] > pivot ) j--;	/* jより右は、pivotより大 */
		if(i >= j) break;

		Swap( array[i], array[j] );
		i++;
		j--;
	}

	if(left < i - 1) {	/* pivot より小が２個以上あれば */
		quick_sort( array, left, i - 1);
	}
	if(j + 1 < right) { /* pivot より大が２個以上あれば */
		quick_sort( array, j + 1, right);
	}
	printf("#### quicksort %d - %d : ", left, right);
    disp_array( array, left, right );
}

/* http://www.codereading.com/algo_and_ds/algo/heap_sort.html */
void	heap_sort( int *array, int narray )
{
	int	i;

	for(i = (narray / 2) - 1; i>= 0; i--)
	{
		down_heap( array, i, narray);
	}

	for(i = narray - 1; i >= 1; i--)
	{
		Swap( array[0], array[i] );
		down_heap( array, 0, i - 1);
	}
}

void	down_heap( int *array, int root, int bottom)
{
	int	done;
	int	maxChild;

	done	=	0;
	while( (root * 2 <= bottom) && (!done) )
	{
		if( root * 2 == bottom ) {
			maxChild	=	root * 2;
		} else if( array[ root * 2 ] > array[ root * 2 + 1 ]) {
			maxChild	=	root * 2;
		} else {
			maxChild	=	root * 2 + 1;
		} 

		if( array[ root ] < array[ maxChild ] ) {
			Swap( array[root], array[maxChild] );
			root	=	maxChild;
		} else {
			done	=	1;
		}
	}
}


/* http://www.codereading.com/algo_and_ds/algo/source/merge_sort.c */
void mergeSort(int numbers[], int temp[], int array_size)
{
  m_sort(numbers, temp, 0, array_size - 1);
}

void m_sort(int numbers[], int temp[], int left, int right)
{
  int mid;

  if (right > left)
  {
    mid = (right + left) / 2;
    m_sort(numbers, temp, left, mid);
    m_sort(numbers, temp, mid+1, right);

    merge(numbers, temp, left, mid+1, right);
	printf("#### mergeSort %d - %d : ", left, right);
    disp_array( numbers, left, right );
  }
}


void merge(int numbers[], int temp[], int left, int mid, int right)
{
  int i, left_end, num_elements, tmp_pos;

  left_end = mid - 1;
  tmp_pos = left;
  num_elements = right - left + 1;

  while ((left <= left_end) && (mid <= right))
  {
    if (numbers[left] <= numbers[mid])
    {
      temp[tmp_pos] = numbers[left];
      tmp_pos = tmp_pos + 1;
      left = left +1;
    }
    else
    {
      temp[tmp_pos] = numbers[mid];
      tmp_pos = tmp_pos + 1;
      mid = mid + 1;
    }
  }

  while (left <= left_end)
  {
    temp[tmp_pos] = numbers[left];
    left = left + 1;
    tmp_pos = tmp_pos + 1;
  }
  while (mid <= right)
  {
    temp[tmp_pos] = numbers[mid];
    mid = mid + 1;
    tmp_pos = tmp_pos + 1;
  }

  for (i=0; i <= num_elements; i++)
  {
    numbers[right] = temp[right];
    right = right - 1;
  }
}

/* Cで学ぶデータ構造とアルゴリズム, 著者: 杉山行浩 */
void	natural_merge( int *array, int narray )
{
	int	work[2][MAX_ARRAY];		/* ワーク配列										*/
	int	merge[MAX_ARRAY];		/* マージ配列										*/
	int	kosu[2];				/* ワーク配列の有効数 								*/
	int	eof[2];					/* ワーク配列が EOF（有効数）まで到達したか否か 	*/
	int	idx[2];					/* ワーク配列の running sufix 						*/
	int	sw;						/* どっちのワーク配列かを表すスイッチ 				*/
	int	i,j;
	int	k ;						/* マージ側配列の running sufix 					*/
	int	rensu;					/* 連の数											*/
	int	output, next;

	for(i = 0; i < narray; i++ ) {
		merge[i]	=	array[i];
	}

	do {
		kosu[0]	= kosu[1]	= 0;	
		eof[0]	= eof[1]	= 0;

		/* phase-1 */
		/* merge を連単位で work に分配 */
		sw	= 0;
		work[sw][0]	=	merge[0];
		kosu[sw]	=	1;
		for(k = 1; k < narray; k++) {
			if( merge[k - 1] > merge[k] ) {
				toggle(sw) ;
			}
			j	=	kosu[sw];
			work[sw][j]	=	merge[k];
			kosu[sw]	=	kosu[sw] + 1;
		}
			
		printf("#### disp_work ####\n");
		printf("work0 %d : ", kosu[0]);
		disp_array( work[0], 0, kosu[0] - 1 );
		printf("work1 %d : ", kosu[1]);
		disp_array( work[1], 0, kosu[1] - 1 );

		/* ２つの work から連をマージ */	
		eof[0]	= eof[1]	= 0;
		idx[0]	= idx[1]	= 0;
		k		= 0;		/* array[] の添字			*/
		rensu	= 0;

		while( (eof[0] != 1) && (eof[1] != 1) ) {
			do {
				if( work[0][ idx[0] ] < work[1][ idx[1] ] ) {
					sw	=	0;
				} else {
					sw	=	1;
				}
				
				output	=	merge[k]	=	work[sw][ idx[sw] ];	/* write */
				idx[sw]	=	idx[sw] + 1;							/* read idx inc */
				next	=	work[sw][ idx[sw] ];					/* read next */
				k++;
				if( idx[sw] >= kosu[sw] ) eof[sw] = 1;
			} while( (eof[sw] != 1) && (output <= next) );
				/* swのどちらかが output > next となり、連が終わるまで */ 

			toggle(sw);
				/* swを切り替えて、もう片方の連の残りを output > next まで出力 */
			do {
				output	=	merge[k]	= work[sw][ idx[sw] ];		/* write */
				idx[sw]	=	idx[sw] + 1;							/* read idx inc */
				next	=	work[sw][ idx[sw] ];					/* read next */
				k++;
				if( idx[sw] >= kosu[sw] ) eof[sw] = 1;
			} while ( (eof[sw] != 1) && (output <= next) ) ;
			rensu++;
		} /* work配列が、二つとも EOF となるまで */

		for( sw = 0; sw < 2; sw++ ) {
											/* 連が残っていれば出力 */
			if( eof[sw] != 1 ) {
				do {
					merge[k]	= work[sw][ idx[sw] ];
					idx[sw]	=	idx[sw] + 1;						/* read idx inc */
					k++;
					if( idx[sw] >= kosu[sw] ) eof[sw] = 1;
				} while( eof[sw] != 1);
				rensu++;
			}
		}
		printf("#### disp_merge [%d]####\n", k - 1);
		disp_array( merge, 0, narray - 1 );
	} while ( rensu != 1 ) ;
	
	for(k = 0; k < narray; k++ ){
		array[k]	=	merge[k];
	}
	
}

