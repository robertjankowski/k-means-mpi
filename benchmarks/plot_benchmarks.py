import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import glob
import re


def load_data(filename, n):
    df = pd.read_csv(filename, names=[n])
    # I assumed that values are from normal distribution
    x_mean = np.mean(df[n])
    x_std = np.std(df[n])
    return {'mean': x_mean, 'std': x_std, 'n': n}


def get_data(filenames, ns):
    x = [load_data(filename, n) for filename, n in zip(filenames, ns)]
    df = pd.DataFrame(x)
    if not df.empty:
        df.sort_values(by=['n'], inplace=True)
    return df.to_dict('list')


def get_data_for_cluster(files, c):
    clusters = []
    for f in files:
        if 'c='+c in f:
            num = re.search("\d+", f)
            num = num.group()
            clusters.append((f, num))
    return clusters


def get_all_data(thread_type, cluster_range):
    files = glob.glob(thread_type+"*.csv")
    data = {}
    for i in cluster_range:
        clusters = get_data_for_cluster(files, i)
        filenames = list(map(lambda x: x[0], clusters))
        ns = list(map(lambda x: x[1], clusters))
        df = get_data(filenames, ns)
        if df:
            df['n'] = [int(n) for n in df['n']]
        data[i] = df
    return data


def plot_benchmark(df, label):
    plt.errorbar(df['n'], df["mean"], yerr=df['std'], label=label)


def plot_for_cluster(thread_types, labels, cluster, filename=None, xlog=False):
    for thread_type, label in zip(thread_types, labels):
        plot_benchmark(thread_type[cluster], label)
    plt.xscale('log')
    if xlog:
        plt.yscale('log')
    plt.xlabel('N')
    plt.ylabel('time [ms]')
    plt.title('Benchmark of Kmeans | k = ' + cluster)
    plt.legend(loc='upper left')
    if filename:
        plt.savefig('../figures/' + filename +
                    '.png', bbox_inches='tight', dpi=500)


def plot_all(thread_types, labels, clusters, filename=None, xlog=False):
    fig = plt.figure(figsize=(16, 5))
    for i in range(len(clusters)):
        plt.subplot(1, 3, i+1)
        plot_for_cluster(thread_types, labels, clusters[i], filename, xlog)
    if filename:
        plt.savefig('../figures/' + filename + '.png',
                    bbox_inches='tight', dpi=500)
    else:
        plt.show()


def load_all_data(path, cluster_range):
    data_single_thread = get_all_data(path + '/single', cluster_range)
    data_open_mp = get_all_data(path + '/openmp', cluster_range)
    data_mpi = get_all_data(path + '/mpi', cluster_range)
    return {'single thread': data_single_thread,
            'OpenMP': data_open_mp,
            'MPI': data_mpi}


if __name__ == "__main__":
    cluster_range = ['3', '5', '10']
    data = load_all_data('spell_results', cluster_range)
    plot_all(data.values(), data.keys(), cluster_range, xlog=True)
