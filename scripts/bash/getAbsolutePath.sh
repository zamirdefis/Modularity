declare -A paths
paths["cmake"]=$(realpath "../../cmake")
paths["build"]=$(realpath "../../build")
paths["scripts"]=$(realpath "../../scripts")
paths["include"]=$(realpath "../../include")
paths["src"]=$(realpath "../../src")
paths[".cache"]=$(realpath "../../.cache")
declare -p paths
