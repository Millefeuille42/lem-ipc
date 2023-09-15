#!/usr/bin/env bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <teams> <player_per_team>"
    exit 1
fi

team_count="$1"
player_count="$2"

for ((a=1; a<=team_count; a++)); do
  ./spawn_team.sh $player_count $a
done
