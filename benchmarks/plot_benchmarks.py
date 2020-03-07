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
    return pd.DataFrame(x).to_dict('list')


def get_data_for_cluster(files, c):
    clusters = []
    for f in files:
        if 'c='+c in f:
            num = re.search("\d+", f)
            num = num.group()
            clusters.append((f, num))
    return clusters


def get_all_data(cluster_range):
    files = glob.glob("*.dat")
    data = {}
    for i in cluster_range:
        clusters = get_data_for_cluster(files, i)
        filenames = list(map(lambda x: x[0], clusters))
        ns = list(map(lambda x: x[1], clusters))
        df = get_data(filenames, ns)
        data[i] = df
    return data


def plot_for_cluter(df):
    # TODO: add option for data for multiple threads

    plt.errorbar(df['n'], df["mean"], yerr=df['std'], label='Single thread')
    # plt.yscale('log')
    plt.xscale('log')
    plt.xlabel('Log(N)')
    plt.ylabel('time [ms]')
    plt.title('Benchmark of Kmeans')
    plt.legend()
    plt.show()


cluster_range = ['3', '5', '10']
data_single_thread = get_all_data(cluster_range)
plot_for_cluter(data_single_thread['3'])
