# Blanket Maker

Create blankets by converting an image into squares following a palette!

## C++
Dependencies:
- [Nlohmann's JSON library](https://github.com/nlohmann/json)
- [OpenCV](https://opencv.org/)

```bash
sudo apt install libopencv-dev
```

Build Instructions:
```bash
cd cpp
cmake .
make
```

Run With:
```bash
./blanketMaker [output size x] [output size y] <--image imageFile> <--output outputFile> <--palette paletteFile> <--upscale true/false> <--textfile filename>
```
For both c++ and python, you can also use "auto" instead of specifying a json file for the palette parameter followed by the palette size to automatically generate a palette.

Example
```bash
./blanketMaker 10 10 --rainbow.png --palette auto 8
```

## Python

Dependencies:
- [Pillow](https://github.com/python-pillow/Pillow/)

Run With:
```bash
python3 main.py [output size x] [output size y] <--image imageFile> <--output outputFile> <--palette paletteFile> <--upscale true/false> <--textfile filename>
```


## Example
```bash
./blanketMaker 10 10 --image rainbow.png --upscale true --output out.png
```

rainbow.png (original)

![][original]

out.png (output)

![][output]

Using the palette:
```json
{
	"colors": [
		"#ff0000",
		"#ffff00",
		"#00ff00",
		"#00ffff",
		"#0000ff"
	]
}
```

A text file is also made that makes it easier to follow the colors when making the blanket.
```txt
#ff0000: 0
#ffff00: 1
#00ff00: 2
#00ffff: 3
#0000ff: 4

Row #0: 2 2 2 3 4 4 4 4 4 4 
Row #1: 1 2 2 2 2 4 4 4 4 4 
Row #2: 1 1 1 2 2 2 3 4 4 4 
Row #3: 1 1 1 1 2 2 2 3 4 4 
Row #4: 1 1 1 1 1 2 2 2 3 4 
Row #5: 1 1 1 1 1 1 2 2 2 3 
Row #6: 1 1 1 1 1 1 1 2 2 2 
Row #7: 0 1 1 1 1 1 1 1 2 2 
Row #8: 0 0 1 1 1 1 1 1 1 2 
Row #9: 0 0 0 1 1 1 1 1 1 1 
```

[original]: <https://media.discordapp.net/attachments/1202064667799724065/1202067164140408903/rainbow.png>
[output]: <https://media.discordapp.net/attachments/1202064667799724065/1202067164442669086/bar.png>
