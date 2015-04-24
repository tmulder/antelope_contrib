import antelope.datascope as datascope
from antelope.datascope import closing, dbopen
from antelope.stock import epoch2str

from numpy import arange, digitize, zeros
import numpy as np
import matplotlib
matplotlib.use('Agg')
import datetime
import matplotlib.dates as mdates
import matplotlib.cbook as cbook

import matplotlib.pyplot as plt

from operator import sub

stanames = []

def _parse_args():
    """
    Parse command line options.
    """
    print "SR parse_args"
    from argparse import ArgumentParser
    parser = ArgumentParser()
    parser.add_argument('dbin', type=str, help='Input database.')
    return parser.parse_args()

def _read_data(args):
    """
    Read and return data from database.
    """
    print "SR read_data"
    with closing(dbopen(args.dbin, 'r')) as db:
        view = db.schema_tables['origin']
        view = view.join('event')
        view = view.subset('orid == prefor')
        view = view.join('netmag')
        view = view.join('stamag')
        view = view.subset('time >= _2014355_')
        view = view.sort('time')
        view_stanames = view.sort('stamag.sta', unique=True)

        ## Get time (x-axis) from database and make a string of zeros of same length.
        #alltimes = [record.getv('time')[0] / 86400.0\
        #        for record in view.iter_record()]
        alltimes = [record.getv('time')[0] for record in view.iter_record()]
        min_time = min(alltimes)
        #alltimes = [t - min_time + 1 for t in alltimes]
        args.alltimes = np.array(alltimes)
        
        zeros = [0] * len(alltimes)
        args.zeros = np.array(zeros)
        
        ## Loop through station names and calculate magnitude residual.
        args.stanames = [record.getv('stamag.sta')[0]\
        		 for record in view_stanames.iter_record()]
    	print args.stanames
    	print "staname length = ", len(args.stanames)
    	    
    	#subset_expr = 'sta =~ /%s/' % args.stanames[0]
    	
    	i = 0
    	args.times = []
    	args.stamags = []
    	args.netmags = []
    	args.magresiduals = []
    	args.avg = []
    	args.std = []
    	
    	for sta in args.stanames[0:]:
    	    #print "i = ", i
            subset_expr = 'sta =~ /%s/' % sta
            #print subset_expr
            view_sta = view.subset(subset_expr)
            
            #times = [record.getv('time')[0] / 86400.0\
            #    for record in view_sta.iter_record()]
            times = [record.getv('time')[0] for record in view_sta.iter_record()]
            #times = [t - min_time + 1 for t in times]
            args.times.append(times)
        
            stamags = [record.getv('stamag.magnitude')[0]\
                for record in view_sta.iter_record()]
            args.stamags.append(stamags)
            #print args.stamags[i]
        
            netmags = [record.getv('netmag.magnitude')[0]\
                for record in view_sta.iter_record()]
            args.netmags.append(netmags)
            
            magresiduals = (np.array(stamags) - np.array(netmags))
            average = np.average(magresiduals)
            stddev = np.std(magresiduals)
            average.tolist
            stddev.tolist
            args.avg.append(average)
            args.std.append(stddev)
            
            #print magresiduals
            magresiduals.tolist
            args.magresiduals.append(magresiduals)
            
            #print(sum(int(magresiduals[0]) for numbers in magresiduals) / len(magresiduals)) 

            #print "station, mean, std dev = ", i, sta, args.avg[i], args.std[i]
            
            i += 1
            #print "Calculate i", i

    	print "DONE STATION LOOP"
    return args

