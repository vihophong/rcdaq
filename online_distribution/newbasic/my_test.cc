
/*
 *--------------------------------------------------------
 *
 * 
 *--------------------------------------------------------
 *
 */




/* system headers */
#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>



#include <strstream.h>


char *interpretFileName ( char *pcFile )
{
	char *pcIntEnv = NULL;

	if ( strncmp( pcFile, "$", strlen( "$" ) ) == 0 && ( strrchr( ++pcFile, '/' ) != NULL ) ) {
		char *pcEnd = strchr( pcFile, '/' );
		char *pcEnv = new char[ pcEnd - pcFile ];
		strncpy( pcEnv, pcFile, pcEnd-pcFile );
		char *pcIntEnv = getenv ( pcEnv );
		pcFile = new char [ strlen( pcIntEnv ) + strlen( ++pcEnd ) + strlen("/") + 1 ];
		strcpy( pcFile, pcIntEnv );
		strcat( pcFile, "/" );
		strcat( pcFile, pcEnd );
	}
	return pcIntEnv;
}


main(int argc, char *argv[])
{

	char *pcFile;
	if ( argc == 2 ) 
		pcFile = interpretFileName( argv[1] );
	if ( pcFile == NULL )
		COUT << argv[1] << std::endl;
	else
		COUT << "*" << pcFile << "*" << std::endl;
}



