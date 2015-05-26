

if [ $# -ne 2 ]; then
	echo "example: sh demo_encode.sh ÄãºÃ°¡ `date +\"%s\"`"
	echo "example: sh demo_encode.sh "utf16\#\#\#\#0xFEFF 0x5E05 0x54E5" `date +\"%s\"`"
	exit
fi

text="$1"
prefix="$2"

if [ "${text%%#*}" == "${text##*#}" ]; then 
	./encoding raw "$text" > $prefix"norm.log"
else 
	./encoding "${text%%#*}" "${text##*#}" > $prefix"norm.log"
fi

echo $text"	"$prefix >> encoding_process.log




