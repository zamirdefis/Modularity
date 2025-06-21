eval $(./getAbsolutePath.sh)
generator="Ninja Multi-Config"
if [ $# -eq 1 ]; then
  if [ $1 == "--rm" ]; then
    rm -rf ${paths["build"]}
    rm -rf ${paths[".cache"]}
  elif [ $1 == "--help" ]; then
    echo "don't use arguments to start the normal generation"
    echo -e "\t--rm to remove files of the previous generation"
    echo -e "\t--help to show this message"
    exit 0
  else
    echo -e -n "\nunknown argument \""
    echo -e $1"\"\n"
    ./generate.sh --help
    exit 0
  fi
fi
cmake -S ${paths["cmake"]} -B ${paths["build"]} -G ${generator}
