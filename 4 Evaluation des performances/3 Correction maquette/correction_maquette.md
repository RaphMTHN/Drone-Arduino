# Correction du système



## Description

On a ajusté la modélisation de la maquette en réalisant un total de 5 acquisitions des paramètres (Ku,Tu).

On souhaite maintenant implémenter le correcteur PID avec les coefficients calculés à l'aide de la méthode de Ziegler Nichols, et valider ou non la correction du système.



## Protocole

- On implémente le correcteur pour les valeurs déterminées à partir des lois semi-empiriques de Tu et Ku

- On trace la réponse temporelle du système :
	
	- avec une consigne en échelon de -10° vers +10°

	- avec une perturbation de type impulsionnelle, initialement à 0°

- On compare les réponses de la modélisation Xcos aux mêmes entrées

- On dresse un tableau comparatif des écarts mesurables

Les acquisitions à réaliser sont : 

| Kd_initial | Ku   | Tu   | Kp   | Ki   | Kd   | 
|------------|------|------|------|------|------| 
| 0,10       | 1,24 | 1,68 | 0,75 | 0,89 | 0,26 | 
| 0,15       | 1,61 | 1,37 | 0,97 | 1,40 | 0,32 | 
| 0,20       | 1,97 | 1,19 | 1,18 | 1,99 | 0,38 | 
| 0,25       | 2,34 | 1,06 | 1,40 | 2,64 | 0,44 | 
| 0,30       | 2,70 | 0,97 | 1,62 | 3,34 | 0,50 | 



## Résultats

On a retenu les essais réalisés pour Kd initialement pris égal à 1.5.


### Réponse indicielle

|              | depassement_max (%) | temps_reponse_10% (s) | 
|--------------|---------------------|-----------------------| 
| modelisation | 61                  | 1,3                   | 
| maquette     | 39                  | 1,9                   | 


### Réponse en régulation


|              | depassement_max (%) | temps_reponse_10% (s) | 
|--------------|---------------------|-----------------------| 
| modelisation | 55                  | 0,85                  | 
| maquette     | 55                  | 1                     |



### Analyse des résultats


En régulation ou en suivi de consigne, le système modélisé est plus rapide que le système réel.

La maquette, cependant, a des dépassements moins importants en réponse indicielle que la modélisation. 

L'étude qualitative de la réponse semble indiquer que des phénomènes non linéaires non pris en compte dans la modélisation interviennent de façon non négligeable.

Des petites perturbations agissent sur la maquette réelle (vibrations, environnement), ce qui explique pourquoi le système ne converge pas exactement vers la consigne mais oscille légèrement de façon aléatoire. Leurs amplitudes ne dépassent pas les 3°, ce qui est tout à fait acceptable pour un premier cycle de développement de système.