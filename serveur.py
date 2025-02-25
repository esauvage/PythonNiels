import socket
import threading

IP = "0.0.0.0"
PORT = 5555

serveur = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

serveur.bind((IP, PORT))

serveur.listen(2)

clients = []
pseudos = []

def diffuser(message):
    for client in clients:
        client.send(bytes(message, "utf-8"))

def gestion_connexion():
    while True:
        client, adresse = serveur.accept()
        print(f"Connexion établie avec {str(adresse)}")

        pseudo = client.recv(1024).decode("utf-8")
        if len(pseudo) > 32:
            continue
        clients.append(client)
        pseudos.append(pseudo)

        print(f"{pseudo} a rejoint le chat")
        client.send(bytes("Bienvenue dans le chat ! \n", "utf-8"))
        diffuser(f"{pseudo} a rejoint le chat")

        thread_client = threading.Thread(target=gestion_client, args=(client, pseudo))
        thread_client.start()

def gestion_client(client, pseudo):
    while True:
        try:
            message = client.recv(1024).decode("utf-8")

            if message == "exit":
                index = clients.index(client)
                clients.remove(client)
                client.close()

                pseudo = pseudos[index]
                pseudos.remove(pseudo)

                diffuser(f"{pseudo} a quitté le chat")
                break

            else:
                diffuser(f"{pseudo} : {message}")

        except:
            index = clients.index(client)
            clients.remove(client)
            client.close()

            pseudo = pseudos[index]
            pseudos.remove(pseudo)

            diffuser(f"{pseudo} a quitté le chat")
            break

print("Le serveur de chat fonctionne.")
gestion_connexion()
