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
./blanketMaker [image file] [output size x] [output size y] <--output outputFile> <--palette paletteFile> <--upscale true/false> <--textfile filename>
```
You can also use "auto" instead of specifying a json file for the palette parameter to automatically generate a palette.

## Python

Dependencies:
- [Pillow](https://github.com/python-pillow/Pillow/)

Run With:
```bash
python3 main.py [image file] [output size x] [output size y] <--output outputFile> <--palette paletteFile> <--upscale true/false> <--textfile filename>
```


## Example
```bash
./blanketMaker rainbow.png 10 10 --upscale true
```

Original

![][original]

After

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

[original]: <https://media.discordapp.net/attachments/1202064667799724065/1202067164140408903/rainbow.png>
[output]: <https://media.discordapp.net/attachments/1202064667799724065/1202067164442669086/bar.png>
