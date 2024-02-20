#!/bin/bash
set -e

# Check if the -it flag is provided
if [ -t 0 ] && [ -t 1 ]; then
    echo "Running in interactive mode"
else
    echo "Error: This container requires an interactive shell (-it flag)."
    exit 1
fi

# Check if the volume directory is present
if [ ! -d "${SHARE_DIR}" ]; then
  echo "Warning: Volume directory '${SHARE_DIR}' is missing."
  echo "Please use the -v flag to specify a host directory."
  echo "For example 'podman run -v /host/path:${SHARE_DIR}'"
  echo "You will need to manually copy fuzzing inputs/outputs from/to the host"
fi

# Continue with the original command
exec "$@"
