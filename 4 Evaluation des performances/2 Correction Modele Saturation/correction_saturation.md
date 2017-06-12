# Amélioration du modèle

Avant d'implémenter le correcteur dans le système réel, on essaie de modéliser plus finement certaines propriétés du système.

En effet, certaines caractéristiques du système réel, comme la saturation de la commande moteur et le caractère discret du système, dont la période de boucle est contrainte par la fréquence d'échantillonnage du capteur, n'ont pas été prises en compte. 

On ne sait pas gérer sur Xcos le caractère discret du système. En revanche, le module CPGE d'XCos permet d'ajouter facilement un saturateur.



## Ajout de la saturation

On a volontairement ajouté lors de la programmation de la loi de commande du système un saturateur afin de limiter la commande moteur à un intervalle centré en la consigne constante moteur C0 (servant également de point de linéarisation du modèle de l'hélice).

Pour C0 = 40%, l'intervalle est alors [10% , 70%]. Autrement dit la consigne sortant du correcteur est limitée à +-30%.

On suit alors le protocole décrit dans lors de l'étape de correction du modèle théorique.



## Résultats

Pour toutes les valeurs de Kd, la saturation améliore la stabilité du système en limitant de façon non négligeable l'amplitude des oscillations. 

La rapidité est moins bonne pour de faibles valeurs de Kd (<0.2). Au delà, le temps de réponse du système stagne à la même valeur que celle de la modélisation sans saturation.

Pour un Kd supérieur à 1, la réponse présente des non linéarités importantes, le système devient même instable pour un Kd égal à 2.


Le meilleur compromis rapidité / stabilité semble être à nouveau atteint pour un Kd égal à 0.5.