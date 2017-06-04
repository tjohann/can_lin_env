#!/usr/bin/env bash

echo " "
echo "+----------------------------------------+"
echo "|   Try to set file capabilities         |"
echo "| --> prepare your password for sudo     |"
echo "+----------------------------------------+"
echo " "

sudo -v
# keep-alive
while true; do sudo -n true; sleep 60; kill -0 "$$" || exit; done 2>/dev/null &


# can_recv
if [ -e linux/can_recv/can_recv ]; then
    sudo setcap "cap_sys_nice=pe" ./linux/can_recv/can_recv
else
    echo "./linux/can_recv/can_recv does not exist"
fi

if [ -e ${HOME}/bin/can_recv ]; then
    sudo setcap "cap_sys_nice=pe" ${HOME}/bin/can_recv
else
    echo "${HOME}/bin/can_recv does not exist"
fi

# can_send
if [ -e linux/can_recv/can_recv ]; then
    sudo setcap "cap_sys_nice=pe" ./linux/can_recv/can_recv
else
    echo "./linux/can_recv/can_send does not exist"
fi

if [ -e ${HOME}/bin/can_send ]; then
    sudo setcap "cap_sys_nice=pe" ${HOME}/bin/can_send
else
    echo "${HOME}/bin/can_send does not exist"
fi

echo " "
echo "+----------------------------------------+"
echo "|          Cheers $USER                |"
echo "+----------------------------------------+"
echo " "
