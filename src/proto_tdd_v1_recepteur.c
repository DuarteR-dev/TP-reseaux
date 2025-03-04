/*************************************************************
* proto_tdd_v1 -  récepteur                                  *
* TRANSFERT DE DONNEES  v1                                   *
*                                                            *
* Protocole sans contrôle de flux, sans reprise sur erreurs  *
*                                                            *
* E. Lavinal - Univ. de Toulouse III - Paul Sabatier         *
**************************************************************/

#include <stdio.h>
#include "application.h"
#include "couche_transport.h"
#include "services_reseau.h"

/* =============================== */
/* Programme principal - récepteur */
/* =============================== */
int main(int argc, char* argv[])
{
    unsigned char message[MAX_INFO]; /* message pour l'application */
    paquet_t paquet, ack; /* paquet utilisé par le protocole et ack */
    int fin = 0; /* condition d'arrêt */

    ack.num_seq = 0;
    ack.lg_info = 0;

    init_reseau(RECEPTION);

    printf("[TRP] Initialisation reseau : OK.\n");
    printf("[TRP] Debut execution protocole transport.\n");

    /* tant que le récepteur reçoit des données */
    while ( !fin ) {

        // attendre(); /* optionnel ici car de_reseau() fct bloquante */
        de_reseau(&paquet);
        if (generer_controle(paquet) == paquet.somme_ctrl) {

            /* extraction des donnees du paquet recu */
            for (int i=0; i < paquet.lg_info; i++) {
                message[i] = paquet.info[i];
            }
            ack.type = ACK;
            /* remise des données à la couche application */
            fin = vers_application(message, paquet.lg_info);
        }

        else {
            ack.type = NACK;
            printf("[TRP] Renvoi NACK.\n");
        }
        vers_reseau(&ack);

    }

    printf("[TRP] Fin execution protocole transport.\n");
    return 0;
}
