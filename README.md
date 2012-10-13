#BitmapFont

Print out info header, hex output, pattern view for each to character in bitmap font files list below. Hope to help programming character display stored in arrays, especially for micro controller system.

	ASC12	ASC16 	ASC48
	HZK12
	HZK14
	HZK16
	HZK16F 	HZK16S
	HZK24F	HZK24H	HZK24K	HZK24S	HZK24T
	HZK32
	HZK40S	HZK40T
	HZK48	HZK48S	HZK48T

##Download
	$ git clone git://github.com/aguegu/BitmapFont.git

##Build
	$ make 

##Use
Get the font_name Font pattern list text in UTF-8 of the in output/, named font_name.txt

	$ make font_name

ex. make ASC12, make HZK32

Get pattern lists (UTF-8) for all the fonts in output/

	$ make all

##Cleanup
	$ make clean

