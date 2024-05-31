import socket
import select
import threading
import sys
if sys.version_info[0] > 2:
    import queue
else:
    import Queue as queue

# Time to wait (in second) for e2 studio to connect
E2STUDIO_CONNECTION_TIMEOUT = 20
# Maximum packet size that can be received
MAX_PACKET_SIZE = 4096
ENCODING = "UTF-8"

# The server socket in ISServer side
listen_socket = None
# The client socket in e2 studio side
client_socket = None
listen_socket_open = False
client_socket_open = False
# Port number of server socket inside e2 studio
e2studio_port_no = 0
# The server thread
server_thread = None
e2studio_handshake_success = False
e2studio_handshake_failed = False
e2studio_handshake_done_event = None
# Flag indicating that e2 studio is disconnecting
disconnecting = False
# Flag indicating that e2 studio is terminating
terminating = False

control_status_events = queue.Queue()
control_status_event_listener = None

def setup_server_socket(port_no):
    global listen_socket
    global listen_socket_open

    actual_port_no = port_no
    try:
        # Create a socket for listening incoming connection requests
        listen_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM, socket.IPPROTO_TCP)
        listen_socket_open = True
        # Bind the socket to any IP address and the specified port
        listen_socket.bind(('', int(port_no)))
        # Set the socket in listen mode
        listen_socket.listen(1)
        # Get the actual port number the socket is binding to
        address, actual_port_no = listen_socket.getsockname()
    except:
        if (listen_socket is not None):
            listen_socket.close()
            listen_socket_open = False
        raise
    return actual_port_no

def connect(listen_port_no, e2s_port_no):
    global e2studio_port_no
    global e2studio_handshake_done_event
    global server_thread

    # if e2studio_port_no is 0 means the server in e2studio is not created
    # else means the server in e2studio is created, just need to continue connecting
    if (e2studio_port_no != 0):
        return e2studio_port_no

    e2studio_port_no = e2s_port_no

    # Setup server socket if not opened
    if (not listen_socket_open):
        setup_server_socket(listen_port_no)

    if (e2studio_handshake_done_event is None):
        e2studio_handshake_done_event = threading.Event()

    if (server_thread is None):
        server_thread = threading.Thread(None, run_thread)
        server_thread.start()
    # Waiting for control/status socket handshake to complete
    e2studio_handshake_done_event.wait(E2STUDIO_CONNECTION_TIMEOUT)
    if (e2studio_handshake_success):
        # Control/Status socket handshake completed OK
        start_control_status_event_thread()
        e2studio_handshake_done_event = None
    elif (e2studio_handshake_failed):
        raise Exception("Control/Status Socket handshake failed")
    else:
        # Timeout
        raise Exception("Control/Status Socket handshake timed out")

    # Return the actual port number of e2 studio server socket
    return e2studio_port_no

def run_thread():
    global e2studio_handshake_failed
    global disconnecting
    global terminating

    try:
        # Wait for a new client to connect
        wait_for_new_connection()
    except:
        e2studio_handshake_failed = True
        e2studio_handshake_done_event.set()
        raise

    while not disconnecting and not terminating:
        # Reset the input/output packet
        input_packet = ""
        output_packet = ""
        try:
            # Perform a non-blocking call for getting a packet every 1 second
            input_packet = get_packet(1)
        except:
            e2studio_handshake_failed = True
            # Do a check just in case this may happen after the handshake
            if (e2studio_handshake_done_event is not None):
                e2studio_handshake_done_event.set()
            # Probably e2 studio is terminated which cause this exception
            terminating = True
            raise

        if  (input_packet != ""):
            # Handle a valid packet
            error_code, output_packet = handle_packet(input_packet)
            if (error_code == 1):
                # Request to close this socket
                # Clean up and drop out this thread
                disconnecting = True

        # Send a response if one is constructed
        if (output_packet != ""):
            write(output_packet)

    if (disconnecting):
        disconnect()

def wait_for_new_connection():
    global client_socket
    global client_socket_open
    global listen_socket_open

    try:
        # Accept the connection
        conn, address = listen_socket.accept()
        client_socket = conn
        client_socket_open = True
        # Set TCP_NODELAY
        client_socket.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
        # Set Receive buffer
        client_socket.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, 64 * 1024)
    except:
        if (not client_socket_open):
            # Exception happen by accept() so close listen_socket
            # Do not close client_socket if exception happen by setsockopt()
            listen_socket.close()
            listen_socket_open = False
        raise

def get_packet(timeout):
    package = ""
    while (True):
        if (not wait_for_input_data(timeout)):
            # Not wait mean timeout, break and return the received packet
            break
        package += read()
    return package

def wait_for_input_data(timeout):
    reads, writes, errors = select.select([client_socket], [], [], timeout)
    return (client_socket in reads)

def read():
    return client_socket.recv(MAX_PACKET_SIZE).decode(ENCODING)

def handle_packet(input_packet):
    global e2studio_port_no
    global e2studio_handshake_success

    if (input_packet.startswith("e2Studio invoked")):
        return (0, "port no={}\n".format(e2studio_port_no))
    elif (input_packet.startswith("e2Studio ready")):
        message_parts = input_packet.split(",")
        if (len(message_parts) == 2):
            port_info = message_parts[1].split("=")
            if ((len(port_info) == 2) and (port_info[0] == "port no")):
                # Set the actual port number of e2 studio server socket
                e2studio_port_no = port_info[1].replace("\n", "")
                e2studio_handshake_success = True
                e2studio_handshake_done_event.set()
                return (0, "OK\n")
        return (0, "Control/Status Socket, Unsupported Message '{}'\n".format(input_packet))
    elif (input_packet.startswith("event")):
        message_parts = input_packet.split(";", 1)
        if len(message_parts) == 2 and message_parts[0] == "event":
            if control_status_events is not None:
                control_status_events.put(message_parts[1].replace("\n", ""))
            return (0, "OK\n")
        return (0, "Control/Status Socket, Unsupported Message '{}'\n".format(input_packet))
    elif (input_packet.startswith("disconnect")):
        return (1, "OK\n")
    else:
        return (0, "Control/Status Socket, Unsupported Message '{}'\n".format(input_packet))

def write(message):
    client_socket.sendall(message.encode(ENCODING))

def set_event_listener(listener):
    global control_status_event_listener

    control_status_event_listener = listener

def start_control_status_event_thread():
    threading.Thread(None, listen_control_status_events).start()

def listen_control_status_events():
    while not disconnecting and not terminating:
        try:
            # Wait at most 1 second for new event
            event = control_status_events.get(True, 1)
            if control_status_event_listener is not None:
                control_status_event_listener(event)
            control_status_events.task_done()
        except queue.Empty:
            # No event to handle which is normal case
            pass

def disconnect():
    global client_socket
    global listen_socket
    global client_socket_open
    global listen_socket_open

    if (client_socket is not None):
        try:
            client_socket.close()
            client_socket_open = False
        except:
            pass
        client_socket = None

    if (listen_socket is not None):
        try:
            listen_socket.close()
            listen_socket_open = False
        except:
            pass
        listen_socket = None

def terminate():
    global terminating

    terminating = True
