import sys
from elevators import Elev, ElevListener
from mysocket import Socket, ListenerSocket
from select import select
from random import choice

address = ('localhost', 10000)

elevator_list = []

sending = []

listen = ElevListener(*address)

def waiting():
    select(elevator_list + [listen], sending, [])

def incomming():
    return select([listen], [], [], 0)[0]

def get_elevator():
    return select(elevator_list, [], [], 0)[0]

def writing():
    return select([], sending, [], 0)[1]


def move_elevator(floor, dir):
    message = str(floor)+" "+str(dir)
    for Elev in elevator_list:
        if (dir == 1) and ( (Elev.dir == 1) or (Elev.dir == 0) ) and (Elev.floor <= floor) :
            break
        elif (dir == -1) and ( (Elev.dir == -1) or (Elev.dir == 0) )  and (Elev.floor >= floor):
            break
    else:
        Elev = choice(elevator_list)
    
    Elev.writeln(message)
    Elev.orders.add(floor, dir)
        
    
def handle_button(Elev):
    message = Elev.readln()
    message = message.split()
    floor = int(message[1])
    dir = int(message[2])
    if message[0] == "btn":
        move_elevator(floor,dir)
    if message[0] == "loc":
        Elev.floor = floor
        Elev.dir = dir
        
def handle_disconnect(Elev):
    for i in Elev.orders:
        move_elevator(i[0],i[1])
    
def main():
    
    while 1:
        waiting()
        for socket in incomming():
            elevator_list.append(socket.accept())
            
        for socket in get_elevator():
            if not socket.transport_in():
                socket.close()
                elevator_list.remove(socket)
                handle_disconnect(socket)
            if socket.hasln():
                handle_button(socket)
                
        for socket in writing():
            socket.transport_out()
        sending = filter(Socket.is_sending, elevator_list)
    
if __name__ == '__main__':
    main()
