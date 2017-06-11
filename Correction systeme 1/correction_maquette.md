# Correction de la maquette


L'étude sur la modélisation a permis d'identifier des paramètres pour le correcteur PID qui permettent d'avoir un système à la fois rapide et stable.

On souhaite vérifier la validité de la modélisation en paramétrant le correcteur en fonction des valeurs théoriques calculées, et en observant la réponse temporelle du système.


## Vérification des valeurs prédites de Ku et Tu


| Kd_initial | Ku_theo | Tu_theo | Ku_exp | Tu_exp | 
|------------|---------|---------|--------|--------| 
| 0,05       | 0,64    | 1,80    | 0,83   | 1,44   | 
| 0,1        | 1,28    | 1,27    | 1,28   | 1,27   | 
| 0,15       | 1,92    | 1,04    | 1,65   | 1,07   | 
| 0,2        | 2,56    | 0,90    | 2      | 0,95   | 
| 0,25       | 3,21    | 0,80    | 2,3    | 0,87   | 


On note un écart important entre les valeurs prédites de Ku et Tu, exploitant les relations découlant du critère de Routh, et la modélisation du système, adapté à la mesure de Ku et Tu réalisée précédemment.

On arrête alors toute tentative de correction du système avant d'améliorer au maximum la modélisation de la maquette.


## Amélioration 2 du modèle de connaissance

Le couple (K, tau) déterminé précédemment n'était déterminé qu'à partir de la mesure de Ku et Tu pour une unique valeur Kd de départ.

On dispose maintenant d'un total de 5 acquisitions de Ku et Tu.

En réalisant une régression linéaire et en exploitant les relations du critère de Routh, on pourra trouver un couple (K,tau) moins entâché des erreurs de mesures (difficile de déterminer si l'on se trouve pile au point de juste instabilité...).


### Résultats de la régression linéaire

On rappelle les équations :

- Ku = Kd/tau

- Tu = 2*pi*sqrt(tau/(K*Kd))


Les régressions linéaires des fonctions Ku(Kd) et Tu(Kd) montrent clairement qu'un modèle affine convient mieux pour Ku.

On ne peut pas expliquer cette différence avec la fonction de transfert du système proposée.


Deux problèmes de modélisation sont distingables :

- la modélisation des moteurs par un premier ordre (a priori un MCC est un second ordre)

- le caractère continu du système (alors qu'il est discret, de période de boucle égale à la période d'échantillonage du gyroscope)


On propose, à partir de la régression linéaire, de faire l'hypothèse que les coefficients linéaires dépendent bien uniquement des paramètres impliqués dans les équations énoncées plus haut.


Les nouvelles équations sont :

- Ku = Kd/tau + Ku0

- Tu = 2*pi*sqrt(tau/(K*Kd))

On obtient alors un nouveau couple :

(K, tau) = (31.4 ; 0.137)

Et Ku0 = 0.514

Le nouveau coefficient correctif est donc :

Kcorrectif = 6.29