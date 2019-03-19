#! /bin/sh

EOKAY=0
EMSG=0
EINFO=0
EDEBUG=1
EEFAULT=2
EABORT=3
EERROR=4
LANG="en_US.ISO8859-1"
PROGTOOL_REPO_URIS="https://github.com/gittup/tup.git https://github.com/libusb/libusb.git"
PROGTOOL_BUILD_ROOT=`pwd`
progtool_root_path="$PROGTOOL_BUILD_ROOT"
progtool_repo_urls=""
progtool_build_path=""
progtool_repo_uris="https://github.com/vendu/OS-Zero.git"
progtool_repo_list=""
progtool_option_flags=""
progtool_build_opt=0
progtool_clean_opt=0
progtool_default_opt=0
progtool_verbose_opt=0
progtool_force_opt=0
progtool_dry_run_opt=0
progtool_quiet_opt=0
progtool_help_opt=0
progtool_info_opt=1
progtool_info_opt=1

PROGTOOL_DEST_DIRS="bin sbin lib libexec man etc com var include share info locale man"

function progtool_usage()
{
    if [[ $progtool_help_opt -ne 0 ]]; then
	echo "invalid command: $@"
    fi
    echo "progtool	[options] [paths]"
    echo ""
    echo "options"
    echo "-------"
    echo "-B, --build		build programs"
    echo "-C, --clean		remove empty build directories"
    echo "-D, --default		build default packages"
    echo "-v, --verbose		verbose mode"
    echo "-f, --force		force operations"
    echo "-n, --dry-run		list what would be done without executing commands"
    echo "-q, --quiet		quiet, no output messages"
    echo "-h, --help		this help screen"
    echo "-U, --uri		repostitory to build"
    echo "-I, --info		build information screen"
    echo "-E, --exec-prefix	exec prefix (e.g. with -X)"
    echo "-X, --cross-build	cross-build, i.e. cross-compilation"

    exit 0
}

function progtool_debug_event()
{
    msg="$1"
    lvl="$2"
    func="$3"
    arg="$4"
    val=""

    if [[ -n "$func" ]]; then
	if [[ -n "$arg" ]]; then
	    $('"$func"' $arg)
	else
	    $('"$func"')
	fi
	eval val="'$?'"
    fi
    if [ -n "$val" ]] && [[  "$val" == "$lvl" ]]; then
	    echo "$msg"
    else
	case $val in
	    0)
		echo "$msg"
		val=$EOKAY
		;;
  	    msg)	    
		echo "$msg"
		val=$EOKAY
		;;
	    info)
		echo "INFO	$msg"
		val=$EINFO
		;;
	    warn)
		echo "WARN	$msg"
		val=$EMSG
		;;
	    debug)
		echo "DEBUG	$msg"
		val=$EDEBUG
		;;	
	    fault)
		echo "FAULT	$msg"
		val=$EFAULT
		;;
	    abort)
		echo "ABORT	$msg"
		exit $EABORT
		;;
	    error)
		echo "ERROR	$msg"
		exit $EERROR
		;;
	esac
    fi
    eval ret='"$val"'

    return $ret
}

function progtool_print_msg()
{
    ret=0
    shift
    str=$@

    if [[ $progtool_dry_run_opt -ne 0 ]]; then
	echo "$str"
    elif [[ $progtool_verbose_opt -ne 0 ]]; then
	echo "$str"
    elif [[ $progtool_quiet_opt -ne 0 ]]; then
	echo "$str" >> progtool.log
    fi
    eval ret="'$?'"
    
    return $ret
}

