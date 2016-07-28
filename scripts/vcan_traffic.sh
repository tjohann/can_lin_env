#!/usr/bin/env bash

# catch SIGNALS
function cleanup {
    echo ""
    echo "+----------------------------+"
    echo "| Cheers $USER "
    echo "+----------------------------+"
    echo ""
}
trap cleanup EXIT

echo "+------------------------------------------------------------+"
echo "| Generate traffic via canplayer on vcan0 (can_sequence.txt) |"
echo "| (infinit loop)                                             |"
echo "+------------------------------------------------------------+"
canplayer -l i -I can_sequence.txt

