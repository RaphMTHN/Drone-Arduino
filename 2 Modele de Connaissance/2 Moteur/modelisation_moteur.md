# Modélisation des moteurs



## Description

Une fois la relation reliant poussée et vitesse de rotation du moteur identifiée, il est nécessaire de pouvoir modéliser le comportement du moteur pour relier une grandeur commandable par le contrôleur embarqué et la vitesse de rotation.

On ne dispose d'aucun dispositif de mesure de vitesse de rotation. Les moteurs utilisés sont prêt à l'emploi : aucun capteur de vitesse de rotation n'est installable facilement entre le rotor et le stator, et les données de retour du variateur de vitesse ne sont pas accessible à l'utilisateur. Ces composants sont généralement installés directement sur un contrôleur de drone pré-programmé, et ne sont pas conçus pour être utilisés au cours du processus de développement d'un drone.

On fera donc l'hypothèse que les moteurs brushless sont asservis en vitesse de rotation, et donc que pour une même tension d'alimentation U=11.1V, la vitesse ne dépend que de la consigne envoyée à l'ESC via un signal de commande 0-5V de type pulse modulé en largeur d'impulsion (PWM).



## Identification des paramètres

Les données expérimentales du constructeur sont suffisantes pour déterminer la valeur du gain moteur Km. 

Une regression linéaire permet d'identifier Km, gain reliant la commande en % à la vitesse de rotation du moteur en rad.s^-1.

Le tracé du graphique correspondant indique que le régime linéaire semble atteindre progressivement une saturation pour une commande égale à 100%. On ne considère pour la régression que les valeurs de consigne inférieures à 85%.

On trouve alors Km = 954 rad.s^-1 , avec un coefficient de corrélation égal à 0.9999. On peut donc valider la modélisation linéaire des moteurs, du moins pour une plage de fonctionnement ne dépassant pas les 85%.

Aucune réponse temporelle n'est disponible. On ne peut donc pas identifier directement la constante de temps tau. On prendra donc un ordre de grandeur, caractéristique des moteurs à courant continu, égal à 0.1s.



## Conclusion

- Pour une modélisation plus fine du moteur, il aurait fallu disposer de capteurs de vitesse de rotation. Une fois la réponse temporelle mesurée, la détermination des coefficients Km et tau d'un premier ordre est accessible sans difficulté.

- Des mesures envisageables sans installer de capteur sur le rotor sont une mesure de vitesse de rotation à l'aide d'un capteur optique (tachymètre laser et gommette réflechissante : ex RPM Laser DT2234C), et une identification de la constante de temps tau par analyse de la réponse sonore (l'amplitude de l'harmonique de fréquence sonore du régime permanent pouvant être mesurée au cours du temps par analyse spectrale de l'enregistrement micro par exemple).

- ** On prendra donc comme valeurs des paramètres : (Km,tau) = (954,0.1). **