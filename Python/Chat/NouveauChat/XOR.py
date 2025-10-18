import base64

def chiffrer(message, cle):
	resultat = ""
	for i in range(len(message)):
		resultat += chr(ord(message[i]) ^ ord(cle[i % len(cle)]))
	return base64.b64encode(resultat.encode("utf-8")).decode("utf-8")

def dechiffrer(message, cle):
	data = base64.b64decode(message).decode("utf-8")
	resultat = ""
	for i in range(len(data)):
		resultat += chr(ord(data[i]) ^ ord(cle[i % len(cle)]))
	return resultat
