# Programmation de l'objet Gyro


## Pourquoi un tel objet

Face au grand nombre de méthodes que possède l'objet mpu5060 disponible dans la bibliothèque éponyme, j'ai décidé d'encapsuler le code utilisé dans la démonstration de la bibliothèque.

Cette approche respecte le grand principe de la programmation orienté objet, particulièrement adaptée à un projet faisant intervenir des composants matériels. L'objectif est de virtualiser l'objet réel, ici le gyroscope, et de fournir à cet objet virtuel une interface simple pour l'utilisateur, masquant la complexité plus importante du code complet *(demande de lecture des registres, réception, déclenchement d'un évènement....)*. Les méthodes et attributs de l'objet sont simples à comprendre, et dans un premier temps, la lecture de l'en-tête de la classe gyro suffit à comprendre l'essentiel de son fonctionnement.

Les avantages sont donc une bien meilleure lisibilité du code source principal implémenté dans la carte arduino embarqué, et une limitation des fonctionnalités à celles que j'utilise effectivement pour ma maquette.


## Ses méthodes :

``` cpp
  Gyro();                       // constructeur de classe
  
  void initialize(void(*dmpDataReady)()); // initialise le mpu5060 (param: interrupt récupérant le
  // signal électrique du pin INTERRUPT_PIN - similaire à la capture d'un évenement dans une machine
  // d'états)
  int getInitStatus() const;    // retourne le statut d'initialisation

  float getRoll() const;        // retourne l'angle de roulis
  void operateGyro();           // exécute une routine nécessaire à la réception éventuelle de données
  bool hasNewData() const;      // retourne true si de nouvelles données sont disponibles
  void setNoDataReady();        // déclare que l'utilisateur a bien lu les données disponibles

  void setInterrupt();          // set mpuInterrupt to true (?)

  void setInitialTime();        // réinitialise l'instant initial d'acquisition des données
  float getMeasureTime() const; // retourne l'instant d'acquisition des données
```


## Les bibliothèques utilisés par Gyro

### MPU5060

MPU5060 contient des méthodes qui effectuent pour la majorité d'entre-elles uniquement des lecture de registres contenant les variables d'état du gyroscope, et d'écriture des registres de configuration.

Son contenu peut donc paraître colossal, mais il s'agit surtout d'une implémentation d'une interface "human-readable" propre au paradygme de la programmation orienté objet, en définissant des variables et méthodes avec des noms beaucoup plus explicites que les adresses hexadécimales des registres, définies dans le datasheet du MPU5060 et redéclarées au début du fichier d'en-têtes.

Certaines méthodes sont précisées dans le header, mais définies dans un autre fichier source : c'est le cas du code de MPU6050_6Axis_MotionApps20.


### MPU6050_6Axis_MotionApps20

Les fichiers en-têtes et source ainsi intitulées contiennent la définition des méthodes propres au DMP du mpu5060. Ses fonctions sont les plus importantes, puisque seul le DMP fournit des données filtrées et cohérentes vis-à-vis des unités attendues.

La définition de ces fonctions résultent du travail de reverse-engineering réalisé par les programmeurs de l'objet mpu5060, rendant ainsi libre le code permettant d'accéder au DMP correctement (ce code n'étant pas fourni dans la documentation du mpu5060, mais uniquement accessible via l'IDE propriétaire).


### helper_3dmath

Il s'agit d'un petit fichier permettant d'effectuer des calculs sur les quaternions renvoyés par le DMP : notamment la conversion quaternion -> angles de Cardan.

*TODO : description des quaternions*

L'objectif de la maquette étant l'asservissement en angle de roulis, il est évident que l'on préfèrera manipuler les angles de Cardan plutôt que les quaternions directement. L'avantage que comporte les quaternions n'a que peu d'utilité dans le cas d'un drone en vol quasi-stationnaire.


### I2Cdev

La bibliothèque I2Cdev est utilisé par l'objet MPU5060 pour réaliser toute les communications série entre la carte Arduino et le capteur, via un bus I2C (série).