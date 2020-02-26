from sklearn.datasets.samples_generator import make_blobs
import numpy as np
import argparse


def generate_points(n_samples, centers, cluster_std, random_state=100):
    X, labels = make_blobs(n_samples=n_samples, centers=centers,
                           cluster_std=cluster_std, random_state=random_state)
    x = X[:, 0]
    y = X[:, 1]
    return x, y, labels


def save_points(x, y, labels):
    filename = f'data/n={len(x)}_c={len(np.unique(labels))}.csv'
    np.savetxt(filename, np.c_[x, y, labels], delimiter=',')


if __name__ == '__main__':

    parser = argparse.ArgumentParser(
        description='Generate random blobs of data')
    parser.add_argument('-N', '--samples', metavar='n_samples',
                        type=int, help='Number of sample to generate', required=True)
    parser.add_argument('-c', '--clusters', metavar='n_clusters',
                        type=int, help='Number of centers', required=True)
    parser.add_argument('-s', '--std', metavar='cluster_std',
                        type=int, help='Std of cluster', default=0.5)

    args = parser.parse_args()
    x, y, labels = generate_points(args.samples, args.clusters, args.std)
    save_points(x, y, labels)
