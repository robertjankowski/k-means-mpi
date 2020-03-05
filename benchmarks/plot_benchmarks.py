import matplotlib.pyplot as plt
import pandas as pd
import numpy as np


def load_data(filename, n):
    df = pd.read_csv(filename, names=[n])
    # I assumed that values are from normal distribution
    x_mean = np.mean(df[n])
    x_std = np.std(df[n])
    return {'mean': x_mean, 'std': x_std, 'n': n}


def get_data(filenames, ns):
    x = [load_data(filename, n) for filename, n in zip(filenames, ns)]
    return pd.DataFrame(x).to_dict('list')


filenames = ["n=100000_c=4.dat", "n=1000_c=2.dat"]
ns = [100000, 1000]

df = get_data(filenames, ns)

plt.errorbar(df['n'], df["mean"], yerr=df['std'], label='Single thread')
# plt.yscale('log')
plt.xscale('log')
plt.xlabel('Log(N)')
plt.ylabel('time [ms]')
plt.title('Benchmark of Kmeans')
plt.legend()
plt.show()
