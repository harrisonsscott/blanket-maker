import json
import math

from PIL import Image
import sys

# first parameter: image file
# second and third parameter: output image size
# fourth: upscale image for better visibility

# defaults
paletteFile = "palette.json"
outputImage = "bar.png"
textFile = "bar.txt"
upscaleImage = False

autoPalette = False
autoPaletteAmount = 0


def getSimilarity(color1, color2):  # calculates the similarity between 2 numbers
    return (
            0.3 * math.pow(color1[0] - color2[0], 2) +
            0.59 * math.pow(color1[1] - color2[1], 2) +
            0.11 * math.pow(color1[2] - color2[2], 2))


def hexToRGB(value):
    value = value.replace("#", "")
    return int(value[:2], 16), int(value[2:4], 16), int(value[4:6], 16)


def RGBToHex(color):
    return '#{:02x}{:02x}{:02x}'.format(color[0], color[1], color[2])


if len(sys.argv) < 4:
    print("main.py [image file] [output size x] [output size y] <--output outputFile> <--palette paletteFile> "
          "<--upscale true> <--textfile filename>")
    exit()

width, height = (int(sys.argv[2]), int(sys.argv[3]))

if len(sys.argv) > 4:
    if sys.argv[4].lower() == "true" or sys.argv[4] == "1":
        upscaleImage = True

if width <= 0 or height <= 0:
    print("Invalid dimensions")
    exit()

iarg = 4
while iarg < len(sys.argv):
    if iarg + 1 < len(sys.argv) and sys.argv[iarg] == "--palette":
        iarg += 1
        paletteFile = sys.argv[iarg]
        if paletteFile == "auto":
            autoPalette = True
            iarg += 1
            autoPaletteAmount = int(sys.argv[iarg])

    if iarg + 1 < len(sys.argv) and sys.argv[iarg] == "--upscale":
        iarg += 1
        if sys.argv[iarg].lower() == "true":
            upscaleImage = True
    if iarg + 1 < len(sys.argv) and sys.argv[iarg] == "--textfile":
        iarg += 1
        textFile = sys.argv[iarg]
    if iarg + 1 < len(sys.argv) and sys.argv[iarg] == "--output":
        iarg += 1
        paletteFile = sys.argv[iarg]
    iarg += 1

im = Image.open(sys.argv[1]).resize((width, height)).convert("RGB")
out = Image.new("RGBA", (width, height), 0xffffff)

if not autoPalette:
    with open(paletteFile, "r") as f:  # loads the palette
        data = json.load(f)
        palette = data["colors"]
else:
    palette = []
    for y in range(height):
        for x in range(width):
            r, g, b = im.getpixel((x, y))
            similarity = 100000
            for color in palette:
                if getSimilarity((r, g, b), hexToRGB(color)) < similarity:
                    similarity = getSimilarity((r, g, b), hexToRGB(color))
            if similarity > 800 and autoPaletteAmount > 0:
                autoPaletteAmount -= 1
                palette.append(RGBToHex((r, g, b)))
outputText = ""

index = 0
for color in palette:
    outputText += color + ": " + str(index) + "\n";
    index += 1

for y in range(height):
    outputText += "\nRow #" + str(y) + ": ";
    for x in range(width):
        r, g, b = im.getpixel((x, y))
        selectedColor = [0, 0, 0, 1000000]  # the 4th value is the similarity, not the alpha
        selectedIndex = 0
        for i in range(len(palette)):
            similarity = getSimilarity((r, g, b), hexToRGB(palette[i]))
            if selectedColor[3] > similarity:
                selectedColor[3] = similarity
                selectedColor[:3] = hexToRGB(palette[i])
                selectedIndex = i
        outputText += f"{str(selectedIndex)} "
        out.putpixel((x, y), (selectedColor[0], selectedColor[1], selectedColor[2], 255))

with open(textFile, "w") as f:
    f.write(outputText)

if not upscaleImage:
    out.save(outputImage)
else:
    aspect = height / width
    sizeX = math.floor(max(width * 16, 1024))
    sizeY = math.floor(sizeX * aspect)
    out.resize((sizeX, sizeY), resample=Image.BOX).save(outputImage)