function progtool_get_opts()
{
    while [ $# -gt 0 ]
    do
	case $1 in
	    -B | --build)
		progtool_build_opt=1
		progtool_option_flags="$progtool_option_flags B"
		shift
		;;
	    -C | --clean)
		progtool_clean_opt=1
		progtool_option_flags="$progtool_option_flags C"
		shift
		;;
	    -D | --default)
		progtool_default_opt=1
		progtool_option_flags="$progtool_option_flags D"
		shift
		;;
	    -v | --verbose)
		progtool_verbose_opt=1
		progtool_option_flags="$progtool_option_flags V"
		shift
		;;
	    -f | --force)
		progtool_force_opt=1
		progtool_option_flags="$progtool_option_flags f"
		shift
		;;
	    -n | --dry-run)
		progtool_dry_run_opt=1
		progtool_option_flags="$progtool_option_flags n"
		shift
		;;
	    -q | --quiet)
		progtool_quiet_opt=1
		progtool_option_flags="$progtool_option_flags Q"
		shift
		;;
	    -h | --help)
		progtool_help_opt=1
		progtool_usage $@
		shift
		;;
	    -U | --uri | --url)
		shift
		progtool_uri_opts="$progtool_uri_opts $1"
		progtool_option_flags="$progtool_option_flags U"
		shift
		;;
	    -I | --info)
		progtool_info_opt=1
		progtool_option_flags="$progtool_option_flags I"
		shift
		;;
	    -E | --exec-prefix)
		shift
		progtool_exec_prefix="$1"
		progtool_option_flags="$progtool_option_flags E"
		shift
		;;
	    -X | --cross-build)
		progtool_cross_build_opt=1
		progtool_option_flags="$progtool_option_flags X"
		shift
		;;
	    *)
		if [[ -d "$1" ]]; then
		    if [[ "$progtool_root_path" == "$PROGTOOL_BUILD_ROOT" ]];
		    then
			progtool_root_path="$1"
			progtool_option_flags="$progtool_option_flags r"
		    fi
		else
		    progtool_uri_opts="$progtool_uri_opts $1"
		    progtool_option_flags="$progtool_option_flags u"
		fi
		shift
		;;
	esac
    done
     
    return
}

function progtool_run_cmd()
{
    cmd="$1"

    if [[ $progtool_dry_run_opt -ne 0 ]]; then
	echo "$cmd"
    elif [[ $progtool_quiet_opt -ne 0 ]]; then
	$cmd 2> /dev/null
    else
	if [[ $progtool_verbose_opt -ne 0 ]]; then
	    echo "$cmd"
	fi
	$cmd
    fi
}

function progtool_create_repo_list()
{
    root="$progtool_root_path"
    uris=$@

    cd "$root"
    echo "ROOT: $root, URIS: $uris"
    if [[ -n "$uris" ]]; then
	for uri in $uris
	do
	    path=`basename $uri`
	    dir=`echo $path | sed -e 's/'"\.git"'\$//'`
	    progtool_repo_list="$progtool_repo_list $uri"
	    progtool_repo_dirs="$progtool_repo_dirs $dir"
	done
    fi
    cd "$root"

    return
}

function progtool_clean_dirs()
{
    root="$progtool_root_path"
    dirs=$@

    echo "CLEAN: $dirs"
    cd "$root"
    if [[ -n "dirs" ]]; then
	for dir in $dirs
	do
	    progtool_run_cmd "rm -rf $dir"
	done
    fi
    cd "$root"

    return
}

function progtool_remove_dirs()
{
    root="$progtool_root_path"
    dirs=$@

    echo "CLEAN: $dirs"
    cd "$root"
    if [[ -n "dirs" ]]; then
	for dir in $dirs
	do
	    progtool_run_cmd "rmdir $dir"
	done
    fi
    cd "$root"

    return
}

function progtool_build_progs()
{
    root="$progtool_root_path"
    dirs=$@
    ret=0

    cd "$root"
    if [[ -n $dirs ]]; then
	for dir in $dirs
	do
	    progtool_print_msg "error building $progtool_root_path/$dir"
	    progtool_build_auto "$dir"
	    eval ret="'$?'"
	done
    fi
    cd "$root"

    return $ret
}

function git_update_repo()
{
    uri="$1"
    dir="$2"
    ret=0

    progtool_print_msg "cloning $uri into $progtool_root_path/$dir"
    git clone "$uri"
    eval ret="'$?'"
    if [[ $ret -ne 0 ]] && [[ -d "$dir" ]]; then
	progtool_print_msg "updating $progtool_root_path/$dir"
	cd "$dir"
	git pull
	eval ret="'$?'"
	if [[ $ret -ne 0 ]]; then
	    progtool_print_msg "failed to update $uri in $progtool_root_path/$dir"
	fi
    fi
    
    return $ret
}

function hg_update_repo()
{
    root="$progtool_root_path"
    uri="$1"
    dir="$2"
    ret=0
    
    progtool_print_msg "cloning $uri in $progtool_root_dir/$path"
    hg clone "$uri"
    eval ret=="'$?'"
    if [[ $ret -ne 0 ]] && [[ -d "$dir" ]]; then
	cd "$dir"
	progtool_print_msg "updating $progtool_root_path/$dir"
	hg pull
	eval ret="'$?'"
	if [[ $ret -ne 0 ]]; then
	    progtool_print_msg "failed to update $uri in $progtool_root_path/$dir"
	fi
    fi
 
    return $ret
}

