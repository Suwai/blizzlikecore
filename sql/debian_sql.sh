#!/bin/bash
####################################################
function pause(){
  read -n1 -p "$*"
}
clear
echo
echo "================"
echo "= MySQL Config ="
echo "================"
read -p "MySQL User [root] " USER
if [ -z "$USER" ]; then USER="root"; fi
read -p "MySQL Pass [] " PASS
if [ -z "$PASS" ]; then PASS=""; fi
MYSQL_="mysql -u ${USER} -p${PASS}"
####################################################
function dropmysql(){
  echo
  $MYSQL_ < drop_mysql.sql
  pause 'Press [any] key to continue...'
}
function createmysql(){
  echo
  $MYSQL_ < create_mysql.sql
  pause 'Press [any] key to continue...'
}
function appsql(){
  echo
  echo "Wait!"
  for SQL_ in development/${LOC_}/*.sql
  do
    echo " Applying $SQL_"
    $MYSQL_ ${LOC_} < $SQL_
    [[ $? != 0 ]] && exit 1
  done
  pause 'Press [any] key to continue...'
}
function setrealmlist(){
  echo
  read -p "What is your Server IP? [127.0.0.1] " IP
  if [ -z "$IP" ]; then IP="127.0.0.1"; fi
  $MYSQL_ -e "update auth.realmlist set address = '"${IP}"' where id = 1;"
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
  echo "Create MySQL--[1]"
  echo "World DB------[2]"
  echo "Chars DB------[3]"
  echo "Auth DB-------[4]"
  echo "Scripts DB----[5]"
  echo "Set Realmlist-[6]"
  echo "Drop MySQL----[7]"
  echo "Exit----------[0]"
  echo
  read -p "what is your option? " var
  if  [ $var == 1 ]; then createmysql; menu
  elif  [ $var == 2 ]; then LOC_="world"; appsql; menu
  elif  [ $var == 3 ]; then LOC_="characters"; appsql; menu
  elif  [ $var == 4 ]; then LOC_="auth"; appsql; menu
  elif  [ $var == 5 ]; then LOC_="scripts"; appsql; menu
  elif  [ $var == 6 ]; then setrealmlist; menu
  elif  [ $var == 7 ]; then dropmysql; menu
  elif  [ $var == 0 ]; then break
  else menu
  fi
}
menu
