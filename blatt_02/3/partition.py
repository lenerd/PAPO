def partition(n, p):
    base_len = n // p
    rest = n % p
    for rank in range(p):
        start = rank * base_len
        if rank < rest:
            start += rank
        else:
            start += rest
        end = (rank + 1) * base_len
        if rank + 1 < rest:
            end += rank + 1
        else:
            end += rest
        print(rank, start, end, end - start)


def main():
    partition(15, 4)


if __name__ == '__main__':
    main()
