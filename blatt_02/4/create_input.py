import random


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


def main():
    # create_vector('vector', 10000)
    create_matrix('matrix_big', 10000, 10000)
    # create_matrix('matrix', 1000, 1000)


if __name__ == '__main__':
    main()
