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


def plot_for_cluster(thread_types, labels, cluster, is_savefig=False):
    for thread_type, label in zip(thread_types, labels):
        plot_benchmark(thread_type[cluster], label)
    plt.xscale('log')
    # plt.yscale('log')
    plt.xlabel('N')
    plt.ylabel('time [ms/one iteration]')
    plt.title('Benchmark of Kmeans | k = ' + cluster)
    plt.legend()
    if is_savefig:
        plt.savefig('../figures/benchmark_'+cluster +
                    '.png', bbox_inches='tight', dpi=500)
    else:
        plt.show()


if __name__ == "__main__":
    cluster_range = ['3', '5', '10']
    data_single_thread = get_all_data("lenovo_results/single", cluster_range)
    data_open_mp = get_all_data("lenovo_results/openmp", cluster_range)

    plot_for_cluster([data_single_thread, data_open_mp],
                     ['single thread', 'OpenMP'], cluster_range[1])
