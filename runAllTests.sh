#!/bin/bash
KnownModules=(Core Network)
Modules=()
Verbose=0
Rebuild=0

if [ "$1" == '-h' ] || [ "$1" == '-help' ] || [ "$1" == '--help' ]; then
  echo "Usage: runAllTests.sh [-r|--rebuild] [-v|--verbose] [Modules]"
  echo "-r --rebuild      Rebuilds all tests in [Modules]"
  echo "-v --verbose      Outputs everything"
  echo 
  echo "By default the script will run all tests in [Modules]"
  echo "If the tests are not already built, or if --rebuild was passed, those will be compiled."
  echo "If no [Modules] are supplied, all modules are selected."
  exit
fi

while (( $# > 0 )); do
  if [ $1 == '-v' ] || [ $1 == '--verbose' ]; then
    Verbose=1
  elif [ $1 == '-r' ] || [ $1 == '--rebuild' ]; then
    Rebuild=1
  else
    Modules+=("$1")
  fi
  
  shift
done

# 
if [ ${#Modules[@]} == 0 ]; then
  Modules=$KnownModules
fi

# 
function die() {
  echo "$*"
  exit 1
}

function buildTest() {
  Path="$1"
  cd "$Path"
  echo "##### Building $Path"
  if [ $Verbose == 1 ]; then
    make $MakeSuffix
  else
    make $MakeSuffix > /dev/null 2> /dev/null
  fi
  
  [ $? != 0 ] && die "##### Failed to build $Path"
  cd ../..
}

function runTest() {
  Name="$1"
  Binary=./tst_$(echo $Name|sed 's!.*/!!')
  
  echo "##### Running test $Name"
  if [ $Verbose == 1 ]; then
    $Binary
  else
    $Binary | egrep -B 1 -e '^FAIL|failed'  | grep -v -e '^PASS' 
  fi
}

# Rebuild?
if [ $Rebuild == 1 ]; then
  for i in ${Modules[@]}; do
    for j in $i/*; do
      [ ! -d "$j" ] && continue
      cd $j
      make clean
      rm $(find . -name 'tst_*' -executable)
      cd ../..
    done
  done
fi

# Build an run
for i in ${Modules[@]}; do
  for j in $i/*; do
    [ ! -d "$j" ] && continue
    buildTest $j
    (cd $j; runTest $j)
  done
done
