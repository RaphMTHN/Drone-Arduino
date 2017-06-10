# Optimisation de la valeur des paramètres de la modélisation

## Description

Afin de modéliser le système de façon la plus précise - les étapes de modélisation des composants ayant permis d'obtenir un premier modèle de comportement proche de celui du système réel (écarts ?), on utilise deux équations récupérées à l'aide du critère de Routh :

- La condition de juste instabilité permet de récupérer expérimentalement la valeur de la constante de temps tau, en connaissant Kp et Kd

condition : Kp = Kd / tau

- La mesure de la période du système en régime juste instable permet de récupérer la valeur du gain de boucle total K

condition : pulsation telle que le dénominateur s'annule : omega = sqrt(K*Kd/tau) (A VERIFIER PAR LA MODELISATION)


## Protocole


Sur la modélisation :

- On fixe Kd à 0.1s

- On fixe Kp = 1

- On mesure T pour vérifier que la valeur est bien égale à la valeur théorique afin de conforter le critère de Routh utilisé


Sur la maquette : 

- On fixe Kd arbitrairement à 0.1s

- On augmente Kp jusqu'à atteindre le régime juste instable

- On lance l'aquisition du signal afin de mesurer la période des oscillations T


Les valeurs théoriques sont :

Kp = Kd/tau

T = 2*pi*sqrt(tau/(K*Kd))

K = 4.955 (SU) ; tau = 0.1s

Donc couple théorique (Kp ; T) = (1 ; 2.82s)


## Résultats


Sur la modélisation :

Kp = 1 : on est bien en régime juste instable

T = 2.821s à l'aide des curseurs sur le logiciel : c'est totalement conforme à la valeur théorique.


Sur la maquette :

Kp = 

T = 

## Conclusion