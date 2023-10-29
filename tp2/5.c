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
#include <sched.h>


static timer_t timer = (timer_t) 0;
static struct timespec *mesure = NULL;
static int nb_mesures = 0;
static int nb_total_mesures = 0;

void handler_signal(int inutilise)
{
    struct timespec heure;
    static struct timespec precedente = {0, 0};

    // Mesurer le temps
    clock_gettime(CLOCK_REALTIME, &heure);

    if (precedente.tv_sec > 0) {
        // Calculer la différence entre l'instant actuel et la mesure précédente
        mesure[nb_mesures].tv_sec = heure.tv_sec - precedente.tv_sec;
        mesure[nb_mesures].tv_nsec = heure.tv_nsec - precedente.tv_nsec;

        if (mesure[nb_mesures].tv_nsec < 0) {
            mesure[nb_mesures].tv_sec--;
            mesure[nb_mesures].tv_nsec += 1000000000;
        }
    }

    precedente = heure;

    nb_mesures++;
    if (nb_mesures == nb_total_mesures)
        timer_delete(timer);
}

int main(int argc, char *argv[])
{
    //set process to realtime priority
    struct sched_param param;
    param.sched_priority = 50;

    if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
        perror("Erreur de sched_setscheduler");
        return 1;
    }


    long int periode;
    struct sigevent event;
    struct itimerspec spec;

    if ((argc != 2) || (sscanf(argv[1], "%ld", &periode) != 1)) {
        fprintf(stderr, "usage: %s periode_en_microsec\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    nb_total_mesures = 5000000 / periode; // 5 secondes

    // Configuration du timer
    signal(SIGRTMIN, handler_signal);

    event.sigev_notify = SIGEV_SIGNAL;
    event.sigev_signo = SIGRTMIN;

    periode = periode * 1000;

    spec.it_interval.tv_sec = 0;
    spec.it_interval.tv_nsec = periode;
    spec.it_value.tv_sec = 0;
    spec.it_value.tv_nsec = periode;

    if (timer_create(CLOCK_REALTIME, &event, &timer) != 0) {
        perror("timer_create");
        exit(EXIT_FAILURE);
    }

    mesure = (struct timespec *)malloc(nb_total_mesures * sizeof(struct timespec));
    if (mesure == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    if (timer_settime(timer, 0, &spec, NULL) != 0) {
        perror("timer_settime");
        exit(EXIT_FAILURE);
    }

    while (nb_mesures < nb_total_mesures)
        pause();

    for (nb_mesures = 0; nb_mesures < nb_total_mesures; nb_mesures++) {
        // Afficher les mesures en microsecondes
        long int microsec = mesure[nb_mesures].tv_sec * 1000000 + mesure[nb_mesures].tv_nsec / 1000;
        printf("%ld\n", microsec);
    }

    free(mesure);
    return EXIT_SUCCESS;
}
