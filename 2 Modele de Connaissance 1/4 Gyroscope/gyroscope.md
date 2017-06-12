# Gyroscope


## Description

Pour que le drone puisse se maintenir en vol stationnaire, il est nécessaire que la commande du système dépendent d'un retour de sa position. On utilise pour cela un capteur, implémenté sur la maquette. Ici seul un capteur gyroscopique suffit, mais ceux-ci sont généralements disponibles sur des capteurs à plusieurs axes comme les capteurs 6-axes comportant 3 accéléromètres et 3 gyromètres MEMS.

http://www.siliconsensing.com/technology/mems-gyroscopes/
http://www.siliconsensing.com/technology/mems-accelerometers/


## Modélisation

Il n'est pas nécessaire de modéliser le gyroscope : son fonctionnement est relativement complexe car une unité de calcul est dédié au filtrage des différentes données afin de pouvoir fournir avec une précision maximale les données attendues d'un tel capteur.

On considèrera le capteur 6-axes comme une boite noire, restituant fidèlement la position du système, et dont la fréquence d'échantillonnage est connue.

Ainsi, les gains du capteur et de l'adaptateur de consigne sont pris unitaire : la transcription entre grandeur réelle et grandeur numérique s'effectuant de façon transparente pour l'utilisateur par le système d'exploitation de l'ordinateur et de la carte de commande et capteur.