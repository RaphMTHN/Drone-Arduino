# Correction du système


## Description

L'étape de modélisation du système a permis d'identifier la fonction de transfert en boucle ouverte. Celle-ci prend finalement la forme suivante :

H(p) = K/(p^2(1+tau*p))

où :

- K = 31.4 (SU)
- tau = 0.137s


Afin de rendre le système stable, on a montré qu'il suffisait d'implémenter un correcteur proportionnel-dérivateur PD dont les coefficients respectent les conditions obtenues à l'aide du critère de stabilité de Routh.

Cependant, comme les exigences du vol stationnaire impose au système d'être précis en régulation lorsqu'une perturbation est d'ordre 1 (échelon : par exemple couple constant de perturbation appliqué  au système total), il est nécessaire d'avoir un intégrateur dans le correcteur.

Le critère de Routh une nouvelle fois appliqué au système présentant un tel correcteur fournit cette fois trois conditions à respecter pour être stable de façon absolue.

Pourtant il est nécessaire dans la synthèse d'un correcteur d'avoir une certaine marge de stabilité : sans quoi le système risque d'être trop proche du point d'instabilité, et a fortiori trop oscillant.



## La méthode de Ziegler Nichols


Il s'agit d'une méthode simple de détermination des coefficients du correcteur PID à partir de mesures réalisées au point de juste instabilité du système non corrigé.

Les paramètres proposés par Ziegler et Nichols résultent d'une analyse empirique de systèmes asservis, et permettent d'obtenir une marge de stabilité suffisamment importante pour avoir un système assez peu oscillant et suffisamment réactif en régulation.


Les paramètres à déterminer sont :

- La période du système au point de juste instabilité (oscillations entretenues) : Tu

- Le gain du correcteur proportionnel nécessaire à l'obtention de ce régime juste instable : Ku


Le réglage du correcteur PID série suit alors la règle suivante :

| Kp    | Ki       | Kd          | 
|-------|----------|-------------| 
| 0.6Ku | 1.2Ku/Tu | 0.075*Ku*Tu |


On reconnait immédiatement les paramètres imposés par le critère de Routh, qui justifie analytiquement la juste instabilité du système.


La condition d'application de la méthode euristique de Ziegler et Nichols est que le système soit stable avant implémentation du correcteur. Cela pose un problème, face à ce qui a été énoncé sur l'instabilité du système.

On décide donc d'appliquer la méthode de Ziegler Nichols au système, déjà corrigé par une action dérivatrice Kd.



## Correction du modèle adapté à la maquette


### Protocole

Pour différentes valeurs de Kd :

- On fixe la valeur de Kd

- On calcule analytiquement les valeurs de Ku et Tu grâce aux relations empiriques obtenues après régression linéaire

- On applique la méthode de Ziegler Nichols et on trace la réponse temporelle du système pour une consigne en échelon (après 1s : -10° -> 0°)

Puis on compare les réponses obtenues pour voir l'influence de Kd sur la réponse du système corrigé par ZN.


### Résultats

On a pris Kd dans {0.1 ; 0.2 ; 0.3 ; 0.4 ; 0.5 ; 1 ; 2}

On étudie qualitativement les réponses temporelles du système :

On peut associer :

- la stabilité au nombre d'oscillations et à leurs amplitudes
- la rapidité au temps de retour à l'équilibre

On constate alors que tant que Kd est inférieur à 0.5, la stabilité et la rapidité s'améliorent lorsque Kd augmente.

Au delà de 0.5, la rapidité se stabilise et la stabilité diminue avec Kd.


Le meilleur compromis entre rapidité et stabilité semble être atteint pour un Kd initial égal à 0.5.