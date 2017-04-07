/*  This program is designed to import data processed by dbxcor and build
an ensemble that can be further processed with seispp filters.  The program
will have a lot of options driven by a pf file.
*/
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "seispp.h"
#include "StreamObjectWriter.h"
#include "dbpp.h"
#include "PfStyleMetadata.h"
#include "ensemble.h"
using namespace std;
using namespace SEISPP;
typedef map<string,double> StaMap;   // tuple of station name and arrival time
void usage()
{
    cerr << "dbxcor_import db [-pf pffile]" <<endl
        << "Constructs a serialized 3C ensemble from Antelope database db"<<endl
        << "serialized data written to stdout"<<endl
        << "use -pf to specify alternate pf file to default dbxcor_import.pf"
        <<endl;
    exit(-1);
}
Hypocenter LoadHypo(DatascopeHandle& dbh)
{
  try{
    double lat,lon,depth,otime;
    lat=dbh.get_double("origin.lat");
    lon=dbh.get_double("origin.lon");
    depth=dbh.get_double("origin.depth");
    otime=dbh.get_double("origin.time");
    /* These need to be radians */
    lat=rad(lat);
    lon=rad(lon);
    Hypocenter h(lat,lon,depth,otime,string("tttaup"),string("iasp91"));
    return h;
  }catch(...){throw;};
}
template <class OutputObject> void write_object(OutputObject& d,
        boost::archive::text_oarchive& oa)
{
    try {
        oa << d;
    }catch(...)
    {
        throw SeisppError(string("write_object failed\n")
                +"Is serialization defined for this object type?\n"
                +"Do you have write permission for output directory?");
    }
}
void SetHypo(Hypocenter& h, ThreeComponentEnsemble& d)
{
  /* We use the css3.0 fully qualified names and store the hypocenter information
  in the ensmble metata */
  try{
    /* Note we leave lat and lon in degrees */
    d.put("origin.lat",deg(h.lat));
    d.put("origin.lon",deg(h.lon));
    d.put("origin.depth",h.z);
    d.put("origin.time",h.time);
    /* save lat and lon with a simpler name in radians */
    d.put("lat",h.lat);
    d.put("lon",h.lon);
    d.put("depth",h.z);
    d.put("otime",h.time);
  }catch(...){throw;};
}
StaMap LoadArrivals(DatascopeHandle dbh)
{
  try{
    StaMap sts;
    DBBundle bundle=dbh.get_range();
    Dbptr dbwork;
    dbwork=bundle.parent;
    for(dbwork.record=bundle.start_record;dbwork.record<bundle.end_record;
                                        ++dbwork.record)
    {
      char sta[20];
      double atime;
      dbgetv(dbwork,0,"sta",sta,"time",&time,NULL);
      sts.insert(pair<string,double>(string(sta),atime));
    }
    return sts;
  }catch(...){throw;};
}
/* Painfull parallel routine for getting robust weight values.   Could
be a template */
StaMap LoadWeights(DatascopeHandle dbh)
{
  try{
    StaMap sts;
    DBBundle bundle=dbh.get_range();
    Dbptr dbwork;
    dbwork=bundle.parent;
    for(dbwork.record=bundle.start_record;dbwork.record<bundle.end_record;
                                        ++dbwork.record)
    {
      char sta[20];
      double wt;
      dbgetv(dbwork,0,"sta",sta,"stackwgt",&wt,NULL);
      sts.insert(pair<string,double>(string(sta),wt));
    }
    return sts;
  }catch(...){throw;};
}

