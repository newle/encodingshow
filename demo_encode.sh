

if [ $# -ne 2 ]; then
	echo "example: sh demo_encode.sh ÄãºÃ°¡ `date +\"%s\"`"
	exit
fi

text="$1"
prefix="$2"

./encoding raw "$text" > $prefix"norm.log"

echo $text"	"$prefix >> encoding_process.log




