#!/usr/bin/env python3

import sys
import struct


def main():
    mode = sys.argv[1]
    src = sys.argv[2]
    dst = sys.argv[3]
    if mode == 'matrix':
        with open(src, 'r') as f:
            rows = 0
            cols = len(f.readline().split())
            f.seek(0, 0)
            with open(dst, 'wb') as g:
                g.write(struct.pack('LL', rows, cols))
                for l in f:
                    rows += 1
                    for num in l.split():
                        g.write(struct.pack('d', float(num)))
                g.seek(0, 0)
                g.write(struct.pack('LL', rows, cols))
    elif mode == 'scalar':
        s = 0.0
        with open(src, 'r') as f:
            s = float(f.readline())
        with open(dst, 'wb') as g:
            g.write(struct.pack('d', s))


if __name__ == '__main__':
    main()