double average_times(StaMap atimes)
{
  try{
    StaMap::iterator aptr;
    double sum;
    int count;
    for(aptr=atimes.begin(),sum=0.0,count=0;aptr!=atimes.end();++aptr,++count)
    {
      sum+=(aptr->second);
    }
    return (sum/static_cast<double>(count));
  }catch(...){throw;};
}
ThreeComponentSeismogram stack3c(ThreeComponentEnsemble& d,StaMap& weights,
                                bool use_weights)
{
  try{
    int i,n;
    const string base_error("stack3c procedure:  ");
    string sta;
    n=d.member.size();
    if(n<=0) throw SeisppError(base_error + "ensemble is empty");
    if(!d.member[0].live) throw SeisppError(base_error
      + "member of input ensemble is marked dead");
    double t0,dt;
    t0=d.member[0].t0;
    dt=d.member[0].dt;
    ThreeComponentSeismogram stack=d.member[0];
    dmatrix s3c=stack.u;
    double wt,sumwt;
    if(use_weights)
    {
      sta=d.member[0].get_string("sta");
      wt=weights[sta];
      s3c = wt*s3c;
      sumwt=wt;
    }
    else
    {
      sumwt=1.0;
    }
    for(i=1;i<n;++i)
    {
      dmatrix uwork;
      /* Simply post a warning if any times differ by more than one sample */
      double t0test=d.member[i].t0;
      if(fabs(t0-t0test)/dt>1.0)
      {
        cerr << base_error << "Warning - t0 for data from station "
          <<d.member[i].get_string("sta")<<" differs by more than one sample from referenc"
          <<endl
          << "Reference t0="<<t0<<" this station t0="<<t0test<<endl
          << "Stack may be incorrect"<<endl;
      }
      uwork=d.member[i].u;
      if(use_weights)
      {
        sta=d.member[0].get_string("sta");
        wt=weights[sta];
        uwork=wt*uwork;
        sumwt+=wt;
      }
      else
      {
        sumwt+=1.0;
      }
      /* This is likely to fail if t0 test fails */
      s3c=s3c+uwork;
    }
    stack.u = s3c/sumwt;
    return stack;
  }catch(...){throw;};
}
bool SEISPP::SEISPP_verbose(true);
int main(int argc, char **argv)
{
  int i;
  if(argc<2)usage();
  string dbname(argv[1]);
  string pffile("dbxcor_import");
  bool binary_data("false");
  for(i=2;i<argc;++i)
  {
    string sarg(argv[i]);
    if(sarg=="-pf")
    {
      ++i;
      if(i>=argc)usage();
      pffile=string(argv[i]);
    }
    if(sarg=="-binary")
      binary_data=true;
    else if(sarg=="--help")
      usage();
    else
    {
      usage();
    }
  }
  try{
    AttributeMap am("css3.0");
    shared_ptr<StreamObjectWriter<ThreeComponentEnsemble>> oa;
    if(binary_data)
    {
      oa=shared_ptr<StreamObjectWriter<ThreeComponentEnsemble>>
           (new StreamObjectWriter<ThreeComponentEnsemble>('b'));
    }
    else
    {
      oa=shared_ptr<StreamObjectWriter<ThreeComponentEnsemble>>
             (new StreamObjectWriter<ThreeComponentEnsemble>);
    }
    Pf *pf;
    char *stmp;
    stmp=strdup(pffile.c_str());
    if(pfread(stmp,&pf))
    {
      cerr << "pfread failed on pffile="<<pffile<<endl;
      exit(-1);
    }
    StationChannelMap scm(pf);
    Metadata control(pf);
    DatascopeHandle dbh(dbname,true); // opens read only
    dbh.lookup("xcorarrival");
    dbh.natural_join("xcorbeam");
    dbh.natural_join("evlink");
    dbh.natural_join("event");
    dbh.natural_join("origin");
    list<string> grpkeys;
    grpkeys.push_back("pwfid");
    dbh.sort(grpkeys);
    dbh.group(grpkeys);
    DatascopeHandle dbhwf(dbh); //points to wfdisc, but used as arg for read
    dbhwf.lookup("wfdisc");
    /* We always define these even if we don't save all of them */
    string dbxcor_beam_sta=control.get_string("dbxcor_beam_sta");
    string robust_beam_sta=control.get_string("robust_beam_sta");
    string simple_beam_sta=control.get_string("simple_beam_sta");
    double ts,te;
    /* read window should be a large enough range to allow for array
    moveout.  May want to change this to automatically add a padding
    derived from range of arrival times in xcorarrival but for now fix it. */
    ts=control.get_double("read_window_start_relative_time");
    te=control.get_double("read_window_end_relative_time");
    TimeWindow read_window(ts,te);
  /* stack_window is the final ensmble size (must be smalller than data_window*/
    ts=control.get_double("stack_window_start_time");
    te=control.get_double("stack_window_end_time");
    TimeWindow stack_window(ts,te);
    /* Stopping point for now.  Had to move on today.   Algorithm needs to
    loop over db grouped by wfid.  Find all arrivals linked to that wfid.
    Use time window reader for 3c ensembles.  Work through the ensemble
    extracting those with define arrivals, cut and align by arrival, and then
    post to output enssemble.   After that construct the simple stack and
    robust stack using operator+ with scaling for stackwt to get robust stack */
    long nrec=dbh.number_tuples();
    long irec;
    dbh.rewind();
    ThreeComponentEnsemble d;
    for(irec=0;irec<nrec;++irec,++dbh)
    {
      d.member.clear();
      /* Neither of these procedures will alter dbh */
      Hypocenter h=LoadHypo(dbh);
      SetHypo(h,d);  //Loads hypocenter data into ensemble metadata
      StaMap arrivals=LoadArrivals(dbh);
      StaMap weights=LoadWeights(dbh);
      double avgtime=average_times(arrivals);
      TimeWindow abs_read_window(avgtime+read_window.start,avgtime+read_window.end);
      ThreeComponentEnsemble rawgather(dbh,abs_read_window,scm);
      /* Now we cut each seismogram to a constant time window around the
      the arrival times found in arrivals.   Only keep those that have arrivals */
      auto_ptr<ThreeComponentSeismogram> dcut;
      string atimekey("xcoratime");  // used for ArrivalTimeReference
      vector<ThreeComponentSeismogram>::iterator rgptr;
      for(rgptr=rawgather.member.begin();rgptr!=rawgather.member.end();++rgptr)
      {
        string sta;
        sta=rgptr->get_string("sta");
        StaMap::iterator stptr=arrivals.find(sta);
        if(stptr!=arrivals.end())
        {
          /* This loads the arrival time from xcorarrival into the trace
          header for this seismogram.  This allows us to call the
          ArrivalTimeReference procedure to window the data */
          rgptr->put(atimekey,stptr->second);
          dcut=ArrivalTimeReference(*rgptr,atimekey,stack_window);
          d.member.push_back(*dcut);
        }
      }
      /* Now we compute simple and robust stack */
      ThreeComponentSeismogram simplestack,robuststack;
      /* This is a very fortran way to do this.   Should make this a more
      generic procedure and should add operator + to ThreeComponentSeismogram*/
      simplestack=stack3c(d,weights,false);
      robuststack=stack3c(d,weights,true);
      simplestack.put("sta",simple_beam_sta);
      robuststack.put("sta",robust_beam_sta);
      d.member.push_back(simplestack);
      d.member.push_back(robuststack);
      oa->write(d);
    }
  }catch(SeisppError& serr)
  {
        serr.log_error();
  }
  catch(std::exception& stexc)
  {
        cerr << stexc.what()<<endl;
  }
}
