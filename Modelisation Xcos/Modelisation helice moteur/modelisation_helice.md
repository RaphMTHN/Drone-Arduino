# Modélisation de l'hélice



## Présentation

On a calculé au cours de l'étape de modélisation l'expression du torseur d'action de l'air sur l'hélice afin de relier la poussée exercée par l'hélice à la vitesse de rotation du rotor.

Les expressions sont :

`LATEX`



## Expérience

### Objectif

On dispose des résultats expérimentaux fournis par le constructeur pour une hélice standard dont on connait les dimensions.

On souhaite donc vérifier la cohérence du modèle, en mesurant les coefficients de trainée et de poussée qui interviennent dans l'expression de la puissance mise en jeu et de la poussée exercée.

Pour cela, on identifie les coefficients Cp et Ct expérimentalement, puis on compare aux valeurs théoriques de ces coefficients obtenus pour un profil standard (Naca2412).


### Données

Données constructeur : TMotor air 2213

http://www.rctigermotor.com/html/2014/Combo_pack_0930/279.html

| b' (RPM) | R_i (g) | P_i (W) | 
|----------|---------|---------| 
| 4400     | 240     | 22,2    | 
| 5900     | 386     | 42,18   | 
| 6900     | 490     | 61,05   | 
| 7800     | 594     | 79,92   | 
| 8300     | 722     | 108,78  | 


- Diamètre de l'hélice D = 9.5" = 0.2375m
- Largeur de l'hélice e = 2cm = 0.02m
- Tension U = 11.1V

P_i est calculé à partir du courant alimentant l'induit du moteur et de la tension de fonctionnement.


---------------------------------------------

Profil NACA 2412 : 

http://airfoiltools.com/polar/details?polar=xf-naca2412-il-1000000

Extrait des données récupérées :

| Alpha  | Cp      | Ct      | 
|--------|---------|---------| 
| -17,5  | -1,1118 | 0,08608 | 
| -17,25 | -1,1738 | 0,07238 | 
| -17    | -1,2296 | 0,05928 | 



### Protocole


- On détermine les coefficients Cp et Ct à l'aide d'une régression linéaire (méthode des moindres carrés - outil d'analyse Excel)

- On détermine ces mêmes coefficients dans la base de données NACA pour un profil classique 2412, après calcul d'un ordre de grandeur du coefficient de Reynolds, au maximum du ratio Cp/Ct (meilleure poussée pour plus faible trainée)

- On compare leurs valeurs, et l'allure des courbes relatives à la poussée (résultante) R_i et la puissance consommée P_i.



### Analyse des résultats


En ce qui concerne la poussée, la valeur du Cp donnée par la régression linéaire est 0.67. Le coefficient de corrélation est égal à 0.998 pour un total de 5 mesures, ce qui est tout à fait acceptable et valide la dépendance en b' ^2.

Pour la puissance consommée, il est à noter que la formule théorique ne porte que sur la puissance mécanique résultant du couple exercé par l'air sur l'hélice et la vitesse de rotation de celle-ci. Après regression linéaire, on trouve un Ct égal à 0.13. Le coefficient de corrélation est de 0.997, ce qui est à nouveau suffisant pour conforter la dépendance en b'^3.

Le rapport Cp/Ct maximal est atteint pour un angle d'incidence de 4.5deg : on trouve alors un couple (Cp;Ct) = (0,7624 ; 0,00752).

Ainsi, l'écart entre le Cp idéal et le Cp calculé expérimentalement est vraiment faible : la modélisation de la poussée de l'hélice semble donc bien valide.

Cependant, il y a un écart énorme entre le Ct théorique et le Ct expérimental : 

- soit la modélisation de l'hélice par un profil NACA 2412 en incidence idéale est incorrect, et ainsi l'ordre de grandeur théorique n'a pas de sens ici ... mais la validité du Cp semble conforter ce choix de profil

- soit le modèle de P_i est incorrect, qu'il s'agisse d'une erreur de modélisation ou de calcul.



## Adaptation aux dimensions d'hélice de la maquette


Afin de pouvoir faire fonctionner la maquette sur une plage de vitesse de rotation importante, j'ai décidé d'utiliser des hélices de diamètre plus petit, afin d'avoir une poussée nettement inférieure. En effet, les moteurs que j'ai choisi sont adaptés à des drones de 1200 à 1500g, dans le but de les réutiliser plus tard pour la réalisation d'un drone complet.

Les hélices installées sur les moteurs sont des [GemFan 6045](https://hobbyking.com/fr_fr/gemfan-propeller-6x4-5-black-cw-ccw-2pcs.html), de diamètre 6" = 0.15m et d'épaisseur e = 1.5cm = 0.015m

En considérant que les profils d'hélices sont similaires, les coefficients de trainée et de poussée sont identiques.

On peut alors calculer le coefficient Kh des hélices de la maquette : 

le coefficient directeur de la régression linéaire de la partie précédente est justement ce coefficient Kh' pour des hélices D'0.2375m et e'=0.02m.

Kh' = 9,26E-06 N.(rad.s-1)^2

Kh = Kh' * (D^3 * e)/(D'^3 * e')

On trouve, et on retiendra donc pour la modélisation :

** Kh = 1.75E-06 N.(rad.s-1)^-2 **



## Linéarisation de la loi de poussée de l'hélice


Le cadre d'étude des systèmes asservis en Sciences Industrielles de l'Ingénieur et en Pysique étant celui des systèmes linéaires, invariants et continus, il est nécessaire de linéariser l'expression de la poussée de l'hélice. 

On souhaite disposer d'une grande amplitude pour la vitesse de rotation du moteur. Les moteurs ont une commande à l'équilibre qui sera égale à 40%. Cela correspond à une vitesse de rotation égale à 382 rad.s^-1.

On linéarise autour de ce point de fonctionnement :

R_i = Kh_lin * b'_i + R_i_0

Où : 

- Kh_lin = 2 * Kh * b'_0

- R_i_o = Kh * b'_0 ^2


On retiendra donc le couple :

(Kh_lin ; R_i_0) = (1,34E-3 N.(rad.s^-1)^-1 ; 0.255 N)


## Conclusion

Les données fournies par le constructeur ont permis de valider l'expression de la poussée exercée par une hélice en fonction de sa vitesse de rotation. Cependant, l'expression de la puissance est remise en question par la différence conséquente d'ordre de grandeur entre le Ct expérimental et théorique d'un profil NACA.

Dans tous les cas, la dépendance en b'^2 et b'^3 est vérifiée. 

On pourra se contenter d'une relation empirique pour la puissance, ce qui n'aura pas d'influence majeure sur la conception et la réalisation de ma maquette.