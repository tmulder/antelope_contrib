/* 
 * Antelope Toolbox for Matlab
 *
 * Kent Lindquist
 * Lindquist Consulting
 * 2003
 */

#define USAGE "Error using ==> cggrid2db\n\n\
Usage: CGGRID2DB ( DBPTR, GRID, RECIPE_NAME, GRID_NAME, OUTPUT_FILE, FMT, UNITS, ['overwrite'] )\n"

#include "antelope_mex.h"

void mexFunction ( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )
{
	CGGrid	*cgg;
	char	*recipe_name;
	char	*grid_name;
	char	*output_file;
	char	*format;
	char	*units;
	char	*overwrite;
	char	errmsg[STRSZ];
	Dbptr	db;
	int	flags;
	int	rc;

	if( nrhs != 7 && nrhs != 8 )
	{
		antelope_mexUsageMsgTxt( USAGE );
		return;
	}
	else if( ! get_dbptr( prhs[0], &db ) )
	{
		antelope_mexUsageMsgTxt ( USAGE );
		return;
	}
	else if( ! get_cggrid( prhs[1], &cgg ) )
	{
		antelope_mexUsageMsgTxt ( USAGE );
		return;
	}
	else if( ! get_string( prhs[2], &recipe_name ) )
	{
		antelope_mexUsageMsgTxt ( USAGE );
		return;
	}
	else if( ! get_string( prhs[3], &grid_name ) )
	{
		antelope_mexUsageMsgTxt ( USAGE );
		return;
	}
	else if( ! get_string( prhs[4], &output_file ) )
	{
		antelope_mexUsageMsgTxt ( USAGE );
		return;
	}
	else if( ! get_string( prhs[5], &format ) )
	{
		antelope_mexUsageMsgTxt ( USAGE );
		return;
	}
	else if( ! get_string( prhs[6], &units ) )
	{
		antelope_mexUsageMsgTxt ( USAGE );
		return;
	}

	if( nrhs == 8 ) 
	{
		if( ! get_string( prhs[7], &overwrite ) ) 
		{
			antelope_mexUsageMsgTxt ( USAGE );
			return;
		}
		else if( ! STREQ( overwrite, "overwrite" ) )
		{
			antelope_mexUsageMsgTxt ( USAGE );
			return;
		}
		else
		{
			mxFree( overwrite );
			flags |= CG_OVERWRITE;
		}
	}

	rc = cggrid2db( db, cgg, recipe_name, grid_name,
		        output_file, format, units, flags );

	mxFree( recipe_name );
	mxFree( grid_name );
	mxFree( output_file );
	mxFree( format );
	mxFree( units );

	if( rc != 0 ) 
	{
		antelope_mex_clear_register( 1 );
		mexErrMsgTxt( "Failed to save cggrid to database");
	}
}
