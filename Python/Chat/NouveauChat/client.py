import socket
import threading
import sys
import XOR

IP = "localhost"
PORT = 7894
CLE = "Smurtz123"

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((IP, PORT))

pseudo = input("Pseudo : ")
client.send(pseudo.encode("utf-8"))

def envoyerMessage():
	while True:
		message = input()
		if message.strip() == "":
			continue

		data = XOR.chiffrer(message, CLE).encode("utf-8")
		client.send(data)

		if message == "exit":
			client.close()
			sys.exit()

def recevoirMessage():
	while True:
		try:
			data = client.recv(1024)
			if not data:
				break

			message = XOR.dechiffrer(data.decode("utf-8"), CLE)
			
			# ~ if not f"<{pseudo}> a rejoint le chat" in message:
				# ~ print(message)
			
			print(message)

		except:
			print("Connexion perdue.")
			break

threading.Thread(target=envoyerMessage, daemon=True).start()
# ~ threading.Thread(target=recevoirMessage, daemon=True).start()
recevoirMessage()

# ~ # Pour que le programme reste vivant
# ~ while not mort:
	# ~ pass
	
# ~ client.close()
