#!/bin/bash

export bin_dir="build"

if [ ! -d "$bin_dir" ]; then
  mkdir -p "$bin_dir"
fi

if [ -d "$bin_dir" ]; then
  cd "./$bin_dir"
  cmake .. -G "Unix Makefiles"
  cmake --build .
else
  echo "unable to create ${bin_dir}"
  exit 1
fi