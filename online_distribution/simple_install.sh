#! /bin/sh

if [ -z "$1" ] ; then

    echo "usage: $0 path-to-install-area"
    echo "  e.g. $0  \$HOME/install"
    exit
fi

here=`pwd`
main=$(dirname "$0")
echo $main 
install="$1"

LIST="newbasic pmonitor"

# do we have lzo installed system-wide?
if [ ! -e /usr/include/lzo ] ; then
    LIST="lzo $LIST"
fi


for area in $LIST ; do

    [ -d "$area" ] || mkdir "$area"
    cd "$area"

  # now see if we have a relative path or not

    if  echo "$main" | grep -q -e '^\.\.' ; then
	../"$main"/"$area"/autogen.sh --prefix="$install"
    else
	"$main"/"$area"/autogen.sh --prefix="$install"
    fi

    make install

    cd "$here"

done