def _plot(args):
    print "SR plot"
    
    ## Format plots, 4 curves per plot, 3 plots per page.
    times = []
    ncurves = len(args.stanames)
    ncurvesperplot = 4
    nplotperpage = 3
    npages = (ncurves % (ncurvesperplot * nplotperpage)) + 1
    j = -1
    print "ncurves, nplots, npages = ", ncurves, nplotperpage, npages
    print args.times[0]
    #print epoch2str
    
    #cmap = plt.cm.get_cmap('hsv', ncurvesperplot+1)
    cmap = plt.cm.get_cmap('spectral', ncurvesperplot+1)
    cs = (cmap(arange(ncurvesperplot)))
    
    ## This plots everything on one plot!
    #plt.figure(1, figsize=(10, 10))
    #for j,k in enumerate(args.stanames):
    #    plt.plot(args.times[j], args.magresiduals[j], \
    #        color=[cs[j][0],cs[j][1],cs[j][2]], lw=2.5)
    #plt.show()
    
    ## Plot all data, n curves per plot, m plots per page.
    for k in range(0, ncurvesperplot*nplotperpage*npages, nplotperpage):
        fig, ax = plt.subplots(nplotperpage,sharex=True,sharey=True,figsize=(10,10))
        plt.ylim(-3,3)
        plt.xlabel('Time')
        plt.xticks(rotation=45, size='x-small')
        fig.autofmt_xdate()
        years    = mdates.YearLocator()   # every year
        months   = mdates.MonthLocator()  # every month
        yearsFmt = mdates.DateFormatter('%Y')

        ax[0].xaxis.set_major_locator(years)
        ax[0].xaxis.set_major_formatter(yearsFmt)
        ax[0].xaxis.set_minor_locator(months)
        ax[1].xaxis.set_major_locator(years)
        ax[1].xaxis.set_major_formatter(yearsFmt)
        ax[1].xaxis.set_minor_locator(months)
        ax[2].xaxis.set_major_locator(years)
        ax[2].xaxis.set_major_formatter(yearsFmt)
        ax[2].xaxis.set_minor_locator(months)
   
        #datemin = datetime.date(r.date.min().year, 1, 1)
        #datemax = datetime.date(r.date.max().year+1, 1, 1)
        #ax.set_xlim(datemin, datemax)
        #ax.format_xdata = mdates.DateFormatter('%Y-%m-%d')
        
        #~ticks = np.arange(x.min(), x.max(), 6)
        #~labels = range(ticks.size)
        #~plt.xticks(ticks, labels)
        #~plt.xlabel('Hours')



        if (j == (ncurves-1)):
            label2 = args.stanames[ncurves]

        ## PLOT 1
        ax[0].set_title('Magnitude Residuals')
        #ax[0].set_xticks(args.times[j])
        times[j] = [epoch2str(unix_time,'%Y-%m-%d') for unix_time in args.times[j]]
        #times = np.array(times)
        #ax[0].set_xticklabels(times_ymd)
        print "Aaack"
        print "Times", times

        n = 0
        if (j < (ncurves-1)):
            label1 = args.stanames[j+1] 
        for j in range(k*ncurvesperplot,(k+1)*ncurvesperplot):
            if (j <= (ncurves-1)):
                print "j <= ncurves-1"
                labname = args.stanames[j]+"  %5.3f +/- %5.3f" % (args.avg[j], args.std[j])
                print "labname ", labname
                #ax[0].plot((args.times[j]), args.magresiduals[j],\
                #    color=[cs[n][0],cs[n][1],cs[n][2]], label=labname, lw=1.5, marker="o", markersize=4)
                ax[0].plot(times[j], args.magresiduals[j],\
                    color=[cs[n][0],cs[n][1],cs[n][2]], label=labname, lw=1.5, marker="o", markersize=4)
                print "ax0 plot"
                ax[0].legend(loc='best', fontsize='small', fancybox=True, framealpha=0.5)
                print "ax0 legend"
                ax[0].plot(args.alltimes, args.zeros, 'k--')
                print "ax0 zeros"
                print "j, n = ", j, n, args.stanames[j]
                n += 1
            else:
                print "break 0"
                break
        
        ## Plot 2
        for j in range((k+1)*ncurvesperplot,(k+2)*ncurvesperplot):
            if (j <= (ncurves-1)):
                labname = args.stanames[j]+"  %5.3f +/- %5.3f" % (args.avg[j], args.std[j])
                #print args.magresiduals[j]
                #ax[1].plot(args.times[j], args.magresiduals[j],\
                #    color=[cs[n-4][0],cs[n-4][1],cs[n-4][2]], label=labname, lw=1.5, marker="o", markersize=4)
                ax[1].plot(times, args.magresiduals[j],\
                    color=[cs[n][0],cs[n][1],cs[n][2]], label=labname, lw=1.5, marker="o", markersize=4)
                ax[1].set_ylabel('Stamag - Netmag (ml)')
                ax[1].legend(loc='best', fontsize='small', fancybox=True, framealpha=0.5)
                ax[1].plot(args.alltimes, args.zeros, 'k--')                    
                print "j, n = ", j, n, args.stanames[j]
                n += 1
            else:
                print "break 1"
                break
        
        ## Plot 3
        for j in range((k+2)*ncurvesperplot,(k+3)*ncurvesperplot):
            if (j <= (ncurves-1)):
                labname = args.stanames[j]+"  %5.3f +/- %5.3f" % (args.avg[j], args.std[j])
                #ax[2].plot(args.times[j], args.magresiduals[j],\
                #    color=[cs[n-8][0],cs[n-8][1],cs[n-8][2]], label=labname, lw=1.5, marker="o", markersize=4)
                ax[2].plot(times, args.magresiduals[j],\
                    color=[cs[n][0],cs[n][1],cs[n][2]], label=labname, lw=1.5, marker="o", markersize=4)
                ax[2].legend(loc='best', fontsize='small', fancybox=True, framealpha=0.5)
                ax[2].plot(args.alltimes, args.zeros, 'k--')
                print "j, n = ", j, n, args.stanames[j]
                n += 1
            else:
                print "break 2"
                break
        print "BreakOUT!"
        #fig.autofmt_xdate()
        if (j < (ncurves-1)):
            label2 = args.stanames[j]
            #plt.savefig('fig_'+label1+'-'+label2)  
        else:
            label2 = args.stanames[j-1]
            plt.savefig('fig_'+label1+'-'+label2)  
            break
                
        plt.savefig('fig_'+label1+'-'+label2)  
#    plt.show()
    plt.close('all')
        

if __name__ == '__main__':
    _plot(_read_data(_parse_args()))
else:
    print 'plot_stamags - No db to import!!'

