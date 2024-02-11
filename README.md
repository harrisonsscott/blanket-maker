# Blanket Maker

Create blankets by converting an image into squares following a palette!

## C++
Dependencies:
- [Nlohmann's JSON library](https://github.com/nlohmann/json)
- [OpenCV](https://opencv.org/)

```bash
sudo apt install libopencv-dev python3-opencv
```

Build Instructions:
```bash
cd cpp
cmake .
make
```

Run With:
```bash
./blanketMaker IMAGE_FILE SIZE_X SIZE_Y UPSCALE_IMAGE
```

## Python

Dependencies:
- [Pillow](https://github.com/python-pillow/Pillow/)

Run With:
```bash
python3 main.py IMAGE_FILE SIZE_X SIZE_Y UPSCALE_IMAGE
```


## Example
```bash
./blanketMaker rainbow.png 10 10 true
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
