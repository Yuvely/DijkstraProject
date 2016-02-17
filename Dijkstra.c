#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int v_count = 0;

struct VERTEX 
{
	char vertex[ 10 ];
};

struct GRAPH
{
	char rightVertex[ 10 ];
	char leftVertex[ 10 ];
	char weight[ 10 ];
};

struct UPDATE
{
	int weight;
	int beforeVertex;
};

struct PRINT_VERTEX
{
	char vertex[ 10 ];
	int weight;
};

int findIndex( struct VERTEX* vp, char vertex[ 10 ] )
{
	int vertexNum = 0;
	int i;

	for( i = 0; i < v_count; i ++ )
	{
		if( !strcmp( vertex, vp[ i ].vertex ) )
		{
			vertexNum = i;
			break;
		}
	}

	return vertexNum;
}

void makeMatrix( struct VERTEX* vp, struct GRAPH* gp, int g_count, int matrix[][ v_count ] )
{
	int row = 0;
	int column = 0;
	int weight = 0;
	int i, j;

	for( i = 0; i < v_count; i ++ )
	{
		for( j = 0; j < v_count; j ++ )
		{
			if( i == j )
			{
				matrix[ i ][ j ] = 0;
			}

			else
			{
				matrix[ i ][ j ] = 99999;
			}
		}
	}

	for( i = 0; i < g_count; i ++ )
	{
		row = findIndex( vp, gp[ i ].leftVertex );
		column = findIndex( vp, gp[ i ].rightVertex );
		weight = atoi( gp[ i ].weight );

		matrix[ row ][ column ] = weight;
	}

	/*
	for( int i = 0; i < v_count; i ++ )
	{
		for( int j = 0; j < v_count; j ++ )
		{
			printf("%d ", matrix[ i ][ j ]);
		}
		printf("\n");
	}
	*/
}

int findShort( int matrix[][ v_count ], int pivot )
{
	int shortWeight = 99999;
	int i;

	for( i = 0; i < v_count; i ++ )
	{
		if( ( shortWeight > matrix[ pivot ][ i ] ) && ( matrix[ pivot ][ i ] != 0 ) )
		{
			shortWeight = i;
		}
	}

	return shortWeight;
}

int findVisited( int matrix[][ v_count ], int pivot )
{
	int check = 0;
	int i;

	for( i = 0; i < v_count; i ++ )
	{
		if( ( matrix[ pivot ][ i ] != 99999 ) && ( matrix[ pivot ][ i ] != 0 ) )
		{
			return 1;
		}
	}

	return 0;
}

void dijkstra( char s[ 10 ], char e[ 10 ], struct VERTEX* vp, int matrix[][ v_count ], struct UPDATE* up, int visited[], int g_count)
{
	int start = findIndex( vp, s );
	int end = findIndex( vp, e );
	int i;

	/* up Initialize */
	for( i = 0; i < v_count; i ++ )
	{
		if( i == start )
		{
			up[ i ].weight = 0;
		}

		else
		{
			up[ i ].weight = 99999;
		}

		up[ i ].beforeVertex = 0; 
	}

	int checkVisited = 0;
	int check = 0;
	int pivot = start;
	int update = 0;
	int checkPivot = 99999;

	while( 1 )
	{
		checkVisited = 0;

		for( i = 0; i < v_count; i ++ )
		{
			checkVisited += visited[ i ];
		}

		// printf("checkVisited : %d\n", checkVisited);

		if( checkVisited == v_count )
		{
			break;
		}
		
		update = findShort( matrix, pivot );

		// printf("update: %d\n", update);

		if( up[ update ].weight > up[ pivot ].weight + matrix[ pivot ][ update ] ) 
		{
			up[ update ].weight = up[ pivot ].weight + matrix[ pivot ][ update ];
			up[ update ].beforeVertex = pivot;

			// printf("weight : %d		beforeVertex : %d\n", up[ update ].weight, up[ update ].beforeVertex );
		}
	
		matrix[ pivot ][ update ] = 99999;

		check = findVisited( matrix, pivot );

		// printf("check : %d\n", check);

		if( check == 0 )
		{
			visited[ pivot ] = 1;
			checkPivot = 99999;

			for( i = 0; i < v_count; i ++ )
			{
				if( ( visited[ i ] != 1 ) && ( checkPivot > up[ i ].weight ) && ( up[ i ].weight != 0 ) )
				{
					checkPivot = i;
				}
			}

			pivot = checkPivot;
			// printf("pivot : %d\n\n", pivot);

		} // if( check == 0 )

	}
}

