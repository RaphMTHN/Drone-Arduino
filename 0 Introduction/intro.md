# Introduction


## Préambule

Mon étude portant sur la recherche d’une loi de commande optimale d’un drone quadrirotor en vol stationnaire a suivi les différents objectifs fixés en début de projet. Cependant, j’ai naturellement effectué des allers et retours entre la modélisation et l’implémentation matérielle d’une maquette. Ainsi, les erreurs sont plus rapidement traitées, évitant d’obtenir de trop gros écarts entre le modèle théorique et le système réel.


## Plan

Pour répondre à ma problématique, j’ai souhaité proposer une maquette de drone et sa modélisation fidèle, capable de suivre une consigne de position, soumis à des perturbations, autour de l’axe de roulis
. 
-	J’ai effectué une analyse des composants utiles au vol stationnaire et de leurs interactions. 

-	J’ai étudié chaque composant du système afin d’obtenir un modèle de connaissance complet du système, implémenté dans l’outil de simulation graphique Xcos.

-	J’ai réalisé l’implémentation de la maquette et ajusté le modèle théorique en conséquence.

-	J'ai proposé une correction du système à l’aide de la méthode de Ziegler Nichols.