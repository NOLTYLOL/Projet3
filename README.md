# Projet 3 — Stéganographie BMP (C++20)

Programme en ligne de commande permettant de dissimuler et d'extraire un message texte dans une image **BMP 24 bits non compressée** via la méthode du bit de poids faible (**LSB**), avec chiffrement **XOR** optionnel.

### 1. Afficher l'aide
```bash
Projet3.exe help
```

### 2. Copier un fichier BMP (vérification de lecture/écriture)
```bash
Projet3.exe copy <image_source.bmp> [image_destination.bmp]
```
*(Si aucune destination n'est précisée, le fichier est copié sous le nom `copy.bmp`.)*

### 3. Cacher un message (`encode`)
Sans chiffrement :
```bash
Projet3.exe encode input/image.bmp output/secret.bmp "message secret"
```

Avec chiffrement XOR (clé optionnelle) :
```bash
Projet3.exe encode input/image.bmp output/secret.bmp "message secret" "cle"
```

### 4. Extraire un message (`decode`)
Sans clé :
```bash
Projet3.exe decode output/secret.bmp
```

Avec clé XOR :
```bash
Projet3.exe decode output/secret.bmp "cle"
```
## Structure du projet

- `main.cpp` : Point d'entrée du programme.
- `cli.h` / `cli.cpp` : Analyse et validation des arguments de la ligne de commande.
- `bmploader.h` / `bmploader.cpp` : Lecture, validation d'un fichier BMP 24 bits.
- `message.h` / `message.cpp` : Insertion/extraction LSB, vérification de la capacité et chiffrement XOR.