void findName( struct PRINT_VERTEX* pp, struct VERTEX* vp, struct UPDATE* up, int pivot, int num )
{
	int position = up[ pivot ].beforeVertex;
	int i;

	pp[ num ].weight = up[ pivot ].weight - up[ position ].weight;

	for( i = 0; i < v_count; i ++ )
	{
		if( i == position ) 
		{
			strcpy( pp[ num ].vertex, vp[ i ].vertex );	
		}
	}
}

int main()
{
	FILE* file1 = fopen( "testcase.txt", "r" );

	char start[ 10 ];
	char end[ 10 ];
	int g_count = 0; 

	if( file1 != NULL )
	{
		char nameStr[ 256 ];
		char* pStr;
		char* token;

		while( !feof( file1 ) )
		{
			pStr = fgets( nameStr, sizeof( nameStr ), file1 );
			token = strtok( nameStr, " " );

			if( pStr == NULL )
			{
				break;
			}

			if( !strcmp( token, "\n" ) )
			{
				v_count ++;
			}

			else
			{
				g_count ++;
			}
		}
	}

	struct VERTEX vertex[ v_count ]; 
	struct GRAPH graph[ g_count ];
	int matrix[ v_count ][ v_count ];
	struct UPDATE update[ v_count ];
	int visited[ v_count ];
	struct PRINT_VERTEX print_vertex[ v_count ];
	int i;

	for( i = 0; i < v_count; i ++ )
	{
		visited[ i ] = 0;
	}

	for( i = 0; i < v_count; i ++ )
	{
		strcpy( print_vertex[ i ].vertex, " " );
		print_vertex[ i ].weight = 0;
	}

	FILE* file = fopen( "testcase.txt", "r" );

	if( file != NULL )
	{
		char nameStr[ 256 ];
		char* pStr;
		char* token;
		int count = 0;
		g_count = 0;

		while( !feof( file ) )
		{
			pStr = fgets( nameStr, sizeof( nameStr ), file );

			token = strtok( nameStr, " " );

			if( pStr == NULL )
			{
				break;
			}

			if( !strcmp( token, "\n" ) )
			{
				count ++;
			}

			else
			{
				strcpy( vertex[ count ].vertex, token );

				strcpy( graph[ g_count ].leftVertex, token );

				token = strtok( NULL, " \n" );
				strcpy( graph[ g_count ].rightVertex, token );

				token = strtok( NULL, " \n" );
				strcpy( graph[ g_count ++ ].weight, token );
			}
		}
		
		fclose( file );
	}

	makeMatrix( vertex, graph, g_count, matrix );

	printf("\nstart point : ");
	scanf("%s", start);

	printf("end point : ");
	scanf("%s", end);

	dijkstra( start, end, vertex, matrix, update, visited, g_count );

	// printf("\nVERTEX COUNT : %d\n\n", v_count);
	// printf("\nGRAPH COUNT : %d\n\n", g_count );

	/*
	for( int i = 0; i < v_count; i ++ )
	{
		printf("%d : %s\n", i, vertex[ i ].vertex );
	}
	*/

	printf("\n");

	int pivot = findIndex( vertex, end );
	int total_num = ( v_count - 1 );

	printf("> distance : %d\n\n", update[ pivot ].weight);

	strcpy( print_vertex[ total_num-- ].vertex, end );

	while( 1 )
	{
		findName( print_vertex, vertex, update, pivot, total_num-- );

		pivot = update[ pivot ].beforeVertex;

		if( pivot == findIndex( vertex, start ) )
		{
			break;
		}
	}

	for( i = 0; i < v_count; i ++ )
	{
		if( strcmp( print_vertex[ i ].vertex, " " ) )
		{
			if( !strcmp( print_vertex[ i ].vertex, end ) )
			{
				printf("%s\n\n", print_vertex[ i ].vertex);
			}

			else
			{
				printf("%s -(%d)-> ", print_vertex[ i ].vertex, print_vertex[ i ].weight);	
			}
		}
	}

	return 0;
}
