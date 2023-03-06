# 3SI1ProjetC

Contexte :

Création d'un utilitaire permettant de cataloguer les adresses IP(v4).
L'utilitaire doit permettre de:
* Stocker des adresses IP
* Filtrer les adresses via un masque
* Afficher une adresse IP sous sa forme binaire, hexa, ou décimale, et indiquer s'il s'agit d'une IP publique,privée, ou spéciale(ex: Loopback)
Les adresses Ip doivent être enregistrées lors de la fermeture du programme et chargées a l'ouverture du
programme : elles doivent être persistantes. Le choix du système de persistance est laissé aux élèves.

Notre Menu de selection ce présente cette forme :
```
Menu Ip Catalogue ?

1 - Ajouter une adresse IP
2 - Liste des adresses IP
3 - supprimer une adresses IP
4 - Filtre masque
5 - Quitter
```

Fonctionnalités Les fonctionnalités principales de ce programme sont les suivantes :

Ajouter des IP au catalogue

```
void add_ip(char * ip) {
    if (ip_count < 100) {
        strcpy(ips[ip_count].str, ip);

        //conversion de l'adresse IP en nombre pour une manipulation plus facile
        unsigned int num = 0;
        char * token = strtok(ip, ".");
        for (int i = 0; i < 4; i++) {
            num += atoi(token) * (int) pow(256, 3 - i);
            token = strtok(NULL, ".");
        }
        ips[ip_count].num = num;

        ip_count++;
    }
}
```

Supprimer des IP du catalogue

```
void remove_ip(char * ip) {
    int index = -1;// un tableau commençant à 0, on fait -1 pour récupérer l'ip recherchée

    //recherche de l'ip dans le tableau
    for (int i = 0; i < ip_count; i++) {
        if (strcmp(ips[i].str, ip) == 0) {
            index = i;
            break;
        }
    }

    //on verifie si l'adresse existe et si oui on la supprime et on réarrange le tableau pour ne pas laisser une case vide et donc faire un saut dedans
    if (index >= 0) {
        for (int i = index; i < ip_count - 1; i++) {
            ips[i] = ips[i + 1];
            masques[i] = masques[i + 1];
        }
        ip_count--;
    }
}
```

Filtrer les IP par masque

```
void filter_ips(char * masque) {
    for (int i = 0; i < masque_count; i++) {
        if (strcmp(masques[i].str, masque) == 0) {
            printf("%s\n\n", ips[i].str);
        }
    }
}
```

Afficher des détails sur une IP sélectionnée

```
void display_ip(ip_t ip, masque_t masque) {
    printf("-------------------------------------\n");
    printf("Adresse IP : %s/%s\n", ip.str, masque.str);

    //afficher l'adresse en binaire
    printf("Binaire : ");
    for (int i = 0; i < 32; i++) {
        if (ip.num & (1 << (31 - i))) {
            printf("1");
        } else {
            printf("0");
        }
    }
    printf("\n");

    //afficher l'adresse en Hexadecimal
    printf("Hexadecimal : 0x%X\n", ip.num);

    //afficher l'adresse en decimal
    printf("Decimal : %u\n", ip.num);

    //vérification de la catégorie de l'IP (Publique, privée ou spéciale)
    if (((ip.num & 0xFF000000) == 0x0A000000) && (strcmp(masque.str, "8") == 0)) {
        printf("Adresse privee (10.0.0.0/8)\n");
    } else if (((ip.num & 0xFFF00000) == 0xAC100000) && (strcmp(masque.str, "12") == 0)) {
        printf("Adresse privee (172.16.0.0/12)\n");
    } else if (((ip.num & 0xFFFF0000) == 0xC0A80000) && (strcmp(masque.str, "16") == 0)) {
        printf("Adresse privee (192.168.0.0/16)\n");
    } else if (ip.num == 0x7F000001) {
        printf("Adresse speciale (loopback)\n");
    } else {
        printf("Adresse publique\n");
    }
    printf("-------------------------------------\n");
}
```

Le programme assure également une persistence des données entre les différentes sessions d'utilisation, c'est-à-dire que les données sont sauvegardées à la fermeture du programme et restaurées à l'ouverture suivante.

```
void save_ips() {
    FILE * file = fopen("IP.txt", "w");
    if (file) {
        for (int i = 0; i < ip_count; i++) {
            fprintf(file, "%s\n", ips[i].str);
        }
        fclose(file);
    }
}

void save_masque() {
    FILE * file = fopen("masque.txt", "w");
    if (file) {
        for (int i = 0; i < masque_count; i++) {
            fprintf(file, "%s\n", masques[i].str);
        }
        fclose(file);
    }
}
```

README Ce projet développé en langage C est un programme permettant de gérer un catalogue d'IP.