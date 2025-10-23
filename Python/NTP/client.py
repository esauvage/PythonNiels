import socket
import sys


def recv_exact(sock, n):
    data = b""
    while len(data) < n:
        packet = sock.recv(n - len(data))
        if not packet:
            break
        data += packet
    return data


IP = "localhost"
PORT = 9632

socketClient = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
socketClient.connect((IP, PORT))

print("Connecté au serveur. Démarrage du transfert de fichiers.")

nomFichier = sys.argv[1]

socketClient.send(f"{len(nomFichier):08}".encode("utf-8"))
socketClient.send(nomFichier.encode("utf-8"))

# Vérifiacation du nom
tailleResponse = int(recv_exact(socketClient, 8).decode("utf-8"))
reponse = recv_exact(socketClient, tailleResponse).decode("utf-8")

if reponse != "Nom de fichier correct.":
    print("Nom de fichier refusé par le serveur.")
    socketClient.close()
    sys.exit(1)

# Envoi du contenu
with open(nomFichier, "rb") as f:
    data = f.read()

socketClient.send(f"{len(data):08}".encode("utf-8"))
socketClient.send(data)

# Vérification du fichier
tailleResponse = int(recv_exact(socketClient, 8).decode("utf-8"))
reponse = recv_exact(socketClient, tailleResponse).decode("utf-8")

if reponse == "Fichier correct.":
    print(f"Fichier '{nomFichier}' transmis avec succès !")
else:
    print("Erreur pendant la transmission.")

socketClient.close()
