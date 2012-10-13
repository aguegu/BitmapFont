BitmapFont
==========

decode bitmap fonts (in font/), as 

	ASC12	ASC16 	ASC48
	HZK12
	HZK14
	HZK16
	HZK16F 	HZK16S
	HZK24F	HZK24H	HZK24K	HZK24S	HZK24T
	HZK32
	HZK40S	HZK40T
	HZK48	HZK48S	HZK48T

print out info header, hex output, pattern view for each to character in a bitmap font file. hope to help programming character display stored in arrays, especially for micro controller system.
------

$ make 

to build

------

$ make clean

to cleanup

------

$ make font_name

ex. make ASC12, make HZK32
to get the pattern list text in UTF-8 of the font_name Font in output/, named font_name.txt

-----

$ make all

to all pattern list (UTF-8) for all the fonts in output/
