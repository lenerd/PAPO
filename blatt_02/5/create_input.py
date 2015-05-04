#!/usr/bin/env python3
import random
import sys


def create_matrix(path, rows, cols):
    with open(path, 'w') as f:
        f.write('{} {}\n'.format(rows, cols))
        for i in range(rows):
            f.write(' '.join(str(random.randint(0, 10000))
                             for j in range(cols)))
            f.write('\n')


def usage():
    print("python create_input.py 1337 42 filename")


def main():
    if len(sys.argv) < 4:
        usage()
        return
    else:
        create_matrix(sys.argv[3], int(sys.argv[1]), int(sys.argv[2]))


if __name__ == '__main__':
    main()
