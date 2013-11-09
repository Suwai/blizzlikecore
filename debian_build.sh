#!/bin/bash
####################################################
function pause(){
  read -n1 -p "$*"
}
function applypatch(){
  echo
  for PATCH_ in patch/*.patch
  do
    read -p " Apply $PATCH_ (y or n)? " APPT
    if [ -z "$APPT" ]; then APPT="n"; fi
    if  [ $APPT == "y" ]; then patch -p1 < $PATCH_; fi
    [[ $? != 0 ]] && exit 1
  done
  pause 'Press [any] key to continue...'
}
function buildcore(){
  echo
  read -p "Cores [4] " CORES
  if [ -z "$CORES" ]; then CORES=4; fi
  read -p "Debug [0] " DEBUG
  if [ -z "$DEBUG" ]; then DEBUG=0; fi
  read -p "Scripts [1] " SCRIPTS
  if [ -z "$SCRIPTS" ]; then SCRIPTS=1; fi
  read -p "Tools [0] " TOOLS
  if [ -z "$TOOLS" ]; then TOOLS=0; fi
  rm -Rf build
  mkdir build
  cd build
  cmake ../ -DPREFIX=/home/$(id -un)/server -DSCRIPTS=${SCRIPTS} -DTOOLS=${TOOLS} -DDEBUG=${DEBUG}
  pause 'Press [any] key to continue...'
  make -j ${CORES}
  make install
  pause 'Press [any] key to continue...'
}
####################################################
function menu(){
  clear
  echo
  echo "================="
  echo "= BlizzLikeCore ="
  echo "================="
  echo
  echo "Apply Patch---[1]"
  echo "Build Core----[2]"
  echo "Exit----------[0]"
  echo
  read -p "what is your option? " var
  if  [ $var == 1 ]; then applypatch; menu
  elif  [ $var == 2 ]; then buildcore; menu
  elif  [ $var == 0 ]; then break
  else menu
  fi
}
menu
