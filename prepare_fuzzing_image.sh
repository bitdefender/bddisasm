#!/bin/bash
set -e

img="bddfuzz"
out="bddfuzz.tar"

rm -rf "${out}"

podman build -t ${img} -f Dockerfile.fuzzing .
podman save -o ${out} ${img}

echo "Done! Copy ${out} to the fuzzing machine and load it with 'podman load -i ${out}'"
echo "You probably want to run it like this: "
echo "'podman run --cap-add=SYS_PTRACE -it -v /host/share/:/bddfuzz/share ${img} /bin/bash'"
echo "All fuzzers are present in the image, you must start one of them"
