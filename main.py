from PIL import Image
import sys

# first parameter: image file
# second and third parameter: output image size
# main.py image.png 10 10

if len(sys.argv) < 4:
    print("main.py [image file] [output size x] [output size y]")
    exit()

width, height = (int(sys.argv[2]), int(sys.argv[3]))

im = Image.open(sys.argv[1]).resize((width, height))
out = Image.new("RGBA", (width, height), 0xffffff)

for x in range(width):
    for y in range(height):
        r, g, b, a = im.getpixel((x, y))
        out.putpixel((x, y), (r, g, b, a))


out.save('bar.png')
