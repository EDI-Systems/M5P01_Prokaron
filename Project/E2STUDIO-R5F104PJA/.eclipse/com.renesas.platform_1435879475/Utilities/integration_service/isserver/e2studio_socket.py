import socket
import threading

# IP address to connect to e2 studio
E2STUDIO_IP_ADDR = "127.0.0.1"
# Maximum response size from e2 studio
E2STUDIO_SOCKET_TX_BUFFER = 2048
ENCODING = "UTF-8"

# The socket to connect to e2 studio
e2s_socket = None
# Port number of server socket inside e2 studio
e2studio_port_no = 0
e2studio_handshake_success = False
e2studio_handshake_failed = False
e2studio_handshake_done_event = None

def connect(port_no):
    global e2studio_port_no
    global e2studio_handshake_done_event

    e2studio_port_no = port_no
    e2studio_handshake_done_event = threading.Event()

    try:
        thread = threading.Thread(None, run_thread)
        thread.start()
        # Waiting for e2 studio server socket handshake to complete
        e2studio_handshake_done_event.wait()
        if (e2studio_handshake_success):
            # e2 studio server socket handshake completed OK
            return
        elif (e2studio_handshake_failed):
            raise Exception("e2 studio Server Socket handshake failed")
        else:
            # Timeout
            raise Exception("e2 studio Server Socket handshake timed out")
    finally:
        e2studio_handshake_done_event = None

def run_thread():
    global e2s_socket
    global e2studio_handshake_failed
    global e2studio_handshake_success

    try:
        # Create a socket
        e2s_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM, socket.IPPROTO_TCP)
        # Connect to server in e2 studio
        e2s_socket.connect((E2STUDIO_IP_ADDR, int(e2studio_port_no)))

        # Check e2 studio status
        response = send_message("GetStatus")
        if (response.startswith("Ready for input")):
            e2studio_handshake_success = True
            e2studio_handshake_done_event.set()
        else:
            raise Exception("e2 studio is not ready for input")
    except:
        e2studio_handshake_failed = True
        e2studio_handshake_done_event.set()
        raise

def send_message(message):
    message += "\n"
    # Send command to e2 studio
    e2s_socket.sendall(message.encode(ENCODING))
    # Receive response from e2 studio
    list_data = []
    while 1:
        #receiving data
        data = e2s_socket.recv(E2STUDIO_SOCKET_TX_BUFFER).decode(ENCODING)
        if data:
            list_data.append(data)
            #call recv until '\n' is received
            if data.endswith('\n'):
                break
        else:
            raise Exception("Connection to e2 studio has been closed")
    response = "".join(list_data)
    if (len(response) == 0):
        raise Exception("Connection to e2 studio has been closed")
    else:
        if (response.endswith('\n')):
            return response[0:len(response) - 1]
        else:
            return response
