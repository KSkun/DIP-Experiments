import sys

from PIL import Image

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print('Histogram Equalizer by KSkun')
        print('Usage: python histogram_equal.py <input file> <output file>')
        exit(1)

    in_file = sys.argv[1]
    out_file = sys.argv[2]
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
    min_color = 255
    max_color = 0
    for i in range(256):
        if cdf[i] != 0:
            if i < min_color:
                min_color = i
            if i > max_color:
                max_color = i
    print('Debug: min %d, max %d' % (min_color, max_color))

    out_image = Image.new('L', image.size)
    for i in range(image.size[0]):
        for j in range(image.size[1]):
            color = image.getpixel((i, j))
            eq_color = round((cdf[color] - cdf[min_color]) / (cdf[max_color] - cdf[min_color]) * 255)
            out_image.putpixel((i, j), eq_color)

    print('Done!')
    out_image.save(out_file, 'PNG')
