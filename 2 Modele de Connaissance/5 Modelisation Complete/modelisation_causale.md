# Modélisation Causale de la maquette de drone


## Description

On a déterminé l'ensemble des paramètres de la fonction de transfert en boucle ouverte du système : 

- Km = 954 rad.s-1
- tau = 0.1s
- Kh_lin = 1,34E-3 N.(rad.s^-1)^-1
- R_i_0 = 1,348 N
- l = 0.22 m
- Jeq = 9,53E-3 kg.m^2


La fonction de transfert en boucle ouverte non corrigée est égale à :

H(p) = (2*Km*Kh_lin*l^2)/(p^2*(1+tau*p))

soit H(p) = K/(p^2*(1+tau*p))

où K = Kcons*2*Km*Kh_lin*l^2/Jeq

AN : K = 2,48 (SU) ; tau = 0.1s



On décide d'utiliser le module Xcos de Scilab afin de tracer le schéma-bloc fonctionnel du système, et pouvoir récupérer la réponse temporel du système non corrigé, puis corrigé.

La modélisation servira alors de support pour valider ou non la synthèse d'un correcteur efficace.

Xcos est un outil de modélisation mathématique graphique et de simulation de systèmes dynamiques, distribué sous license libre. Son utilisation entre dans le cadre des SII en classe préparatoire scientifique, ce qui justifie de l'avoir choisi.


## Constituants

### Génération de la consigne

La modélisation doit être capable de gérer des consignes de différents types : 

- constante égale à 0

- échelon : initialement nulle, puis non nulle

Les signaux sont générés en degrés puis convertis en radians avant d'entrer dans un gain d'adaptation (unitaire : signal réel / signal numérique), puis le sommateur


### Correction

- conversion radians / degrés

- génération de consigne pour Kp = 1 : Kcons = 0.30/90 : i.e. si l'écart entre l'angle de consigne et l'angle réel de roulis de la maqette vaut 90°, la consigne proportionnelle sera égale à 30%.

- correcteur parallèle Kp - Ki - Kd


### Consigne moteur - helice - couple

- commande différentielle des moteurs

- fonction de transfert du moteur

- gain d'hélice, linéarisé autour du point de fonctionnement pour C=C0=0.40 (Kh_lin)

- paramètre géométrique (distance à l'axe) d^2 pour le calcul du couple

- sommateur pour obtenir la somme des couples s'appliquant au système


### Equation dynamique

- transformée de laplace de l'équation du TRD (en 0, selon l'axe de roulis) (implique l'inertie Jeq du système)


### Ajout de perturbations

- impulsionnelle (modélisée par un trapèze : plus proche d'une perturbation réelle qu'un dirac) (décalage de la maquette d'un certain angle)


### Boucle de retour

- Gain de capteur (unitaire : convertit l'angle réel en un angle en valeur numérique)


### Visualisation du signal

- convertisseur Kdeg

- Scopes permettant de visualiser la sortie, et chaque consigne / perturbation