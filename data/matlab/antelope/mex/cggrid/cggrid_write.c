/* 
 * Antelope Toolbox for Matlab
 *
 * Kent Lindquist
 * Lindquist Consulting
 * 2003
 */

#define USAGE "Error using ==> cggrid_write\n\n\
Usage: CGGRID_WRITE ( CGG, FORMAT, FILE )\n"

#include <stdio.h>
#include "antelope_mex.h"

void mexFunction ( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )
{
	char	*filename;
	char	*format;
	FILE	*fp;
	CGGrid	*cgg;

	if( nlhs >= 1 ) 
	{
		antelope_mexUsageMsgTxt ( USAGE );
		return;
	}

	if( nrhs != 3 )
	{
		antelope_mexUsageMsgTxt ( USAGE );
		return;
	}
        else if( ! get_cggrid( prhs[0], &cgg ) )
        {
                antelope_mexUsageMsgTxt ( USAGE );
		return;
        }
        else if( ! get_string( prhs[1], &format ) )
        {
                antelope_mexUsageMsgTxt ( USAGE );
		return;
        }
        else if( ! get_string( prhs[2], &filename ) )
        {
                antelope_mexUsageMsgTxt ( USAGE );
		return;
        }

	if( ( fp = fopen( filename, "w" ) ) == NULL ) 
	{
		perror( "cggrid_write" );
		mxFree( format );
		mxFree( filename );
		mexErrMsgTxt( "Failed to open cggrid file for writing." );
	}

	if( cggrid_write( cgg, format, fp ) < 0 ) 
	{
		fclose( fp );
		mxFree( format );
		mxFree( filename );
		antelope_mex_clear_register( 1 );
		mexErrMsgTxt( "Failed to write cggrid file." );
	}
	else 
	{
		antelope_mex_clear_register( 1 );
		fclose( fp );
		mxFree( format );
		mxFree( filename );
	}
}
