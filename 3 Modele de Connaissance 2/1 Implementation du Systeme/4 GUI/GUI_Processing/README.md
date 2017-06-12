# GUI Processing pour la maquette de drone


## Utilisation

1. Pour utiliser l'interface graphique, il est nécessaire d'avoir installé au préalable Processing et de s'être assuré que les drivers graphiques sont installés et compatible avec la version installée de l'IDE.

2. Veillez à bien copier le contenu du dossier libraries, contenant les bibliothèques indispensables au fonctionnement du GUI, à l'intérieur du dossier de même nom, du *sketchbook folder*.


## Enregistrement des données

Les données sont enregistrés au format CSV afin de faciliter leur intégration dans les différents programmes compatibles. On a choisi d'utiliser le caractère semicolon ';' pour séparer les différentes colonnes, la première ligne correspond bien à l'en-tête des colonnes.

Il est souvent nécessaire d'effacer la dernière ligne d'acquisition, incomplète.

Le nom du fichier est construit ainsi : ` day() + "-" + month() + "[" + Kp + "," + Ki + "," + Kd + "].csv" `

### Un exemple

*TODO*