// ---------------------------------------------------------------------
// Fichier d'exemple pour le TP2 Question 4-a --> création des timers
// VO par Christophe BLAESS
// Adapté par Jalil Boukhobza
// ---------------------------------------------------------------------

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

static timer_t timer = (timer_t) 0;
static long int *mesure = NULL;
static int nb_mesures = 0;
static int nb_total_mesures = 0;

void handler_signal(int inutilise)
{
    // Fonction à exécuter à chaque expiration du timer
    // Incrémenter le nombre de mesures
    nb_mesures++;
    printf("Timer expired, mesures: %ld\n", nb_mesures);

    if (nb_mesures >= nb_total_mesures) {
        // Sortir du programme lorsque le nombre de mesures désiré est atteint.
        exit(EXIT_SUCCESS);
    }
}

int main(int argc, char *argv[])
{
    long int periode;
    struct sigevent event;
    struct itimerspec spec;

    if ((argc != 2) || (sscanf(argv[1], "%ld", &periode) != 1)) {
        fprintf(stderr, "usage: %s periode_en_microsec\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Nombre de mesures réalisées en 5 secondes (par exemple)
    nb_total_mesures = 5000000 / periode; // 5 secondes

    // Configurer le timer
    // À la réception du signal SIGRTMIN, exécuter la fonction handler_signal()
    signal(SIGRTMIN, handler_signal);

    // Configurer la structure sigevent pour la notification par signal
    event.sigev_notify = SIGEV_SIGNAL;
    event.sigev_signo = SIGRTMIN;

    periode = periode * 1000;  // en nanosec

    // Définir la période du timer
    spec.it_interval.tv_sec = 0; // Période en secondes (partie entière)
    spec.it_interval.tv_nsec = periode; // Période en nanosecondes (partie fractionnaire)
    spec.it_value = spec.it_interval;

    // Allouer le timer
    if (timer_create(CLOCK_REALTIME, &event, &timer) != 0) {
        perror("timer_create");
        exit(EXIT_FAILURE);
    }

    // Programmer le timer
    if (timer_settime(timer, 0, &spec, NULL) != 0) {
        perror("timer_settime");
        exit(EXIT_FAILURE);
    }

    // Attendre le dernier signal
    while (nb_mesures < nb_total_mesures)
        pause();

    return EXIT_SUCCESS;
}
