import random
import sys


def create_vector(path, size):
    with open(path, 'w') as f:
        f.write('{}\n'.format(size))
        for i in range(size):
            f.write('{}\n'.format(random.randint(0, 10000)))


def create_matrix(path, rows, cols):
    with open(path, 'w') as f:
        f.write('{} {}\n'.format(rows, cols))
        for i in range(rows):
            f.write(' '.join(str(random.randint(0, 10000))
                             for j in range(cols)))
            f.write('\n')


def usage():
    print("python create_input.py matrix 1337 42 filename")
    print("python create_input.py vector 1337 filename")


def main():
    if len(sys.argv) < 2:
        usage()
        return
    if sys.argv[1] == 'matrix':
        if len(sys.argv) < 5:
            usage()
            return
        else:
            create_matrix(sys.argv[4], int(sys.argv[2]), int(sys.argv[3]))
    elif sys.argv[1] == 'vector':
        if len(sys.argv) < 4:
            usage()
            return
        else:
            create_vector(sys.argv[3], int(sys.argv[2]))
    else:
        usage()


if __name__ == '__main__':
    main()
