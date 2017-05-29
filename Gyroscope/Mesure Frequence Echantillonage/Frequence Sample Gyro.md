# Fréquence d'échantillonnage du gyroscope

## Présentation

*TODO ...*

## Expérience

### Objectif

On souhaite mesurer la fréquence effective d’échantillonnage de la centrale inertielle, afin de vérifier qu’elle correspond à celle choisie dans la librairie Arduino. Les valeurs possibles de ces fréquences sont 25, 50,100 et 200Hz – à noter que les valeurs sont apparemment bruitées *(voir commentaires librairie Arduino)* pour 200Hz.

### Protocole

La librairie I2Cdev-MPU5060 indique la valeur à donner et facilite l’envoi des bytes des différents registres du mpu afin de modifier la fréquence d’envoi de données du buffer FIFO du capteur, permettant de choisir la fréquence d’échantillonnage du capteur. 

On choisit la fréquence d’échantillonnage en modifiant la valeur du byte du registre correspondant via une requête I2C (en modifiant le code de la librairie, changement pris en compte dans l’initialisation du capteur dans le programme arduino).

On mesure le temps entre deux acquisitions capteurs et on le récupère via le moniteur série Arduino.

On calcule la moyenne sur 5 mesures capteurs.



### Résultats

On observe que les fréquences expérimentales du FIFO sont plus élevées que les valeurs effectives données par les développeurs de la librairie.

Afin d’optimiser la boucle d’asservissement, on souhaite maximiser la fréquence de lecture de données capteur. Il s’agit maintenant de vérifier ou non si les données sont suffisamment fiables, dans le cas où f=200Hz et f=100Hz.

### Remarques

En suivant le protocole, on a réalisé que le code source de mon objet gyro était incorrect : le booléen newDataReady, représentant le déclenchement de l'évènement associé à la réception d'une nouvelle trame de données provenant du gyroscope,  n’était jamais réinitialisé à la valeur false. 

Du point de vue du code global, l’objet gyro contenait a priori constamment de nouvelles valeurs pour les paramètres dynamiques du système, ce qui n’était absolument pas le cas (cette valeur s’actualise à la fréquence choisie pour le fifo du gyroscope). 

La conséquence était que le nombre de calculs effectués était beaucoup trop important sans que cela ne soit utile. On a donc ajouté une méthode setNoDataReady() permettant de réinitialiser la valeur du booléen.