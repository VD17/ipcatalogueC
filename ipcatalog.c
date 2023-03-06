/*
 * TP IP Catalogue
 * Autheur : Vishal Desai, Thibaut Guesdon, Benjamin Francisco
 *
 * Date : 2023-03-05
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define longeur_max_ip 16
#define cidr 2
#define arr_cidr 32

// structure pour stocker les adresses IP et les masques de sous-réseau
typedef struct {
    char str[longeur_max_ip + 1];
    unsigned int num;
} ip_t;

typedef struct {
    char str[cidr + 1];
    unsigned int num;
} masque_t;

//tableaux pour stocker les adresses IP et les masques de sous-réseau
ip_t ips[100];
masque_t masques[100];

// compteur pour le nombre d'adresses IP et de masques de sous-réseau stockées dans les tableaux
int ip_count = 0;
int masque_count = 0;

// fonction pour clear la console
void clear_console() {
    system("clear || cls");
}

// fonction pour ajouter une adresse IP
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

// fonction pour ajouter un masque de sous-réseau
void add_masque(char * masque) {
    if (masque_count < arr_cidr) {
        strcpy(masques[masque_count].str, masque);

        // conversion du masque de sous-réseau en nombre pour une manipulation plus facile
        unsigned int num = 0;
        for (int i = 0; i < atoi(masque); i++) {
            num += (int) pow(2, 31 - i);
        }
        masques[masque_count].num = num;

        masque_count++;
    }
}

// fonction pour sauvegarder les adresses IP dans un fichier
void save_ips() {
    FILE * file = fopen("IP.txt", "w");
    if (file) {
        for (int i = 0; i < ip_count; i++) {
            fprintf(file, "%s\n", ips[i].str);
        }
        fclose(file);
    }
}

// fonction pour charger les adresses IP à partir d'un fichier
void load_ips() {
    FILE * file = fopen("IP.txt", "r");
    if (file) {
        char line[longeur_max_ip + 1];
        while (fgets(line, sizeof(line), file)) {
            line[strlen(line) - 1] = '\0';
            add_ip(line);
        }
        fclose(file);
    }
}

// fonction pour sauvegarder les masques de sous-réseau dans un fichier
void save_masque() {
    FILE * file = fopen("masque.txt", "w");
    if (file) {
        for (int i = 0; i < masque_count; i++) {
            fprintf(file, "%s\n", masques[i].str);
        }
        fclose(file);
    }
}

void load_masque() {
    FILE * file = fopen("masque.txt", "r");
    if (file) {
        char line[cidr + 1];
        while (fgets(line, sizeof(line), file)) {
            line[strlen(line) - 1] = '\0';
            add_masque(line);
        }
        fclose(file);
    }
}

//fonction permettant de supprimer un IP de la liste
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

//fonction qui permet d'afficher les adresses ip lors du listing de celles-ci
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

//filtrage des IPs en fonction de leur masque
void filter_ips(char * masque) {
    for (int i = 0; i < masque_count; i++) {
        if (strcmp(masques[i].str, masque) == 0) {
            printf("%s\n\n", ips[i].str);
        }
    }

}

//PROGRAMME
int main(int argc, char ** argv) {
    int choice = 0;
    load_ips();
    load_masque();

    //boucle permettant de vérifier que les instructions rentrées sont bonnes, si une instruction est fausse on relance la boucle
    do {
        printf("\n");
        printf("Menu Ip Catalogue ?\n\n");
        printf("1 - Ajouter une adresse IP\n");
        printf("2 - Liste des adresses IP\n");
        printf("3 - supprimer une adresses IP\n");
        printf("4 - Filtre masque\n");
        printf("5 - Quitter\n");

        if (scanf("%d", &choice) != 1) {
            printf("Erreur: vous devez entrer un entier.\n");
            return 0;
        }

        //Switch qui va permettre de définir l'action selectionnée par l'utilisateur parmis celles énumérées au dessus
        switch(choice) {
            case 1: {
                clear_console();
                printf("Entrez une adresse IP (Ex.: 10.93.0.0): ");
                char ip[longeur_max_ip + 1];
                scanf("%s", ip);
                add_ip(ip);
                printf("Entrez un masque reseau en CIDR : ");
                char masque[cidr + 1];
                scanf("%s", masque);
                add_masque(masque);
                break;
            }
            case 2: {
                clear_console();
                printf("Liste des adresses IP :\n");
                for (int i = 0; i < ip_count; i++) {
                    display_ip(ips[i], masques[i]);
                    printf("\n");
                }
                break;
            }
            case 3: {
                clear_console();
                printf("Liste des adresses IP :\n");
                for (int i = 0; i < ip_count; i++) {
                    display_ip(ips[i], masques[i]);
                    printf("\n");
                }

                printf("\nsupprimer une adresse IP (Ip dans la liste) : ");
                char ip[longeur_max_ip + 1];
                scanf("%s", ip);
                remove_ip(ip);
                break;
            }
            case 4: {
                clear_console();
                printf("Filtrage IP par masque CIDR: \n");
                printf("Entrez un masque reseau en forme CIDR: ");
                char masque[cidr + 1];
                scanf("%s", masque);
                printf("\nListe des adresse ip avec le masque reseau : /%s\n\n", masque);
                filter_ips(masque);
                break;
            }
            case 5: {
                clear_console();
                save_ips();
                save_masque();
                printf("Au revoir !\n");
                return 0;
            }

            default: {
                clear_console();
                printf("Choix invalide\n");
                break;
            }
        }
    } while (1);
}