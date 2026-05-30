##### 2025-2026 Podgorski Romain et Robinson Yanis
# **Projet de Capteur Graphite**

## Table des matières
- [Projet de Gapteur Graphite](#projet-de-capteur-graphite)
- [Contexte](#contexte)
- [Réalisation du projet](#réalisation-du-projet)
  - [1. Conception du capteur graphite](#1-conception-du-capteur-graphite)
    - [1.1 Matériel utilisé pour ce projet](#11-matériel-utilisé-pour-ce-projet)
    - [1.2 Principe de fonctionnement du capteur graphite](#12-principe-de-fonctionnement-du-capteur-graphite)
  - [2. Simulation electronique du circuit amplificateur avec LTSpice](#2-simulation-electronique-du-circuit-amplificateur-avec-ltspice)
    - [2.1 Principe de fonctionnement](#21-principe-de-fonctionnement)
    - [2.2 Simulations](#22-simulations)
  - [3. Conception de la PCB avec KiCad](#3-conception-de-la-pcb-avec-kicad)
    - [3.1 Réalisation des empreintes et de la schématique](#31-réalisation-des-empreintes-et-de-la-schématique)
    - [3.2 Placement et routage](#32-placement-et-routage)
    - [3.3 Visualisation 3D et vérifications](#33-visualisation-3d-et-vérifications)
  - [4. Réalisation du shield](#4-réalisation-du-shield)
    - [4.1 Impression de la PCB](#41-impression-de-la-pcb)
    - [4.2 Montage et soudure](#42-montage-et-soudure)
  - [5. Ecriture du code Arduino](#5-ecriture-du-code-arduino)
  - [6. Développement de l'application android couplée au code arduino avec MIT App Inventor](#6-développement-de-lapplication-android-couplée-au-code-arduino-avec-mit-app-inventor)
  - [7. Banc de test](#7-banc-de-test)
- [Conclusion](#conclusion)
- [Références](#références)
- [Contacts](#contacts)
  - [Étudiants](#étudiants)
  - [Enseignants](#enseignants)


---

# Contexte

Dans un monde où la complexification de conception, de fabrication, d'utilisation et de réparation des dispositifs electroniques et appareils entraînent certaines dérives environnementales et de surconsommation, le développement de la low-tech est enjeu majeur de ces prochaines années.  

Dans ce contexte et dans le cadre de notre formation au Génie Physique de l'INSA Toulouse, nous avons développé au cours de ce projet un capteur de flexion simple de conception, de réalisation et de réparation, basé sur du graphite de crayon à papier. Nous l'avons intégré au sein d'un circuit électronique interactif couplé à un microcontrôleur Arduino, pilotable grâce à un code arduino et une application Android.

Le but final de ce capteur est de pouvoir le comparer avec un capteur de flexion industriel, et pouvoir répondre à la question :
*Ce capteur peut-il remplacer un capteur flex industriel ?*

Ce projet a été effectué en différentes étapes documentées dans ce README.

# Livrables

Ce projet contient 4 différents livrables :  
* Un shield Arduino pour le circuit électronique de nos composants
* Un code Arduino qui gère les différents composants ainsi que le banc de test
* Une application android APK qui gère l'interface avec le Arduino
* Une dtasheet du capteur de flexion

---

# Réalisation du projet

## 1. Conception du capteur graphite

### 1.1 Matériel utilisé pour ce projet
Pour réaliser ce projet, nous avons utilisé les composants électroniques suivant :
* 1 capteur en graphite fabriqué avec la mine d'un crayon à papier
* 1 carte Arduino UNO Rev 3
* 1 module Bluetooth HC-05
* 1 écran OLED SSD 1306
* 1 encodeur rotatif Keyes KY-040
* 1 capteur de flexion commercial LLC 1070
* 1 potentiomètre numérique MCP41100
* 2 supports IC pour le potentiomètre et l'amplificateur

Pour la réalisation du circuit amplificateur transimpédance, nous avons utilisé :
* 1 amplificateur opérationnel LTC1050
* 2 résistance 1kΩ
* 1 résistance 10kΩ
* 2 résistances 100kΩ
* 2 condensateurs 100nF
* 1 condensateur 1µF


### 1.2 Principe de fonctionnement du capteur graphite

Les mines de crayon sont composées de fines particules de graphite liées entre elles par des liants argileux. Le capteur de flexion en graphite repose sur le dépôt de ces particules sur un papier. En effet, les traces de crayon peuvent être considérées comme des films minces conducteurs constitués d'un réseau granulaire désordonné de particules de graphite percolées sur le papier. Les mines de crayon plus dures contiennent une proportion plus élevée de liants argileux tandis que les mines plus tendres contiennent une proportion plus élevée de particules de graphite résultant à une résistance moins élevée.

Face à une déformation mécanique, le réseau de ces particules est affecté, et la conduction électrique, qui s'effectue par le biais des contacts entre les particules voisines(effet tunnel), est donc fortement modifiée, augmentant ou diminuant la résistance du capteur :
* Sous contraction, la distance entre les particules diminue, favorisant les contacts entre paritucles et diminuant la résistance.
* Sous extension, la distance entre les particules augmente, rendant les contacts entre particules compliqués et augmentant donc la résistance.

C'est ainsi que nous pouvons mesurer la variation de flexion grâce à la variation de résistance au sein du capteur.

![Image du capteur graphite](IMAGE CAPTEUR GRAPHITE)

---

## 2. Simulation electronique du circuit amplificateur avec LTSpice

### 2.1 Principe de fonctionnement

La première étape de ce projet consiste à simuler le circuit amplificateur grâce au logiciel LTSpice.

Le capteur en graphite possède une résistance variable de l'ordre du GΩ. Si l'on applique la tension de 5V fournie par notre carte Arduino, le courant généré sera très faible, de l'ordre de la dizaine de nA. IL est donc nécessaire d'utiliser un circuit amplificateur pour pouvoir traiter ce signal.

Pour cela, nous utilisons un montage amplificateur transimpédance, qui permet de convertir le courant faible reçu par le capteur en une tension suffisante pour le convertisseur analogique-numérique (ADC) d'une carte Arduino. Le schéma du montage transimpédance est comme suit :

![Schéma montage transimpédance](/Photos/Schema%20ampli%20transimpédance.png)

La résistance de shunt R1 permet la conversion de courant à tension, afin d'accepter un courant entrant venant du capteur. Ce montage est amplificateur à fort gain, non-inverseur et a l'avantage de pouvoir fonctionner avec seulement une alimentation positive. Cependant, la chute de tension sur R1 perturbe un peu la tension appliquée au capteur.

Des contraintes sont néanmoins présente pour le choix de l'amplificateur opérationnel. En effet, il doit accepter un faible courant d'entrée et une ultra-faible offset de tension afin de ne pas fausser les valeurs de tension transmises à l'ADC. L'amplificateur LTC1050 répond à ces critères.

Pour compléter ce montage, il est nécessaire de rajouter des filtres. En effet, le montage est très sensible, et vulnérable au bruit 50 Hz, aux horloges de circuits digitaux (microcontrôleur, afficheur, etc...) et aux transceivers RF (bluetooth, etc...). Une bande passante de 1 Hz est largement suffisante pour extraire toute l'information utile du capteur. Un filtrage anti-repliement est aussi indispensable en raison de l'échantillonage effectué par l'ADC, et pour cela il est donc nécessaire de faire fonctionner l'ADC à la plus grande fréquence sans perte de qualité, qui est de 15.4 kHz pour pour l'Arduino UNO. Le montage final est donc comme suivant :

![Montage amplificateur final](/Photos/Simulation%20du%20montage%20amplificateur.png)

Dans ce montage, plusieurs filtres sont en action :
* un filtre passif avec R1 et C1, avec $f_{c}= 16$ Hz, pour éviter que les bruits en courant de haute fréquence ne causent de la distorsion dans l'étage d'entrée
* un filtre actif avec R3 et C4, avec $f_{c}= 1,6$ Hz,pour un maximum d'efficacité sur l'amplificateur opérationnel
* un filtre passif avec C2 et R6, avec $f_{c}= 1,6$ Hz, pour retirer le bruit introduit en cours de traitement

De plus, la résistance en R5 protège l'amplificateur opérationnel contre les décharges électrostatiques, et forme avec C1 un filtre pour les bruits en tension.

Pour ce projet, nous avons aussi décidé de faire varier la résistance R2 avec un potentiomètre digital cablé en résistance variable, pour pouvoir adapter le gain en fonction du courant entrant. En effet, le gain de ce montage, exprimé comme ci-dessous, est inversement proportionnel à R2 :

$V_{ADC}=(1+\frac{R_{3}}{R_{2}})\frac{R_{1}}{R_{1}+R_{c}+R_{5}}V_{cc}$

Et nous pouvons donc remonter à la valeur de la résistance du capteur graphite :

$R_{c}=(1+\frac{R_{3}}{R_{2}})R_{1}\frac{V_{cc}}{V_{ADC}}-R_{1}-R_{5}$

### 2.2 Simulations

Afin de vérifier l'efficience de notre circuit amplificateur, nous l'avons simulé sur le logiciel LTSpice. Voici le résultat obtenu :

![Simularion en régime transitoire](/Photos/Simu_transitoire.png)

Le signal est bien amplifié de 500 mV à 1,00 V et peut donc être interprété par l'ARDUINO.

Nous avons également simulé un signal alternatif pour vérifier l'efficience des filtres présents sur notre circuit. Le résultat obtenu est le suivant :

![Simulation en régime alternatif](/Photos/Simu_ac.png)

On observe trois filtrages successifs du signal initial, correspondants au trois filtres de notre système. De plus, le signal est atténué d'environ 70dB à 50 Hz, comme souhaité. 

Ainsi, notre simulation est réussi.


---

## 3. Conception de la PCB avec KiCad

Une fois que nous avons validé la simulation sur LTSpice, nous sommes passés à la réalisation du shield Arduino : le circuit imprimé (PCB). Il comportera tous les composants cités dans la section [1.1 Matériel utilisé pour ce projet](#11-matériel-utilisé-pour-ce-projet), pour la réalisation du montage amplificateur ainsi que les différents composants additionnels (module bluetooth, écran OLED...).    Pour ce faire, nous utilisons la suite logicielle KiCad 9.0. Nous sommes passés par plusieurs étapes décrites dans la section suivante.

### 3.1 Réalisation des empreintes et de la schématique

Tout d'abord, nous avons créé les symboles de schématique non présents dans les librairies fournies par KiCad, nous les avons associé à des empreintes et un module 3D. Nous avons ensuite réalisé le schéma électrique comme suivant :

![Schématique KiCad](/Photos/Schematique%20KiCad.png)

Certaines contraintes sur le choix des pins nous étaient imposées :
- Potentiomètre digital : pin SDI => pin D11 de l'Arduino, et pin SCK => pin D13 de l'Arduino
- Ecran OLED : pin SDA => pin A4 de l'Arduino, et pin SCK => pin A5 de l'Arduino
- Encodeur rotatoire : pin CLK => pin D2 de l'Arduino

Pour le restes des pins, nous étions libres de choisir les pins digitales ou analogiques de notre choix.

### 3.2 Placement et routage

La prochaine étape a été de placer l'organisation de la disposition des composants sur la PCB. Cette étape est cruciale car il faut pouvoir placer les composants startégiquement afin que les "routes" soient ordonnées et ne se surpassent pas, et qu'on utilise le moins de via possible. Le routage des différents composants a ensuite été effectué, et un plan de masse a été intégré pour stabiliser les potentiels et réduire le nombre de "routes". Voici la représentation de notre PCB :

![PCB KiCad](/Photos/PCB%20KiCad.png)

### 3.3 Visualisation 3D et vérifications

Pour terminer, il a fallu vérifier la conformité du routage et des vias grâce à l'outil de contrôle des règles de conception, ainsi que visulaiser la PCB en 3D pour vérifier que les composants ne se chevauchent pas et améliorer l'ergonomie de la PCB (accès facile aux différents composants).


Une fois ces contrôles validées, la PCB est prête à être imprimée.

---

## 4. Réalisation du shield

### 4.1 Impression de la PCB

Une fois la PCB validée, nous avons pu passer à l'impression, par une méthode chimie (photolithographie). La fabrcation s'est faite en plusieurs étapes, avec l'aide de Cathy Crouzet (un grand merci à elle !) :
1. Vérification de la PCB avec KiCad (diamètre des trous, plan de masse...)
2. Impresssion du masque de gravure de la PCB
3. Insolation UV d'une plaquette d'epoxy recouverte d'une couche de cuivre et d'une couche de résine photosensible
4. Immersion de la plaquette dans un révélateur pour éliminer la résine non exposée (résine positive)
5. Immersion de la plaquette dans du perchlorure de fer pour graver les pistes 
6. Nettoyage de la plaquette avec de l'acétone pour éliminer la résine restante.

### 4.2 Montage et soudure

Une fois la PCB imprimée, nous avons pu percer les trous de notre plaquette et souder les différents composants. 
Lors des premiers tests du shield, nous nous sommes rendu compte de la présence de courts-circuits sur la plaquette, dûs à un problème lors de la fabrication. Nous avons donc gratté avec un cuteur pour supprimer ces connexions indésirables.
Un autre problème est survenu lors des tests. Nous nous sommes rendus compte que nou avions relié les pins TX et RX du modules bluetooth aux pins 0 et 1 de l'Arduino, réservées à la communication par port série. Nous avons donc coupé ces routes et rajouté des fils pour les relier aux pins 7 et 8 qui etaient libres (à nouveau un grand merci, à Jérémie Grisolia pour nous avoir aidé à régler ces problèmes)

Une fois ces problèmes résolus, le shield Arduino a parfaitement fonctionné.

---

## 5. Ecriture du code Arduino

Le code Arduino comprend plusieurs fonctions contrôlant les différents modules de notre PCB. Les différents modules sont au préalable calibrés sur les bons pins.

**Configuration de chaque module**

On associe des pins à chaque module en en-tête du code pour la communication.

**Le setup (setup())**

Cette fonction à l'exécution du code :
* ouvre le canal de transmission avec le module Bluetooth avec un Baud rate de 9600.
* met le potentiomètre digital en veille
* Déclare les canaux de la clock, des datas et du switch de l'encodeur rotatoire comme des canaux d'entrée et active les pull-ups de canaux de la clock et des datas pour garantir la fiabilité des valeurs
* active le protocole SPI pour lire rapidement les données et règle l'horloge
* déclenche l'interruption matérielle en cas de front montant pour lire la valeur de la position de l'encodeur rotatoire
* Initialise l'écran OLED

**Le réglage du potentiomètre (setPotWiper(int addr, int pos))**

Cette fonction règle la valeur du potentiomètre en fonction de la position et calcule la résistance associée et l'affiche de le moniteur Serial.

**La mesure de le résistance du capteur graphite (graphiteMeas())**

On mesure une valeur de tension numérique aux bornes du capteur graphite entre 0 et 1023. On la convertit en tension analogique entre 0 et 5V et on calcule la résistance à partir de cette tension.

**La mesure de la résistance du flex sensor (flexMeas())**

Le principe de calcul est le même que celui utilisé pour le capteur graphite mais appliqué dans ce cas au flex sensor. 

**L'appui sur le bouton (get_encodButton())**

Cette fonction détecte lorsque le bouton de l'encodeur rotatoire est pressé et filtre les faux-déclenchements (anti-rebonds)

**La rotation de l'encodeur rotatoire (get_encodPos())**

Cette fonction détecte le sens de rotation de l'encodeur rotatoire et met à jour sa position.

**La calibration du potentiomètre numérique (calibration())**

Cette fonction calibre la valeur du potentiomètre numérique pour déterminer une valeur de tension aux bornes du capteur en graphite soit de 2,5 V (la valeur médiane). La valeur du potentiomètre est également transférée à l'application via le module Bluetooth.

**La construction de l'écran OLED (set_OLED_screen(byte tailleDeCaractere) et OLED_InverseColor(bool Inverse))**

Ces fonctions initialisent l'écran OLED aux dimensions souhaitées et écrivent les textes en noir sur fond blanc, respectivement.

**L'utilisation de l'écran OLED (OLED_manage_menu())**

Cette fonction affiche le menu selon la position de l'encodeur (Calibration, Meas on ou Meas off) et exécute l'action lorsqu'on appuise sur le bouton. Les données sont également envoyées à l'application.

**La fonction de rafraichissement (loop())**

Cette fonction lit la position de l'encodeur pour connaître la position du curseur sur le menu OLED, le gère, fait des mesures de réssitances du capteur graphite et du flex sensor si le curseur est sur l'onglet "Meas on" et envoue ces données à l'application via la Bluetooth.

## 6. Développement de l'application android couplée au code arduino avec MIT App Inventor

L'application s'articule en 5 points différents :

* L'ouverture du canal entre l'Arduino et l'appli via la connexion Bluetooth
* La récupération des données brutes
* Le tri des données en fonction du chiffre reçu (1 pour le calibrage, 2 pour la réception et l'affichage des mesures de résistance, 3 pour l'arrêt de la transmission et de l'affichage des mesures de résistance)
* L'afffichage des données sur la face avant, dans des spins box pour le calibrage et les valeurs de résistance en direct du capteur graphite et du flex sensor et sur des graphes des valeurs de résistance du capteur graphite et du flex sensor en fonction du temps
* La fermeture du canal


## 7. Banc de test

Nous avons utilisé un banc de test pour caractériser notre capteur graphite. Ce banc de test est un cylindre qui possède différentes valeurs de rayon de 1 à 2,5 cm. Nous avons dès lors calculé la variation de résistance $\frac{\Delta R}{R_0}$ en fonction de la déformation $\epsilon = \frac{e}{D}$, pour une épaisseur de papier e = 0,02 cm.

On obtient des courbes de caractérisations pour une déformation en tension et en compression avec différents types de crayons à papier.

![Variation des résistances en fonction de la déformation en tension](/Photos/Variation_de_la_résistance_en_fonction_de_la_déformation_en_tension.png)

![Variation des résistances en fonction de la déformation en compression](/Photos/Variation_de_la_résistance_en_fonction_de_la_déformation_en_compression.png)

Dans le cas d'une déformation en tension, plus la déformation est grande, plus la variation relative est grande. L'inverse se produit en compression. Cela s'explique par le fait que les atomes s'éloignent en tension ce qui augmente la résistance, à l'inverse de la compression.

En outre, plus le crayon est gras, plus la variation relative est faible en tension (HB, 2B, 4B) ; les crayons gras contiennent plus d'atomes de carbone.

Les résultats obtenus dépendent fortement de la quantité de crayon déposé sur le papier. Ainsi, les résultats obtenus ne sont pas reproductibles.

---

# Conclusion

La création d'un capteur de la première étape à l'étape de fin a été d'un grand enrichissement.
Nous avons eu l'occasion de toucher à divers outils pour la création de notre capteur (LTSPice, KiCad, Arduino, MIT App Inventor). En somme, ce projet est un avant-goût du travail actuel d'un ingénieur.

En outre, notre capteur est fonctionnel et simple d'utilisation. Les valeurs obtenues sont cohérentes,  et nous avons pu remplir l'ensemble du cahier des charges.
---

# Références

# Contacts

## Étudiants
- **Romain Podgorski** : podgorski@insa-toulouse.fr  
- **Yanis Robinson** : robinson@insa-toulouse.fr  

## Enseignants
- **Jérémie Grisolia** : jeremie.grisolia@insa-toulouse.fr
- **Benjamin Mestre** : benjamin.mestre@scalian.com  
- **Arnauld Biganzoli** : arnauld.biganzoli@insa-toulouse.fr  
- **Cathy Crouzet** : crouzet@insa-toulouse.fr  
