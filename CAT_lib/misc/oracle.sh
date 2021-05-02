#!/bin/bash

for i in `ls | grep test` ; do
  if ! test -d $i ; then
    continue ;
  fi

  pushd ./ &> /dev/null ;
  cd $i ;
  if test -f output*/*_output ; then
    popd &> /dev/null ;
    continue ;
  fi
  if test -f output*/oracle.txt ; then
    popd &> /dev/null ;
    continue ;
  fi

  echo $i ;
  make oracle ;
  echo "" ;
  
  popd &> /dev/null ;
done
