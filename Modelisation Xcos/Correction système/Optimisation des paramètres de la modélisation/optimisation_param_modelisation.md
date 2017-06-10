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

Lorsque l'on fixe Kp = 1, on reste en régime stable. Vraisemblablement la valeur de tau, qui n'était qu'un ordre de grandeur, n'est pas exactement égale à 0.1s.

Le régime juste instable est atteint en :

Kp = 1.28

T = 1.27s

On trouve alors pour ces valeurs un couple 

(tau_exp ; K_exp) = (0.078s ; 5.09)

L'écart entre tau_exp et la valeur estimée était assez prévisible. Néanmoins l'ordre de grandeur était correct.

L'écart entre le gain K_exp et le gain théorique K_théo est :

delta(K) = (K_exp-Kthéo)/K_exp = 2.7 %

C'est un écart tout à fait acceptable, et conforte grandement la démarche de modélisation et les hypothèses formulées.



## Conclusion


- L'utilisation du critère de Routh a permis d'obtenir deux conditions sur le gain de boucle et la constante de temps du système afin de placer le système dans un régime juste instable.


- Ces conditions de juste instabilité, une fois respectées dans la modélisation causale du système, rendent en effet le système oscillant en théorie, ce que l'on peut observer dans la réponse temporelle du signal.


- On se sert alors de ces deux conditions pour déterminer le gain expérimental et la constante de temps de la fonction de transfert.


- La constante de temps mesurée remplace l'ordre de grandeur que l'on avait posé dans l'étape de modélisation du moteur.


- L'écart entre le gain mesuré et le gain théorique (résultant de l'ensemble des étapes de modélisation des composants) est égal à 2.7%.


- On utilise un gain correctif afin de modéliser au mieux le système réel : ce gain sera pris : Kcorrectif = K_exp/K_théo = 1.027.