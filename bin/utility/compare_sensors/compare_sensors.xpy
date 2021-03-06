import matplotlib.pyplot as plt
from antelope.datascope import closing,\
                               dbopen
from antelope.stock import epoch2str,\
                           pfin,\
                           pfread,\
                           str2epoch
from argparse import ArgumentParser
from numpy import arange
from scipy.signal import resample

def parse_args():
    parser = ArgumentParser()
    parser.add_argument('sta', type=str, help="station")
    parser.add_argument('vel_chan', type=str, help="velocity channel")
    parser.add_argument('acc_chan', type=str, help="acceleration channel")
    parser.add_argument('db', type=str, help="database")
    parser.add_argument('time', type=str, help="time")
    parser.add_argument('twin', type=float, help="time window")
    parser.add_argument('filter',
                        type=str,
                        nargs='?',
                        default="BW 10 3 13 3",
                        help="filter string [\"BW 10 3 13 3\"]")
    parser.add_argument('-s', '--save', type=str, help="save output files")
    args = parser.parse_args()
    args.time = str2epoch(args.time)
    return args

def parse_pf(pfile):
    if pfile:
        pfile = os.path.abspath(pfile)
        if os.path.splitext(pfile)[1] == '.pf':
            pfile = pfin(pfile)
        else:
            pfile = pfin(pfile)
    else:
        pfile = pfread('compare_sensors')
    return eval_dict(pfile.pf2dict())

def eval_dict(my_dict):
    for key in my_dict:
        if isinstance(my_dict[key], dict):
            eval_dict(my_dict[key])
        else:
            if key in locals():
                continue
            try:
                my_dict[key] = eval(my_dict[key])
            except (NameError, SyntaxError):
                pass

    return my_dict

def get_data():
    global args
    try:
        with closing(dbopen(args.db, 'r')) as db:
            tbl_wfdisc = db.lookup(table='wfdisc')
            tr1 = tbl_wfdisc.trloadchan(args.time,
                                         args.time + args.twin,
                                         args.sta,
                                         args.vel_chan)
            tr2 = tbl_wfdisc.trloadchan(args.time,
                                         args.time + args.twin,
                                         args.sta,
                                         args.acc_chan)
    except Exception:
        #actually do something here.
        raise
    return (tr1, tr2)

def process_data(data):
    global args
    tr1, tr2 = data
    tr1.trapply_calib()
    tr2.trapply_calib()
    tr1.trfilter("DEMEAN; DIF; {}".format(args.filter))
    tr2.trfilter("DEMEAN; {}".format(args.filter))
    tr1.record, tr2.record = 0, 0
    tr1, tr2 = tr1.trdata(), tr2.trdata()
    if len(tr1) > len(tr2):
        tr1 = resample(tr1, len(tr2))
    elif len(tr2) > len(tr1):
        tr2 = resample(tr2, len(tr1))
    max_tr1, max_tr2 = max([abs(v) for v in tr1]), max([abs(v) for v in tr2])
    calib = max_tr1 / max_tr2
    return (tr1, tr2)

def plot_data(data):
    global args
    tr1, tr2 = data
    fig = plt.figure(figsize=(24, 12))
    axs = [fig.add_subplot(3, 1, i) for i in (1, 2, 3)]
    axs[0].plot(tr1, 'b')
    axs[1].plot(tr2, 'r', linestyle='--')
    axs[2].plot(tr1, 'b')
    axs[2].plot(tr2, 'r', linestyle='--')
    axs[0].set_ylabel("{} : {} nm/s/s".format(args.sta, args.vel_chan), fontsize=16)
    axs[1].set_ylabel("{} : {} nm/s/s".format(args.sta, args.acc_chan), fontsize=16)
    axs[2].set_ylabel("nm/s/s")
    xticks = arange(0, len(tr1), len(tr1) / 4)
    dt = args.twin / len(tr1)
    xtick_labels = [epoch2str(t, "%Y%j %H:%M:%S.%s") for t in\
            [args.time + x * dt for x in xticks]]
    for i in range(len(axs)):
        axs[i].set_xticks(xticks)
        axs[i].set_xticklabels(xtick_labels)
    if args.save:
        print 'saving'
        print '{}/{}_{}_x_{}_{}.png'.format(os.path.abspath(args.save),
                                               args.sta,
                                               args.vel_chan,
                                               args.acc_chan,
                                               args.time)
        plt.savefig('{}/{}_{}_x_{}_{}.png'.format(os.path.abspath(args.save),
                                                     args.sta,
                                                     args.vel_chan,
                                                     args.acc_chan,
                                                     args.time),
                    format='png')
    else:
        plt.show()

def main():
    global args
    args = parse_args()
    #pfile = parse_pf(args.pfile)
    plot_data(process_data(get_data()))

if __name__ == '__main__':
    main()
