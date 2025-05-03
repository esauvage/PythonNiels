import hashlib
import time
import string
import itertools
import os
import argparse

class HashHunter:
    def __init__(self):

        self.parser = argparse.ArgumentParser()

        self.parser.add_argument("--hash", help="The type of hash to crack.", required=False)
        self.parser.add_argument("-d", "--dictionary", help="The path to the dictionary used to test passwords.", required=False)
        self.parser.add_argument("-p", "--password", help="The password to crack.", required=False)
        self.parser.add_argument("-l", "--lenght", help="The length of the password for an incremental attack.", required=False)
        self.parser.add_argument("-c", "--custom", help="The custom password creation mode.", required=False)
        self.parser.add_argument("-v", "--verbose", help="Show all tested passwords", required=False)

        self.args = self.parser.parse_args()

        self.hash_type = None

    def choix(self):
        while True:
            choix = input("Do you want to use (i)ncremental mode, (d)ictionary mode or (c)ustom dictionary creation mode ? (q for quit, h for help) : ")

            if choix == "h":
                print("""HashHunter is a brute-force program developed in Python.
                It supports three modes:
                - (d) or -d Dictionary attack (fastest).
                - (i) or -i Incremental brute-force (slow).
                - (c) -c Creating custom wordlists.""")

            elif choix == "i":
                mdp = input("Enter your hash : ")
                longueur = input("Enter the length of the password : ")
                self.hash_type = input("Enter the hash type (md5/sha1/sha256/sha224/sha384/sha512) : ").lower()

                self.incr_mode(mdp, longueur)

            elif choix == "d":
                self.dict_mode()

            elif choix == "c":
                words = input("Enter your list of words separated by a space : ").split(" ")

                self.create_custom_dict(words)

            elif choix == "q":
                exit()

            else:
                print("This choice is not in the options.")

    def create_custom_dict(self, words):
        custom_dict_file = "custom_dict.txt"

        script_dir = os.path.dirname(os.path.abspath(__file__))
        file_path = os.path.join(script_dir, custom_dict_file)

        with open(file_path, 'w', encoding='utf-8') as f:
            for c in range(1, len(words)):
                for x in itertools.permutations(words, c):
                    f.write("".join(x) + "\n")

        print("Your wordlist has been generated successfully ! ")
        self.choix()

    def hash_function(self, word):

        if self.hash_type == "md5":
            return hashlib.md5(word.encode()).hexdigest()
        elif self.hash_type == "sha1":
            return hashlib.sha1(word.encode()).hexdigest()
        elif self.hash_type == "sha256":
            return hashlib.sha256(word.encode()).hexdigest()
        elif self.hash_type == "sha224":
            return hashlib.sha224(word.encode()).hexdigest()
        elif self.hash_type == "sha384":
            return hashlib.sha384(word.encode()).hexdigest()
        elif self.hash_type == "sha512":
            return hashlib.sha512(word.encode()).hexdigest()
        else:
            print("Invalid hash type. Defaulting to MD5.")
            return hashlib.md5(word.encode()).hexdigest()

    def dict_mode(self):
        if not self.args.hash:
            self.hash_type = input("Enter the hash type (md5/sha1/sha256/sha224/sha384/sha512) : ").lower()

        if not self.args.password:
            mdp = input("Enter your hash : ")

        if self.args.password:
            mdp = self.args.password

        while len(mdp) not in [32, 40, 56, 64, 96, 128]:
            print("This string is not a valid hash!")
            mdp = input("Enter your hash : ")

        if not self.args.dictionary:
            chemin_dict = input("Enter the full path of your dictionary. If you don't put anything, our list will be used by default) : ")

        else:
            chemin_dict = self.args.dictionary

        if os.path.isfile(chemin_dict):
            try:
                with open(chemin_dict, encoding="utf8", errors='ignore') as rockyou:
                    lignes = [ligne.strip() for ligne in rockyou.readlines()]
            except Exception as e:
                print(f"An error occurred while reading the file: {e}")
                self.choix()
        else:
            script_dir = os.path.dirname(os.path.abspath(__file__))
            file_path = os.path.join(script_dir, "Liste.txt")

            with open(file_path, encoding="utf8", errors='ignore') as rockyou:
                lignes = [ligne.strip() for ligne in rockyou.readlines()]

        debut = time.time()

        for i, ligne in enumerate(lignes):
            if mdp.upper() == self.hash_function(ligne).upper():
                fin = time.time()
                print(f"PASSWORD FOUND! \"{ligne}\" in {fin - debut:.2f} seconds.")
                self.choix()

            if not self.args.verbose:
                if i % 1000000 == 0:
                    print(f"Tested {i+1} passwords...")

            else:
                print(f"Tested : {ligne}")

        print("Password not found.")
        self.choix()

    def incr_mode(self, mdp, longueur):

        debut = time.time()
        caracteres = string.ascii_lowercase + string.digits

        print(f"Starting brute-force (length {longueur})...")

        for test_pass in map("".join, itertools.product(caracteres, repeat=int(longueur))):
            if mdp == self.hash_function(test_pass):
                fin = time.time()
                print(f'PASSWORD FOUND! "{test_pass}" in {fin - debut:.2f} seconds.')
                self.choix()

        print("Password not found.")
        self.choix()

    def debut(self):
        script_dir = os.path.dirname(os.path.abspath(__file__))
        file_path = os.path.join(script_dir, "Logo.txt")

        with open(file_path, "r") as fichier:
            print(fichier.read() + "\n")

        print("Welcome on HashHunter !")

        if self.args.dictionary:
            if self.args.hash:
                self.hash_type = self.args.hash

            self.dict_mode()

        elif self.args.lenght:
            if self.args.hash:
                self.hash_type = self.args.hash

                if self.args.password:
                    self.incr_mode(self.args.password, self.args.lenght)

                else:
                    mdp = input("Enter your hash : ")
                    self.incr_mode(mdp, self.args.lenght)

            else:
                self.hash_type = input("Enter the hash type (md5/sha1/sha256) : ").lower()

                if self.args.password:
                    self.incr_mode(self.args.password, self.args.lenght)

                else:
                    mdp = input("Enter your hash : ")
                    self.incr_mode(mdp, self.args.lenght)

        elif self.args.password:
            print("""You have not entered either the password length (for an incremental attack)
            or a dictionary path (for a dictionary attack).
            Please choose either incremental mode or dictionary mode. (HashHunter --help for help) : """)
            exit()

        elif self.args.hash:
            print("""You have not entered either the password length (for an incremental attack)
                or a dictionary path (for a dictionary attack).
                Please choose either incremental mode or dictionary mode. (HashHunter --help for help) : """)
            exit()

        elif self.args.custom:
            mots = self.args.custom

            self.create_custom_dict(mots.split(" "))

        else:
            self.choix()

if __name__ == "__main__":
    hashhunter = HashHunter()
    hashhunter.debut()
