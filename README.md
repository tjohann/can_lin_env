Linux Realtime and Time-Triggert-System
=======================================

This repository is my playground around can and lin. It will include scripts and code example for different usecases on different target devices. One (for my opinion) interesting approach is to use a jailhouse bare-metal cell for can.

Dont expect to much from this repository because its only a starting point and more a playground than something serious.

Everthing useful will make it into libbaalue (https://github.com/tjohann/libbaalue.git).


Prerequisites
-------------

I use libcap-ng to get rid of root rights, can-utils (https://github.com/linux-can/can-utils) for some scripts and jailhouse (https://github.com/siemens/jailhouse) for the hypervisor examples.
The LIN examples need an external driver which is available here -> http://rtime.felk.cvut.cz/gitweb/linux-lin.git

If you use my sdk's for Arietta (https://github.com/tjohann/arm926_sdk) or Bananapi (https://github.com/tjohann/a20_sdk), then you have everthing included.

For the microcontroller example i use avr-gcc and my avr sdk (https://github.com/tjohann/avr_sdk).


Documentation
-------------

Below ./Documentation you find some hopefully usefull stuff.

      info_*.* -> some general and unsorted informations about a topic
      howto_*.* -> a more cookbook style


Target devices (Linux)
----------------------

Directory -> ./linux

	1). Arietta with MCP2515
	2). Bananapi with MCP2515
	3). Bananapi with A20-CAN
	4). ...


Target devices (µC)
-------------------

Directory -> ./atmel

	1). AT90 and MCP2515
	2). AT90CAN128
	3). ...


Target devices (hypervisor)
---------------------------

Directory -> ./atmel

	1). Bananapi with MCP2515 -> bare-metal Jailhouse Cell
	2). Bananapi with A20-CAN -> bare-metal Jailhouse Cell
	3). ...


Installation
------------

There're make tags for install and uninstall. It will copy the libs to ${HOME}/lib and the bin to ${HOME}/bin.

	 make install
	 ...
	 make uninstall


To set filecaps (see man 7 capabilities) for bin it needed (no need to run them as root)

       make set_caps

