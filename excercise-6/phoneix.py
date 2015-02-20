#/usr/bin/env python2

from socket import socketpair, timeout
from os import fork
from time import sleep

def run_master(socket, i):
    while True:
        print(i)
        socket.sendall(str(i))
        sleep(0.4)
        i += 1

def run_slave(socket):
    i = 0
    socket.settimeout(3)
    try:
        while True:
            remote = socket.recv(1024)
            i = int(remote)
    except timeout:
        return i + 1

def main():
    master_socket, slave_socket = socketpair()

    i = 0

    while True:
        if fork() != 0:
            run_master(master_socket, i)
            # unreachable
        i = run_slave(slave_socket)
        # slave is promoting to master

if __name__ == "__main__":
    main()
