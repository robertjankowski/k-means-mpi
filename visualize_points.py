import pandas as pd
import matplotlib.pyplot as plt
from matplotlib import rc


def load_data(filename):
    return pd.read_csv(filename, names=['x', 'y', 'label'])


if __name__ == "__main__":
    # rc('font', **{'sans-serif': ['Helvetica']})
    rc('text', usetex=True)

    df = load_data('data/n=1000_c=5.csv')
    
    fig, ax = plt.subplots()
    scatter = ax.scatter(df.x, df.y, c=df.label, cmap='viridis')
    legend = ax.legend(*scatter.legend_elements(), loc='best', title='Classes')
    ax.add_artist(legend)
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    #plt.savefig('example_plot.png', bbox_inches='tight', dpi=500)
    plt.show()
    
