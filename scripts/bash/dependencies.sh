declare -A paths
paths["cmake"]=$(realpath "../../cmake")
paths["build"]=$(realpath "../../build")
paths["scripts"]=$(realpath "../../scripts")
paths["include"]=$(realpath "../../include")
paths["src"]=$(realpath "../../src")
paths[".cache"]=$(realpath "../../.cache")
paths["release"]=${paths["build"]}/Release
paths["debug"]=${paths["build"]}/Debug
binName=$(cat ${paths["cmake"]}/binName.txt)
paths["releaseBin"]=${paths["release"]}/${binName}
paths["debugBin"]=${paths["debug"]}/${binName}
declare -p paths
