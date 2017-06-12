# Optimisation de la valeur des paramètres de la modélisation



## Description

On a pu déterminer analytiquement l'expression de la fonction de transfert du système en boucle ouverte sans correcteur :

H(p) = K/(p^2(1+tau*p))

Où :

- K = 4.955 (SU)

- tau = 0.01s


Le système possède un déphasage strictement supérieur à pi : il est intrinsèquement instable.

Sans l'action d'un dérivateur, le système oscille spontanément.

L'ajout d'un correcteur proportionnel-dérivateur PD rend possible l'obtention d'un système stable en boucle fermé : en effet, si l'on applique le critère de Routh à la fonction de transfert du système corrigé en boucle fermé, il vient deux conditions sur les paramètres du correcteur Kp et Kd à respecter : 

- Kp > 0

- Kd > tau * Kp

Ainsi, le point critique du système dépend du coefficient Kd fixé au départ.


Le système atteint un régime critique d'oscillation auto-entretenues lorsque le dénominateur de la fonction de transfert s'annule. Il vient alors deux relations sur le gain critique et la pulsation des oscillations dans ce régime :

- Ku = Kd / tau

- Tu = 2*pi*sqrt(tau/(K*Kd))


Afin de modéliser le système de façon la plus précise possible - les étapes de modélisation des composants ayant permis d'obtenir un premier modèle de comportement - , on utilise ces deux équations.

On dispose en effet de deux équations indépendantes permettant de déterminer les deux paramètres de la fonction de transfert : K et tau.



## Protocole


Sur la maquette : 

- On fixe Kd pour plusieurs valeurs, prises dans {0.1 ; 0.15 ; 0.2 ; 0.25 ; 0.3} (le comportement de la modélisation, non ajustée, est correct pour ces valeurs)

- On augmente Kp jusqu'à atteindre le régime juste instable

- On lance l'aquisition du signal afin de mesurer la période des oscillations T


Une fois les résultats obtenus, on effectue une régression linéaire afin d'identifier les paramètres K et tau, et valider ou non le modèle théorique.



## Résultats


| Kd_initial | Kd_root_-1  | Ku_exp | Tu_exp | 
|------------|-------------|--------|--------| 
| 0,05       | 4,472135955 | 0,83   | 1,44   | 
| 0,1        | 3,16227766  | 1,28   | 1,27   | 
| 0,15       | 2,581988897 | 1,65   | 1,07   | 
| 0,2        | 2,236067977 | 2      | 0,95   | 
| 0,25       | 2           | 2,3    | 0,87   | 



Régression affine sur Ku(Kd) : 

- Ku = A*Kd + B avec A = 7,32; B = 0,514; R² = 0,994


Régression linéaire sur Tu(1/sqrt(Kd)) : 

- Tu = C/sqrt(Kd) avec C = 0,415; R² = 0,999


Les régressions linéaires des fonctions Ku(Kd) et Tu(1/sqrt(Kd)) montrent clairement qu'un modèle affine convient mieux pour Ku.

On ne peut pas expliquer cette différence avec la fonction de transfert du système proposée.


Deux problèmes de modélisation sont distingables :

- la modélisation des moteurs par un premier ordre (a priori un MCC est un second ordre)

- le caractère continu du système (alors qu'il est discret, de période de boucle égale à la période d'échantillonage du gyroscope)


On propose, à partir de la régression linéaire, de faire l'hypothèse que les coefficients linéaires dépendent bien uniquement des paramètres impliqués dans les équations énoncées plus haut.


Les nouvelles équations sont :

- Ku = Kd/tau + Ku0

- Tu = 2*pi*sqrt(tau/(K*Kd))


On obtient alors un couple de paramètres pour la fonction de transfert :

(K, tau) = (31.4 ; 0.137)

Et un coefficient Ku0 = 0.514 pour l'estimation de Ku.


On ajoute donc un coefficient correctif à la modélisation :

Kcorrectif = 6.29

Ce coefficient est assez important : la modélisation théorique est visiblement assez éloignée de la maquette réelle. C'était assez prévisible, puisque certains calculs comme celui de l'inertie du système, et du coefficient de poussée de l'hélice sont basés sur des hypothèses assez fortes.



## Conclusion


- L'utilisation du critère de Routh a permis d'obtenir deux conditions sur le gain de boucle et la constante de temps du système afin de placer le système dans un régime juste instable.


- Ces conditions de juste instabilité, une fois respectées dans la modélisation causale du système, rendent en effet le système oscillant en théorie, ce que l'on peut observer dans la réponse temporelle du signal.


- La régression linéaire sur Ku(Kd) montre qu'un modèle affine correspond mieux. On détermine alors une constante corrective Ku0, et en supposant que le coefficient directeur dépend des mêmes paramètres que ceux de l'équation du critère de Routh, on obtient la mesure de tau. Celle sur Tu(1/sqrt(Kd)) est acceptable et permet de déterminer le gain K.


- On utilise un gain correctif afin de modéliser au mieux le système réel : ce gain sera pris : Kcorrectif = K_exp/K_théo = 6.29. 


- Ce correctif est assez grand : le modèle de connaissance n'est pas assez précis pour décrire le comportement réel du système. Il faudra revoir la détermination des paramètres des différentes relations associées aux composants si l'on souhaite diminuer l'écart entre modèle de connaissance et modèle expérimental.