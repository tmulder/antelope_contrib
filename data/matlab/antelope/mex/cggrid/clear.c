/* 
 * Antelope Toolbox for Matlab
 *
 * Kent Lindquist
 * Lindquist Consulting
 * 2003
 */

#define USAGE "Error using ==> clear\n\n\
Usage: CLEAR ( CGGRID )\n"

#include <stdio.h>
#include "antelope_mex.h"

void mexFunction ( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )
{
	CGGrid *cgg;

	if( nlhs > 1 ) 
	{
		antelope_mexUsageMsgTxt ( USAGE );
		return;
	}

	if( nrhs != 1 )
	{
		antelope_mexUsageMsgTxt ( USAGE );
		return;
	}
        else if( ! get_cggrid( prhs[0], &cgg ) )
        {
                antelope_mexUsageMsgTxt ( USAGE );
		return;
        }
	
	cggrid_free( &cgg );
	antelope_mex_clear_register( 1 );
}
