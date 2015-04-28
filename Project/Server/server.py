import sys
from elevators import Elev, ElevListener
from mysocket import Socket, ListenerSocket
from select import select
from random import choice
from math import fabs

address = ('0.0.0.0', 8080)

elevator_list = []

def handle_disconnect(Elev):
    for i in Elev.order_list:
        move_elevator(i[0],i[1])

def handle_elev_msg(Elev):
    message = Elev.readln()
    message = message.split()
    floor = int(message[1])
    dir = int(message[2])
    if message[0] == "btn":
        move_elevator(floor,dir)
    if message[0] in ("loc","stop"):
        Elev.floor = floor
        Elev.dir = dir
    if message[0] == "stop":
        Elev.order_list.discard((floor,dir))
        for elevator in elevator_list:
            elevator.writeln("lite "+str(floor)+" "+str(dir))
        

def move_elevator(floor, dir):
    message = "move "+str(floor)+" "+str(dir)
    the_chosen = None
    for Elev in elevator_list:
        elev_up = (Elev.dir == 1)
        elev_down = (Elev.dir == -1)
        if (dir == 1) and elev_up and (Elev.floor <= floor) :
            break
        elif (dir == -1) and elev_down and (Elev.floor >= floor):
            break
        elif ((dir == 1) or (dir == -1)) and Elev.dir == 0:
            if (Elev.floor+1 == floor) or (Elev.floor-1 == floor):
                break
    else:
        the_chosen = elevator_list[0]
        for i in elevator_list:
            if fabs(i.floor -floor) <= (the_chosen.floor - floor):
                the_chosen = i
        Elev = the_chosen            
        
    Elev.writeln(message)
    print Elev, message
    Elev.order_list.add((floor, dir))
    
def main():
    
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

    print "running"
    while 1:
        
        waiting()
        
        
        for socket in incomming():
            elevator_list.append(socket.accept())
            
        for socket in get_elevator():
            if not socket.transport_in():
                socket.close()
                elevator_list.remove(socket)
                if elevator_list:
                    handle_disconnect(socket)
            if socket.hasln():
                handle_elev_msg(socket)
                
        for socket in writing():
            socket.transport_out()
        sending = filter(Socket.is_sending, elevator_list)
    
if __name__ == '__main__':
    main()
