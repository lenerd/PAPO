#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np


def create_speedup_graph(data):
    base_time = data[0, 1]

    def speedup(time):
        return base_time / time

    print([speedup(t) for t in data[:, 1]])
    plt.plot(data[:, 0], [speedup(t) for t in data[:, 1]], 'ro')
    plt.show()


def create_efficiency_graph(data):
    base_time = data[0, 1]

    def efficiency(proc, time):
        return base_time / time / proc

    print([efficiency(p, t) for p, t in data])
    plt.plot(data[:, 0], [efficiency(p, t) for p, t in data], 'bo')
    plt.show()


def main():
    measurements = np.loadtxt('./speedup-87857.out')
    shape = (measurements.shape[0], 2)
    num_m = measurements.shape[1] - 1
    data = np.zeros(shape)
    data[:, 0] = measurements[:, 0]
    data[:, 1] = sum(measurements[:, m] for m in range(1, num_m + 1)) / num_m
    print(data)
    create_speedup_graph(data)
    create_efficiency_graph(data)


if __name__ == '__main__':
    main()
