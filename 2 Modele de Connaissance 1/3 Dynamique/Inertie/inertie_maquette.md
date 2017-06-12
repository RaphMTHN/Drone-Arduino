# Calcul de l'inertie équivalente de la maquette

## Modélisation

Pour calculer l'inertie équivalente de la maquette par rapport à l'axe de roulis, on fait les hypothèses suivantes :

- La breadboard et le capteur gyroscopique sont de masse suffisamment faible et suffisamment proche de l'axe pour être d'inertie négligeable

- la tige servant de chassis est supposé uniforme, de masse volumique égale à celle de l'aluminium brut

- l'ensemble ESC + moteur + hélice peut être modélisé par un système ponctuel situé à la même distance de l'axe que le centre de l'axe de rotation du moteur


## Résultat du calcul

*Formule Latex*

Après calcul, on trouve une inertie :

**Jeq = 4,77E-3 kg.m^2**

On utilisera cette valeur dans la modélisation causale du système complet.