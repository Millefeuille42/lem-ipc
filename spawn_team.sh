#!/usr/bin/env bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <count> <team_name>"
    exit 1
fi

count="$1"
team_name="$2"

for ((i=0; i<=$count; i++)); do
    echo "Team: $team_name - Iteration: $i"
    ./lemipc $team_name &
done
