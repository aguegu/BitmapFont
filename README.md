#BitmapFont

Print out info header, hex output, pattern view for each to character in bitmap font files list below. Hope to help programming character display stored in arrays, especially for micro controller system.

blog related: http://aguegu.net/?p=1279

fonts include:

	ASC12	ASC16 	ASC48
	HZK12
	HZK14
	HZK16	HZK16F 	HZK16S
	HZK24F	HZK24H	HZK24K	HZK24S
	HZK32
	HZK40
	HZK48

##Download
	$ git clone git://github.com/aguegu/BitmapFont.git

##Build
	$ make 

##Use
Get the font_name Font pattern list text in UTF-8 of the in output/, named font_name.txt

	$ make font_name

ex. make ASC12, make HZK32

Get pattern lists (UTF-8) for all the fonts in output/

	$ make allfonts

##Cleanup
	$ make clean

##License
	BitmapFont is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	BitmapFont is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

	Author: Weihong Guan (@aGuegu)
	E-mail: weihong.guan[at]gmail.com
	Host: https://github.com/aguegu/BitmapFont
