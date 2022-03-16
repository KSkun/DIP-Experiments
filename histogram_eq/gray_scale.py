import sys

from PIL import Image

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print('Gray Scale Converter by KSkun')
        print('Usage: python gray_scale.py <input file> <output file>')
        exit(1)

    in_file = sys.argv[1]
    out_file = sys.argv[2]
    image = Image.open(in_file)

    if image.mode == 'L':
        image.save(out_file, 'PNG')
        print('Input picture is already a grayscale one, did nothing.')
        exit(0)

    if image.mode != 'RGB':
        print('Only support RGB mode image.')
        exit(1)

    gray_image = Image.new('L', image.size)
    for i in range(image.size[0]):
        for j in range(image.size[1]):
            color = image.getpixel((i, j))
            # Grayscale = 0.299R + 0.587G + 0.114B
            gray_color = color[0] * 0.299 + color[1] * 0.587 + color[2] * 0.114
            gray_image.putpixel((i, j), round(gray_color))

    print('Convert success!')
    gray_image.save(out_file, 'PNG')
