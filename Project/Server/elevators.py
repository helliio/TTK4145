from mysocket import Socket, ListenerSocket

class Elev(Socket):
    def __init__(self):
        self.floor = None;
        self.dir = None;
        self.orders = set()
        

    @staticmethod
    def from_socket(socket):
        socket.__class__ = Elev
        Elev.__init__(socket)
        return socket

class ElevListener(ListenerSocket):
    def accept(self):
        socket = ListenerSocket.accept(self)
        Elev = Elev.from_socket(socket)
        print "Elevator connected."
        return Elev
