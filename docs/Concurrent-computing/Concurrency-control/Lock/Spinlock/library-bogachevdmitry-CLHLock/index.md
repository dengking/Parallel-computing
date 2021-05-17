# [bogachevdmitry](https://github.com/bogachevdmitry)/**[CLHLock](https://github.com/bogachevdmitry/CLHLock)**

CLH locks (due to Travis Craig, Erik Hagersten, and Anders Landin), it is a spin lock based on a linked list, it constantly polls the state of the precursor, if the precursor releases the lock, it ends the spin.

