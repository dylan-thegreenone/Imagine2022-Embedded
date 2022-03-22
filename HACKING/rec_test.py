import socket
import sys

BLOCK_SIZE = 4096

def main():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server_addr = ('', 6666)
    sock.bind(server_addr)
    try:
        while True:
            buff = ""
            for v in sock.recv(BLOCK_SIZE):
                buff += chr(v)
            print(buff)
    except KeyboardInterrupt:
        print("\nAdios mf")



if __name__ == '__main__':
    main()
