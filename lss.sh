#!/bin/sh

valid_filename()
{
    #Check if filename is a file
    if [ -f $1 ]; then
        return 1
    #Check if filename is a directory
    elif [ -d $1 ]; then
        return 1
    #Print error message if filename is an invalid path
    else
        echo invalid filename
        return 0
    fi
}

valid_short_arg()
{
    #Strip "-" from argument
    arg=`echo $1 | sed 's/^.//'`
    #Iterate through characters in argument
    #Check if argument is in approved list
    for i in $(seq 1 ${#arg}); do
        if [[ "${arg:i-1:1}" =~ (a|A|b|B|d|D|F|g|G|h|H|i|I|
                                \k|L|n|N|o|p|q|r|R|s|T|u|w) ]]; then
            #argument is valid
            break
        else
            echo ${arg:i-1:1} is invalid short arg
            return 0
        fi
    done
    return 1
}

valid_long_arg()
{
    #Strip "-" from argument
    arg=`echo $1 | sed 's/^..//'`
    #Check if argument is in approved list
    if [[ "$arg" =~ ^(all|almost-all|author|escape|block-size=([KMGTPEZY]|[0-9]*)|ignore-backups|\
            |color(=never|=auto|=always)|directory|dired|classify|file-type|full-time|\
            |group-directories-first|no-group|human-readable|si|dereference-command-line|\
            |dereference-command-line-symlink-to-dir|hide=.*|inode|ignore=.*|kibibytes|\
            |dereference|numeric-uid-guid|literal|indicator-style=slash|hide-control-chars|\
            |show-control-chars|quote-name|quoting-style=(literal|locale|shell|shell-always|c|\
            |escape)|reverse|recursive|size|time=(atime|access|use|ctime|status)|\
            |tabsize=[0-9]*|width=[0-9]*)$ ]]; then
        return 1
    else
        echo $arg is invalid long arg
        return 0
    fi
}

exit_if_invalid()
{
    #exit program if last function returns 0
    if [ $? -eq 0 ]; then
        echo exiting
        exit 1
    fi
}
        
call_ls()
{
    echo " "
    #If files are specified do not descend into directories
    if [ "$descend" == 0 ]; then
        #Call long-sorted ls with approved options, do not descend into directories
        ls -lSd "$@"
    else
        #Call long-sorted ls with approved options
        ls -lS "$@"
    fi
    echo " "
}

check_arguments()
{
    #iterate through arguments supplied to lss
    for var in "$@"; do
        #Check if argument starts with "-"
        if [[ $var == -[!-]* ]]; then
            #Check if short arguments are valid
            valid_short_arg $var
            exit_if_invalid
        #Check if argument starts with "--"
        elif [[ $var == --* ]]; then
            #Check if long arguments are valid
            valid_long_arg $var
            exit_if_invalid
        #If not an option to ls, assume a filename
        else
            #Check if filename is a valid path
            valid_filename $var
            exit_if_invalid
            descend=0;
        fi
    done
}

main()
{
    #Change to 0 if file arguments are given        
    descend=1

    check_arguments "$@"

    call_ls "$@"
}

main "$@"

