#!/bin/sh


BIN_DIR=`dirname $0`
PROJECT_DIR="${BIN_DIR}/.."
GH_PROJECTS="kfsone/imguiwrap altschuler/imgui-knobs Nelarius/imnodes"
OFFLINE=${OFFLINE:="no"}

cd ${PROJECT_DIR}
for project in ${GH_PROJECTS}; do
  GH_USERNAME=`echo ${project} | cut -f 1 -d '/'`
  GH_PROJECT=`echo ${project} | cut -f 2 -d '/'`
  if [ ! -d "${GH_PROJECT}" ]; then
    git clone --recurse-submodules https://github.com/${project}
  elif [ "${OFFLINE}" = "no" ]; then
    git -C "${GH_PROJECT}" pull
    git -C "${GH_PROJECT}" submodule update --init
  fi
done
