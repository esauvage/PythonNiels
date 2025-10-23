import socket


def recv_exact(sock, n):
    data = b""
    while len(data) < n:
        packet = sock.recv(n - len(data))
        if not packet:
            break
        data += packet
    return data


IP = "0.0.0.0"
PORT = 9632

socketServeur = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
socketServeur.bind((IP, PORT))
socketServeur.listen()

print("En attente d'une connexion...")

connexionClient, adresseClient = socketServeur.accept()
print(f"Nouvelle connexion venant de {adresseClient}")

# Réception du nom du fichier
tailleNomFichier = int(recv_exact(connexionClient, 8).decode("utf-8"))
nomFichier = recv_exact(connexionClient, tailleNomFichier)

if len(nomFichier) != tailleNomFichier:
    message = "Nom de fichier incorrect."
else:
    message = "Nom de fichier correct."

connexionClient.send(f"{len(message):08}".encode("utf-8"))
connexionClient.send(message.encode("utf-8"))

# Réception du contenu
tailleFichier = int(recv_exact(connexionClient, 8).decode("utf-8"))
fichier = recv_exact(connexionClient, tailleFichier)

if len(fichier) != tailleFichier:
    message = "Fichier incorrect."
else:
    message = "Fichier correct."

connexionClient.send(f"{len(message):08}".encode("utf-8"))
connexionClient.send(message.encode("utf-8"))

# Sauvegarde
with open(nomFichier.decode("utf-8"), "wb") as f:
    f.write(fichier)

print(f"Fichier reçu et sauvegardé : {nomFichier.decode('utf-8')}")

connexionClient.close()
socketServeur.close()
