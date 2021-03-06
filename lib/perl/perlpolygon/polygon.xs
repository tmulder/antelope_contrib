#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "polygon.h"

static char *
elogmsgs()
{
    char *log ;
    log = elog_string(0) ;
    elog_clear() ;
    return log ;
}

MODULE = polygon		PACKAGE = polygon		
PROTOTYPES: DISABLE


void
inWhichPolygons(idatabase, itable, ifield, irecord, lat, lon)
	int		idatabase
	int 	itable
	int 	ifield
	int 	irecord
	double	lat
	double	lon
	PPCODE:
	{ 
	Dbptr db;
	Dbptr dbr;
	Point P;
	db.database= idatabase;
	db.table= itable;
	db.field= ifield;
	db.record= irecord;
	P.lat= lat;
	P.lon= lon;
	dbr= inWhichPolygons(db, P);
	EXTEND(sp,4);
	PUSHs(sv_2mortal(newSViv(dbr.database)));	
	PUSHs(sv_2mortal(newSViv(dbr.table)));	
	PUSHs(sv_2mortal(newSViv(dbr.field)));	
	PUSHs(sv_2mortal(newSViv(dbr.record)));	
	}

char *
windrose(azimuth)
	double azimuth

void
readPolygon(idatabase, itable, ifield, irecord)
	int		idatabase
	int 	itable
	int 	ifield
	int 	irecord
	PPCODE:
	{
	Dbptr 	db;
	int		nrecords;
	int 	i;
	Point 	*poly;	
	
	db.database= idatabase;
	db.table= itable;
	db.field= ifield;
	db.record= irecord;
	nrecords= readPolygon(db,&poly);
	EXTEND(sp,nrecords * 2);

	for (i= 0; i < nrecords; i++) {
 		PUSHs(sv_2mortal(newSVnv(poly[i].lat)));
 		PUSHs(sv_2mortal(newSVnv(poly[i].lon)));
 	}
	free(poly);
	}

# void
# writePolygondata(idatabase, itable, ifield, irecord,)
# 	int		idatabase
# 	int 	itable
# 	int 	ifield
# 	int 	irecord
# 	
# 	PPCODE:
# 	{
# 	Dbptr 	db;
# 	int		record;
# 	
# 	int 	i;
# 	Point 	*poly;	
# 	
# 	db.database= idatabase;
# 	db.table= itable;
# 	db.field= ifield;
# 	db.record= irecord;
# 	record= writePolygonData(db,poly,npoints,pname,ptype,auth,dir,dfile,pcode);
# 	EXTEND(sp,nrecords * 2);
# 
# 	for (i= 0; i < nrecords; i++) {
#  		PUSHs(sv_2mortal(newSVnv(poly[i].lat)));
#  		PUSHs(sv_2mortal(newSVnv(poly[i].lon)));
#  	}
# 	}
# 
