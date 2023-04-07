import socket

HOST = '192.168.0.105'  # Standard loopback interface address (localhost)
PORT = 65432        # Port to listen on (same as the one used in the M5StickC code)
if __name__ == "__main__":
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST, PORT))
        s.listen()
        print(f"Listening on {HOST}:{PORT}")
        conn, addr = s.accept()
        with conn:
            print(f"Connected by {addr}")
            try:
                while True:
                    data = conn.recv(1024)
                    if not data:
                        break
                    print(data.decode(), end='')

                    # Send message to client based on user input
                    message = input("Enter message to send to client: ")
                    conn.sendall(message.encode())
            except KeyboardInterrupt:
                print("\nKeyboard interrupt received. Stopping server.")