# Blanket Maker

Create blankets by converting an image into squares following a palette!

## C++
Dependencies:
- [Nlohmann's JSON library](https://github.com/nlohmann/json)
- [OpenCV](https://opencv.org/)

Build Instructions:
```bash
cmake .
make
```

Run With:
```bash
./blanketMaker IMAGE_FILE SIZE_X SIZE_Y
```

Example:
```bash
./blanketMaker rainbow.png 16 16
```

## Python

Dependencies:
- [Pillow](https://github.com/python-pillow/Pillow/)

## Example
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
