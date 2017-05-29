# Gyroscope

## Fonctionnement

Le gyroscope est un composant indispensable au drone afin d'évaluer ses composantes dynamiques et ainsi calculer la commande adéquate à envoyer aux moteurs pour rendre le système stable. Dans le cas de la commande de l'angle de roulis, c'est le gyroscope qui permet l'asservissement du système selon cet angle : les données mesurées interviennent directement dans l'élaboration de la commande, puisqu'elles représentent la chaine de retour de la boucle d'asservissement.

Un gyroscope est constitué de ... *TODO*


## A propos du MPU5060

Le MPU5060 est une centrale inertielle 6-axes composée de 3 accéléromètres et 3 gyro(scopes – mètres ?). Ce microprocesseur est équipé d’un port série (I2C) afin de communiquer avec d’autres composants, et  d’un processeur *(Digital Motion Processor DMP)* capable d’effectuer des calculs complexes afin de traiter les données brutes des capteurs (filtrage par fusion de données, calcul de quaternion). 

Invensense fournit dans la documentation technique du capteur une carte des registres (register map) à accéder afin de modifier un certain nombre de réglages, d’effectuer certaines actions ou de récupérer certaines données non traitées. Mais une politique de découragement de la compagnie a poussé les développeurs d'Invensense à volontairement omettre de préciser comment modifier les réglages du DMP et du buffer FIFO accumulant les données calculées, et comment récupérer les quaternions utiles au repérage du capteur dans l’espace (tout ceci étant uniquement disponible dans l’IDE MotionDriver (dans le cas du mpu5060) – accessible aux développeurs reconnus par Invensense et non prévue pour Arduino).

Aucune interface de développement (IDE) n’a été conçue par Invensense pour une utilisation facile du MPU5060 avec une carte Arduino, néanmoins plusieurs développeurs sont parvenus ensemble à produire une bibliothèque arduino libre capable de paramétrer le capteur et de récupérer non seulement les données brutes, mais aussi les données calculées par le DMP.


## A propos d’Invensense et du FireFly DK

Face au besoin croissant de capteur dans un monde où le marché de l’IoT (Internet of Objects) est en train d’exploser, Invensense a développé par la suite le FireFly Development Kit, constitué d’une carte arduino Zero (connectivité usb / internet etc.), d’un carte mère shield programmable (SDK) contenant les processeurs (DMP), d’une carte fille contenant les capteurs de mouvement et d’un IDE, SensorStudio, offrant une approche haut niveau de la programmation de systèmes fonctionnant en temps réel.

> SensorStudio is our new development tool used to ease the development and programming of the FireFly platform. 

>The simple GUI uses sensors blocks that can be arranged in elegant flow charts for developers to run and validate their program, this makes for rapid prototyping application development.

>InvenSense supplies our standard sensor block functions derived from our sensor framework software. Developers can also create their own custom sensor functions and verify their functionality in SensorStudio.

>SensorStudio lets you visually assemble elements in a logical way and access all the necessary visualizations that fully support the sensor fusion design cycle needs. SensorStudio lets you program and add your own sensors to the Sensor Framework via a documented set of APIs to control I2C, GPIO, tasks and timers, in a way the complexity of real-time OS is not visible.

>SensorStudio brings a unique level of simplification and integration the IoT market needs to build Smart applications faster.”

[lien site Ivensense](HTTPS://WWW.INVENSENSE.COM/PRODUCTS/MOTION-TRACKING/6-AXIS/ICM-30630-2/)


## Choix du mpu5060

le mpu5060 a été choisi pour des raisons de disponibilité (magasin d’électronique) et de prix très attractif (~5€) ; le travail de la communauté Arduino étant très important pour ce composant, et n’ayant pas eu connaissance de la solution technique FireFly DK, il n'a pas été question d'envisager un changement de centrale inertielle.