import socket
import threading
import sys
import VigenèreChat

IP = "127.0.0.1"
PORT = 5555

CLE = """zaP$RGb/u;Xu#TBZ~KN>,:R9ll4t@K<^ncQp@J9w-&a8DISQ9.5~2DVG^z@)D~PniPt6KtMtY7^A^d5ofuz@W6&w_ik={bR)U!*4"""

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

try:
    client.connect((IP, PORT))

except Exception as e:
    print("Serveur introuvable. Pour régler le problème, vérifier votre connexion à Internet. Sinon, réessayer plus tard.\nVoulez-vous afficher l'erreur ?")
    choix = input()

    if choix == "oui".lower():
        print(e)

    exit()

pseudo = input("Entrez votre pseudo : ").strip()
client.send(bytes(pseudo, "utf-8"))

print("Bienvenue dans le chat !\n")

cle = 3

def recevoir_messages():
    while True:
        try:
            message = client.recv(1024).decode("utf-8")
            if not message:
                break

            message = VigenèreChat.decrypt(message, CLE)


            # if not message.startswith(pseudo + " :"):
                # print(message.strip())

            print(message.strip())

        except:
            print("Connexion perdue.")
            break

    client.close()
    sys.exit()

def envoyer_messages():
    while True:
        message = input()
        if message.strip().lower() == "exit":
            client.send(b"exit")
            break
        else:
            message = f"{pseudo} : {message}"
            message = VigenèreChat.encrypt(message, CLE)
            client.send(bytes(message, "utf-8"))

    client.close()
    sys.exit()

threading.Thread(target=recevoir_messages).start()
envoyer_messages()
