# Implémentation du système



## Virtualiser les composants embarqués

La maquette de drone est constitué de différents composants matériels qui interagissent directement avec la carte de commande. Ils sont au nombre de 2 :

- les variateurs de vitesses (ESC) des moteurs brushless

- le capteur 6-axes

Cette approche respecte le grand principe de la programmation orienté objet.

 L'objectif est de virtualiser l'objet réel, et de fournir à cet objet virtuel une interface simple pour l'utilisateur, masquant la complexité plus importante du code complet *(lecture ou ecriture des registres du composant, écoute et déclenchement d'un évènement...)*. Les méthodes et attributs de l'objet virtuel représentent ainsi les actions que peut exécuter le composant, ainsi que ses variables d'état.

Les avantages sont donc une bien meilleure lisibilité du code source principal implémenté dans la carte de commande embarquée, et une limitation des fonctionnalités à celles que j'utilise effectivement pour ma maquette.



### L'objet Esc

Cet objet a été entièrement codé pour l'utilisation des variateurs de vitesse avec la carte de commande Arduino. Après lecture du document technique du processeur de la carte, capable de générer des signaux de commande compatible avec les Esc, on a produit le code source associé à l'objet virtuel, et validé à l'oscilloscope la nature du signal généré.


### L'objet Gyro

Cet objet exploite dans son intégralité le code source de l'objet mpu5060 développé et publié librement par des développeurs indépendants de la communauté Arduino. Il encapsule les différentes étapes nécessaires au fonctionnement du capteur à l'intérieur de méthodes plus simples et compactes.



## Programmer la boucle d'asservissement

La particularité de la programmation de systèmes réels est que la gestion du cycle d'opérations est totalement différente d'un programme classique étudié en informatique en classe préparatoire. Il ne s'agit pas d'un simple enchainement d'opérations : des évenements se produisent et font varier le comportement du système.

On adopte le patron de conception de la programmation d'une machine d'état décrit en sciences de l'ingénieur. La maquette peut être sous différents états : veille, calibration, marche, arrêt d'urgence. En fonction de l'état dans lequel elle se situe, les instructions seront différentes. C'est alors l'utilisateur qui déclenche le passage d'un état à un autre.

On implémente de plus dans l'état de marche un correcteur PID numérique en discrétisant les équations d'un tel correcteur. La consigne est alors contrôlée par un saturateur afin de rester dans une plage de fonctionnement finie.

La génération de la commande par le correcteur est alors effectuée à chaque réception de nouvelles données provenant du capteur gyroscopique : la fréquence d'échantillonnage du capteur 6-axes imposent donc une période de boucle non infiniment petite.



## Créer l'interface graphique

Pour pouvoir piloter la maquette et récupérer les données de commande, il est nécessaire d'avoir une interface homme-machine reliant la carte arduino et un ordinateur par exemple.

On a choisi pour réaliser l'interface graphique d'utiliser la bibliothèque de développement Processing, utilisant le langage de programmation Java. Processing est particulièrement adapté à la création d'interfaces graphiques : on a ainsi utilisé le module controlp5 et exploité les exemples de la documentation afin de développer rapidement l'interface souhaitée.

Ses fonctionnalités sont :

- choisir l'état du système (veille, marche, arrêt d'urgence, calibration)

- communiquer les paramètres de réglage du système (consigne, PID, constantes relatives à la commande moteur)

- enregistrer les données d'acquisition sous format csv (format informatique de représentation de données tabulaires)



## Relier l'interface graphique et la carte de commande

Pour permettre aux différents composants d’un système automatisé de communiquer entre-eux, ceux-ci doivent s’accorder sur un même protocole de communication. A l’instar d’une conversation entre deux individus, la syntaxe des messages doit être connue par les deux interlocuteurs. Le protocole série en est un, très répandu en programmation embarquée.

Matériellement, on utilise les ports USB (universal serial bus) de l'ordinateur et de la carte Arduino.

Processing et Arduino proposent tous deux des modules à importer qui permettent d'instancier un objet virtualisant la connexion série.

On configure alors de part-et-d'autres les différentes trames que chaque interlocuteur va envoyer ou recevoir. On convertit chaque donnée sous forme de byte, seul format compatible avec un tel protocole.

Les événements attachés à l'envoi d'une nouvelle trame série sont la réception de nouvelles données du capteur, et l'action de l'utilisateur sur l'interface graphique.