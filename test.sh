set -e
exec {tcp}<> /dev/tcp/localhost/$1
cat - <&$tcp &
#cat src/commands.txt >&$tcp
cat - >&$tcp
