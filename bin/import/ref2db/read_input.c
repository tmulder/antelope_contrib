#include "ref2db.h"

Arr *StrmDas = 0;
Arr *Chan = 0;
extern Arr *Packets;
    
int read_input( SpecPar *par, RunArg *arg )
{

    double epoch;
    int retcode = 0, in_err=0, ifdata, psize = 0 ; 
    int nbytes = 0;
    int done = 0;
    char das[8], *dasname;
    char srcname[64];
    unsigned char *buffer;   
    
    timerr = 0;
    Pblcks = 0;
    allot ( unsigned char *, buffer, IBUF_SIZE );

    StrmDas = newarr(0);
    Chan = newarr(0);
    if( Packets == 0 ) init_pkt( par );

    while( !done )  {
      ifdata = 0;
      if( arg->dev_type == IN_DISK && Pblcks >= PsclDk.maxblk ) break;

        switch( (nbytes = read( arg->ifp, buffer, DSK_BLK_SIZE))  )  {

    	     case 0:
		complain( 0, "read EOF form %s.\n", arg->iport);
                if( close( arg->ifp ) != 0 )  {
                    die( 0, "can't close %s.\n", arg->iport );
    	        }
                done = 1;
                break;                
    	     case -1:
		complain( 1, "read() error on %s.\n", arg->iport);
                if( close( arg->ifp ) != 0 )  {
                    die( 0, "can't close %s.\n", arg->iport );
    	        }
                done = 1;
                break;                
	     default:
                switch( ( retcode = valid_pkt( &buffer, &srcname[0], &epoch,
                               &psize, nbytes, par->byevent )) )  {
                   case -1:
                
                       in_err++;
                       if(in_err >= 30)  {
		           die(0, "too much bad blocks\n");
                       } 
                       break;
                   case 1:
                
                       in_err = 0;
                       if( epoch > arg->etime ) { 
                           complain(0, "All data for specified time window (%lf-%lf) was extracted.\n",
                              arg->stime, arg->etime );
                           if( close( arg->ifp ) != 0 )  {
                              die( 0, "can't close %s.\n", arg->iport );
    	                   }
                           done = 1;
                           break; 
                       }
                       if( epoch >= arg->stime )  {
		           if( arg->nodata ) break;
                           if( !wrt2db ( epoch, srcname, buffer, par ) )
		                die (0, "pkt2db fails\n");
		           in_err = 0;
                       }
                       break;
                   case 0:
                       in_err = 0;
                       break;
		}
		break;
	}
        Pblcks++;
    }
    if( timerr ) fclose(timerr);
    return 1;
}

 
