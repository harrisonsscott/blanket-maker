import json
import math

from PIL import Image
import sys

# first parameter: image file
# second and third parameter: output image size
# fourth: upscale image for better visibility
# main.py image.png 10 10

with open("palette.json", "r") as f:  # loads the palette
    data = json.load(f)
    palette = data["colors"]


def getSimilarity(color1, color2):  # calculates the similarity between 2 numbers
    return (
            0.3 * math.pow(color1[0] - color2[0], 2) +
            0.59 * math.pow(color1[1] - color2[1], 2) +
            0.11 * math.pow(color1[2] - color2[2], 2))


def hexToRGB(value):
    value = value.replace("#", "")
    return int(value[:2], 16), int(value[2:4], 16), int(value[4:6], 16)


hexToRGB("#ff7700")

if len(sys.argv) < 4:
    print("main.py [image file] [output size x] [output size y] [upscale image]")
    exit()

width, height = (int(sys.argv[2]), int(sys.argv[3]))
upscaleImage = False

if len(sys.argv) > 4:
    if sys.argv[4].lower() == "true" or sys.argv[4] == "1":
        upscaleImage = True

if width <= 0 or height <= 0:
	print("Invalid dimensions")
	exit()

im = Image.open(sys.argv[1]).resize((width, height)).convert("RGB")
out = Image.new("RGBA", (width, height), 0xffffff)

for x in range(width):
    for y in range(height):
        r, g, b = im.getpixel((x, y))
        selectedColor = [0, 0, 0, 1000000]  # the 4th value is the similarity, not the alpha
        for color in palette:
            similarity = getSimilarity((r, g, b), hexToRGB(color))
            if selectedColor[3] > similarity:
                selectedColor[3] = similarity
                selectedColor[:3] = hexToRGB(color)
        out.putpixel((x, y), (selectedColor[0], selectedColor[1], selectedColor[2], 255))

if not upscaleImage:
    out.save('output.png')
else:
    aspect = width/height
    sizeX = math.floor(max(width * 16, 1024))
    sizeY = math.floor(sizeX * aspect)
    out.resize((sizeX, sizeY), resample=Image.BOX).save("output.png")

