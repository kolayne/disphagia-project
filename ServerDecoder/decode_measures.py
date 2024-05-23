#!/usr/bin/env python3
from typing import List
from struct import calcsize, unpack


# Description of the `Measure` structure according to
# https://docs.python.org/3/library/struct.html#format-characters
# The less-than character in the beginning indicates the little-endian byte order.
struct_format = '< f fff f i'  # Note that spaces have no semantic effect


def decode_measures(payload: bytes, fmt: str = struct_format) -> List[tuple]:
    size = calcsize(struct_format)
    if len(payload) % size != 0:
        raise ValueError("Struct size %d does not divide the payload length %d" % (size, len(payload)))

    ans = []
    for i in range(len(payload) // size):
        ans.append(unpack(fmt, payload[i*size:(i+1)*size]))
    return ans


if __name__ == '__main__':
    from sys import stdin
    print(decode_measures(stdin.buffer.read()))
