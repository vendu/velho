#! /bin/sh

vsht_print_usage()
{
    echo -e "$0 usage"
    echo -e "\t-a, --analyxe\tenable static analysis"
    echo -e "\t-p, --profile\tenable profiling"-
    echo -e "\t-v, --verbose\tverbose messages"
    echo -e "\t--cc=CMD\tuse CMD as the compiler"
    echo -e "\t--as=CMD\tuse CMD as the assembler"
    echo -e "\t--ld=CMD\tuse CMD as the linker"
}

vsht_print_env()
{
    if [ "$vsht_opt_verbose" -ne 0 ]; then
	vsht_print_opts
	vsht_print_conf
    fi
}

set -x
vsht_print_env

