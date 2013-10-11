#! /bin/sh

prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix="$prefix/bin"
exec_prefix_set=no

usage="\
Usage: libmikmod-config [--prefix[=DIR]] [--exec-prefix[=DIR]] [--version] [--libs] [--cflags] [--ldadd]"

if test $# -eq 0 ; then
	echo "$usage" 1>&2
	exit 1
fi

while test $# -gt 0 ; do
	case "$1" in
	-*=*) optarg=`echo "$1" | sed 's/[-_a-zA-Z0-9]*=//'` ;;
	*) optarg= ;;
	esac

	case $1 in
	--prefix=*)
		prefix=$optarg
		if test $exec_prefix_set = no ; then
			exec_prefix=$optarg
		fi
	;;
	--prefix)
		echo $prefix
	;;
	--exec-prefix=*)
		exec_prefix=$optarg
		exec_prefix_set=yes
	;;
	--exec-prefix)
		echo $exec_prefix
	;;
	--version)
		echo @LIBMIKMOD_VERSION@
	;;
	--cflags)
		includedir="$prefix/include"
		if test $includedir != /usr/include ; then
			includes=-I$includedir
		fi
		echo $includes @REENTRANT@
	;;
	--ldadd)
		echo @LIB_LDADD@
	;;
	--libs)
		libdir=-L"$prefix/lib"
		echo $libdir -lmikmod @LIBRARY_LIB@
	;;
	*)
		echo "${usage}" 1>&2
		exit 1
	;;
	esac
	shift
done

