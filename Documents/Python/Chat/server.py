import socket
import threading

HOST = "0.0.0.0"
PORT = 4556

clients = []

def handle_client(conn, addr):
    print(f"Nouvelle connexion de {addr}")
    clients.append(conn)

    try:
        while True:
            msg = conn.recv(1024)
            if not msg:
                break
            print(f"{addr} : {msg.decode()}")
            broadcast(msg, conn)
    except:
        pass
    finally:
        print(f"{addr} s'est déconnecté.")
        clients.remove(conn)
        conn.close()

def broadcast(message, sender_conn):
    for client in clients:
        if client != sender_conn:
            try:
                client.send(message)
            except:
                clients.remove(client)

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((HOST, PORT))
server.listen()

print(f"Serveur en écoute sur {HOST}:{PORT}")

while True:
    conn, addr = server.accept()
    thread = threading.Thread(target=handle_client, args=(conn, addr))
    thread.start()
