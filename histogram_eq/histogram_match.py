from __future__ import annotations

import bisect
import sys
from typing import List, Tuple

from PIL import Image


def get_min_max_color(cdf: List[int] | List[float]) -> Tuple[int, int]:
    """
    Find the minimum and maximum color values from CDF list.
    :param cdf: List of CDF values.
    :return: A tuple of minimum and maximum color values.
    """
    min_color = -1
    max_color = -1
    for i in range(256):
        if cdf[i] != 0 and min_color == -1:
            min_color = i
        if i < 255 and cdf[i] != cdf[i + 1]:
            max_color = i + 1
    assert min_color != -1 and max_color != -1
    return (min_color, max_color)


def get_equalized_color(cdf: float, min_cdf: float, max_cdf: float) -> int:
    """
    Calculate equalized color value from CDF values.
    :param cdf: Input color value's CDF.
    :param min_cdf: Minimum color value's CDF.
    :param max_cdf: Maximum color value's CDF.
    :return: Equalized color value.
    """
    return round((cdf - min_cdf) / (max_cdf - min_cdf) * 255)


if __name__ == '__main__':
    if len(sys.argv) < 4:
        print('Histogram Matcher by KSkun')
        print('Usage: python histogram_match.py <histogram file> <input file> <output file>')
        exit(1)

    histogram_file = sys.argv[1]
    in_file = sys.argv[2]
    out_file = sys.argv[3]

    # read the histogram to match
    to_cdf = []
    with open(histogram_file, 'r') as f:
        for i in range(256):
            line = f.readline()[:-1]
            if len(line) == 0:
                print('Invalid histogram, require 256 lines')
                exit(1)
            to_cdf.append(float(line))
    for i in range(1, 256):
        to_cdf[i] += to_cdf[i - 1]
    if to_cdf[255] < 0.99999 or to_cdf[255] > 1.00001:
        print('Invalid histogram, sum of all probabilities is not 1')
        exit(1)
    # get the equalized colors of the histogram to match
    to_min, to_max = get_min_max_color(to_cdf)
    rev_transform = {}  # reverse transformation
    for i in range(256):
        s_color = get_equalized_color(to_cdf[i], to_cdf[to_min], to_cdf[to_max])
        # clip color to [min, max]
        s_color = max(s_color, to_min)
        s_color = min(s_color, to_max)
        if s_color not in rev_transform.keys():
            rev_transform[s_color] = i

    image = Image.open(in_file)

    if image.mode != 'L':
        print('Only support grayscale image.')
        exit(1)

    # get histogram result
    cdf = [0] * 256
    for i in range(image.size[0]):
        for j in range(image.size[1]):
            color = image.getpixel((i, j))
            cdf[color] += 1
    # get cdf values
    for i in range(1, 256):
        cdf[i] += cdf[i - 1]
    # find min/max color values
    min_color, max_color = get_min_max_color(cdf)
    print('Debug: min %d, max %d' % (min_color, max_color))

    out_image = Image.new('L', image.size)
    for i in range(image.size[0]):
        for j in range(image.size[1]):
            color = image.getpixel((i, j))
            eq_color = get_equalized_color(cdf[color], cdf[min_color], cdf[max_color])
            if eq_color not in rev_transform.keys():
                keys = list(rev_transform.keys())
                idx = bisect.bisect_left(keys, eq_color)
                if idx >= len(keys):
                    idx -= 1
                eq_color = keys[idx]
            match_color = rev_transform[eq_color]
            out_image.putpixel((i, j), eq_color)

    print('Done!')
    out_image.save(out_file, 'PNG')
