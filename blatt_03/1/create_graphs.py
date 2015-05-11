#!/usr/bin/env python3

import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
import numpy as np


pp = PdfPages('speedup.pdf')


def create_speedup_graph(data):
    base_time = data[0, 1]

    def speedup(time):
        return base_time / time

    plt.title("Speedup")
    plt.plot(data[:, 0], [speedup(t) for t in data[:, 1]], 'ro')
    plt.axis((0, 13, 0, 5))
    plt.xlabel('Processes')
    plt.ylabel('Speedup')
    # plt.show()
    pp.savefig()
    plt.close()


def create_efficiency_graph(data):
    base_time = data[0, 1]

    def efficiency(proc, time):
        return base_time / time / proc

    plt.title("Efficiency")
    plt.plot(data[:, 0], [efficiency(p, t) for p, t in data], 'bo')
    plt.axis((0, 13, 0, 1.2))
    plt.xlabel('Processes')
    plt.ylabel('Efficiency')
    # plt.show()
    pp.savefig()
    plt.close()


def main():
    measurements = np.loadtxt('./speedup-87857.out')
    shape = (measurements.shape[0], 2)
    num_m = measurements.shape[1] - 1
    data = np.zeros(shape)
    data[:, 0] = measurements[:, 0]
    data[:, 1] = sum(measurements[:, m] for m in range(1, num_m + 1)) / num_m
    create_speedup_graph(data)
    create_efficiency_graph(data)
    pp.close()


if __name__ == '__main__':
    main()
