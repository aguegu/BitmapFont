#! /bin/bash

echo $1 > /tmp/utf8_$$.txt

iconv -f UTF-8 -t GBK /tmp/utf8_$$.txt -o /tmp/gbk_$$.txt

#text=$(hexdump -C /tmp/gbk_$$.txt)
#text=$(dd if=/tmp/gbk_$$.txt ibs=2 count=1 2>/dev/null)

length=$(cat /tmp/gbk_$$.txt | wc -c)
length=$((length - 1))

#hexdump -C /tmp/gbk_$$.txt

#x=$(od -t x1 -N 1 /tmp/gbk_$$.txt | head -1 | awk '{print $2}'
#echo $((16#$x))
#y=$(od -t x1 -N 1 -j 1 /tmp/gbk_$$.txt | head -1 | awk '{print $2}')
#echo $((16#$y))

./SearchFont font/HZK48 /tmp/gbk_$$.txt

rm /tmp/utf8_$$.txt
rm /tmp/gbk_$$.txt
#rm /tmp/code_$$.txt

