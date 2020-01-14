#! /bin/sh

vsht_opt_debug_vsht=0
vsht_opt_analyze=0
vsht_opt_profile=0
vsht_opt_verbose=0
vsht_opt_cc=""
vsht_opt_as=""
vsht_opt_ld=""

vsht_export_opts()
{
    export vsht_opt_debug_vsht
    export vsht_opt_analyze
    export vsht_opt_profile
    export vsht_opt_verbose
    export vsht_opt_cc
    export vsht_opt_as
    export vsht_opt_ld

    return
}

vsht_get_opts()
{
    for arg
    do
	case $arg in
	    -D | --vsht-debug)
		vsht_opt_debug_vsht=1
		;;
	     -a | --analyze)
		 vsht_opt_analyze=1
		 ;;
	     -h | --help)
		 vsht_print_usage

		 exit 0
		 ;;
	     -p | --profile)
		 vsht_opt_profile=1
		 ;;
	     -v | --verbose)
		 vsht_opt_verbose=1
		 ;;
	     --cc=*)
		 vsht_opt_cc=`echo ${arg} | cut -f 2 -d\=`
		 ;;
	     --as=*)
		 vsht_opt_as=`echo ${arg} | cut -f 2 -d\=`
		 ;;
	     --ld=*)
		 vsht_opt_ld=`echo ${arg} | cut -f 2 -d\=`
		 ;;
	     *)
		 vsht_print_usage

		 exit 1
		 ;;
	esac
    done
#    vsht_export_opts

    return
}

vsht_print_opts()
{
    echo -e "build parameters"
    echo -e "----------------"
    echo -e "analyze\t\t$vsht_opt_analyze"
    echo -e "profile\t\t$vsht_opt_profile"
    echo -e "compiler\t$CC"
    echo -e "assembler\t$AS"
    echo -e "linker\t\t$LD"

    return
}

