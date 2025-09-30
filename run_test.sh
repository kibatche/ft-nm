#!/bin/sh

# Colors
RED="\e[31m"
GREEN="\e[32m"
YELLOW="\e[33m"
BLUE="\e[34m"
MAGENTA="\e[35m"
CYAN="\e[36m"
ENDCOLOR="\e[0m"

# Paths to the programs
NM="/usr/bin/nm"
FT_NM="./ft_nm"

# Test folders
BASE_FOLDER="./test_binaries"
BIN_FOLDER="$BASE_FOLDER/bin"
ERR_FOLDER="$BASE_FOLDER/err"
LIB_FOLDER="$BASE_FOLDER/lib"
OBJ_FOLDER="$BASE_FOLDER/obj"
RES_FOLDER="$BASE_FOLDER/results"

# Function to run tests
run_tests() {
    local folder=$1
    if [ ! -d "$folder" ]; then
        echo "${RED}Folder not found: ${YELLOW}$folder${ENDCOLOR}"
        return
    fi

    for file in "$folder"/*; do
        if [ -f $file ]; then
            test_file "$file"
        fi
    done
}

run_error_test() {
    local folder=$1
    if [ ! -d "$folder" ]; then
        echo "${RED}Folder not found: ${YELLOW}$folder${ENDCOLOR}"
        return
    fi

    echo "${RED}--------------------------------------------------${ENDCOLOR}"
    echo "${RED}--------------------WARNING-----------------------${ENDCOLOR}"
    echo "${RED}--------------------------------------------------\n${ENDCOLOR}"
    echo "For the error handling the result are not diff between nm and ft_nm."
    echo "The result may not be the same but they need to be coherent.\n"
    for file in "$folder"/*; do
        if [ -f $file ]; then
            echo "\n${BLUE}Testing file: ${YELLOW}$file${ENDCOLOR}\n"
            echo "${CYAN}NM:${ENDCOLOR}"
            $NM "$file"
            echo "Error code = $?\n"
            echo "${MAGENTA}FT_NM:${ENDCOLOR}"
            $FT_NM "$file"
            echo "Error code = $?\n"
        else
            echo "${RED}File not found: ${YELLOW}$file${ENDCOLOR}"
        fi
    done
}

# Function to test a single file
test_file() {
    local file=$1
    if [ -f $file ]; then
        echo "${BLUE}Testing file: ${YELLOW}$file${ENDCOLOR}"

        # Run nm and ft_nm
        $NM "$file" > $RES_FOLDER/nm_output_$(basename $file).txt
        $FT_NM "$file" > $RES_FOLDER/ft_nm_output_$(basename $file).txt

        # Compare outputs
        if diff --color $RES_FOLDER/nm_output_$(basename $file).txt $RES_FOLDER/ft_nm_output_$(basename $file).txt > $RES_FOLDER/diff_$(basename $file).txt; then
            echo "${GREEN}PASS: ${BLUE}Outputs match for ${YELLOW}$file${ENDCOLOR}\n"
        else
            echo "${RED}FAIL: ${BLUE}Outputs differ for ${YELLOW}$file${ENDCOLOR}\n"
        fi

        # Clean up temporary files
        # rm -f nm_output.txt ft_nm_output.txt
    else
        echo "${RED}File not found: ${YELLOW}$file${ENDCOLOR}"
    fi
}

# Parse script arguments
if [ $# -eq 1 ]; then
    make re
    case $1 in
        bin)
            echo "${BLUE}Testing executables in ${YELLOW}$BIN_FOLDER${BLUE}...${ENDCOLOR}\n"
            run_tests "$BIN_FOLDER"
            ;;
        obj)
            echo "${BLUE}Testing object files in ${YELLOW}$OBJ_FOLDER${BLUE}...${ENDCOLOR}\n"
            run_tests "$OBJ_FOLDER"
            ;;
        lib)
            echo "${BLUE}Testing shared libraries in ${YELLOW}$LIB_FOLDER${BLUE}...${ENDCOLOR}\n"
            run_tests "$LIB_FOLDER"
            ;;
        err)
            echo "${BLUE}Testing error in ${YELLOW}$ERR_FOLDER${BLUE}...${ENDCOLOR}\n"
            run_error_test "$ERR_FOLDER"
            ;;
        all)
            echo "${BLUE}Testing all files...${ENDCOLOR}\n"
            run_tests "$BIN_FOLDER"
            run_tests "$OBJ_FOLDER"
            run_tests "$LIB_FOLDER"
            run_error_test "$ERR_FOLDER"
            ;;
        *)
            if [ -f $1 ]; then
                echo "${BLUE}Testing single file: ${YELLOW}$1${ENDCOLOR}"
                test_file "$1"
            else
                echo "${RED}Invalid argument: ${YELLOW}$1${ENDCOLOR}"
                echo "Usage: $0 {bin|obj|lib|all|<file_path>}"
                exit 1
            fi
            ;;
    esac
else
    echo "Usage: $0 {bin|obj|lib|err|all|<file_path>}"
    exit 1
fi

echo "Testing completed."