function progtool_update_repos()
{
    root="$progtool_root_path"
    uris="$@"
    ret=0

    for uri in $uris
    do
	path=`basename $uri`
	dir=`echo $path | sed -e 's/'"\.git"'\$//p'`
	cd "$root"
	git_update_repo "$uri" "$dir"
	eval ret="'$?'"
	cd "$root"
	if [[ $ret -ne 0 ]] && [[ -d "$dir" ]]; then
	    hg_update_repo "$uri" "$dir"
	    eval ret="'$?'"
	    cd "$root"
	    if [[ $ret -ne 0 ]]; then
		progtool_print_msg "failed to update $uri in $progtool_root_path/$dir"
	    fi
	fi
    done
    cd "$root"
    
    return $ret;
}

function progtool_build_auto()
{
    root="$progtool_root_path"
    dirs="$@"
    docdir="$progtool_root_path/doc/$pkg"
    ret=0

    cd "$root"
    if [[ -d "$dir" ]]; then
	cd "$dir"
    else
	eval ret="'$?'"

	return $ret
    fi
    if [[ $progtool_quiet_opt -ne 0 ]]; then
	configure_opts="-q"
    fi
    if [[ $progtool_dry_run_opt -ne 0 ]]; then
	configure_opts="-n"
    fi
    ret=1
    if [[ -x "bootstrap.sh" ]]; then
	prog="bootstrap.sh"
	./bootstrap.sh
	eval ret="'$?'"
    fi
    if [[ $ret -ne 0 ]]; then
	if [[ -x "autogen.sh" ]]; then
	    prog="autogen.sh"
	    ./autogen.sh
	    eval ret="'$?'"
	fi
    fi
    if [[ $ret -ne 0 ]]; then
	prog="autoreconf"
	autoreconf -i
    fi
    eval ret="'$?'"
    if [[ $ret -ne 0 ]]; then
	progtool_print_msg "$prog failed"
    else
	./configure $configure_opts --prefix="$progtool_root_path" --exec-prefix="$progtool_root_path" --program-prefix="$pkg" --includedir="$progtool_root_path/include" --oldincludedir="$progtool_root_path/usr/include" --datarootdir="$progtool_root_path/share" --datadir="$progtool_root_path/share" --infodir="$progtool_root_path/info" --localedir="$progtool_root_path/share" --mandir="$PROGTOOL_ROOT/man" --docdir="$docdir" --htmldir="$docdir" --dvidir="$docdir" --pdfdir="$docdir" --psdir="$docdir"
	eval ret="'$?'"
	if [[ $ret -ne 0 ]]; then
	    progtool_print_msg "configure failed"
	else
	    make
	    eval ret="'$?'"
	    if [[ $ret -ne 0 ]]; then
		progtool_print_msg "make failed"
	    else
		make install
		eval ret="'$?'"
		if [[ $ret -ne 0 ]]; then
		    progtool_print_msg "make install failed"
		fi
	    fi
	fi
    fi
    cd "$root"

    return $ret
}

function progtool_print_info()
{
    uris="$progtool_repo_list"
    echo "package repositories"
    echo "--------------------"
    for uri in $uris
    do
	echo "	$uri"
    done
    dirs="$progtool_repo_dirs"
    echo "package directories"
    echo "-------------------"
    for dir in $dirs
    do
	echo "	$dir"
    done

    return
}

function progtool_main()
{
    ret=0
    
    progtool_get_opts $@
    progtool_create_repo_list "$PROGTOOL_REPO_URIS"
    progtool_create_repo_list "$progtool_repo_uris"
    if [[ $progtool_build_opt -ne 0 ]]; then
	progtool_update_repos "$progtool_repo_list"
	progtool_build_progs "$progtool_repo_dirs"
	eval ret="'$?'"
    fi
    if [[ $progtool_clean_opt -ne 0 ]]; then
	progtool_clean_dirs "$progtool_repo_dirs"
	if [[ $progtool_force_opt -ne 0 ]]; then
	    progtool_clean_dirs "$PROGTOOL_DEST_DIRS"
	else
	    progtool_remove_dirs "$PROGTOOL_DEST_DIRS"
	fi
	eval ret="'$?'"
    fi
    progtool_print_info "$progtool_repo_list"
    progtool_print_info "$progtool_repo_dirs"
    
    exit $ret
}

progtool_main $@

