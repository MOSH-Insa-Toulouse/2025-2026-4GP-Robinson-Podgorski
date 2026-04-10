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
  - [3. Conception de la PCB avec Kicad](#3-conception-de-la-pcb-avec-kicad)
  - [4. Réalisation de la Shield](#4-réalisation-de-la-shield)
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

Dans un monde où la complexification de conception, de fabrication, d'utilisation et de réparation des dispositifs electronique et appareils entraînent certaines dérives environnementales et de surconsommation, le développement de la low-tech est enjeu majeur de ces prochaines années.  

Dans ce contexte et dans le cadre de notre formation au Génie Physique de l'INSA Toulouse, nous avons développé au cours de ce projet un capteur de flexion simple de conception, de réalisation et de réparation, basé sur du graphite de crayon à papier. Nous l'avons intégré au sein d'un circuit électronique interactif couplé à un microcontrôleur Arduino, pilotable grâce à un code arduino et une application Android.

Le but final de ce capteur est de pouvoir le comparer avec un capteur de flexion industriel, et pouvoir répondre à la question :
*Ce capteur peut-il remplacer un capteur flex industriel ?*

Ce projet a été effectué en différentes étapes, qui sont documentées dans ce README

# Livrables

Ce projet contient 4 différents livrables :  
* Une shield PCB pour le circuit électronique de nos composants
* Un code Arduino qui gère les différents composants ainsi que le banc de test
* Une application android APK qui gère l'interface avec la shield Arduino
* Une dtasheet du capteur de flexion


# Réalisation du projet

## 1. Conception du capteur graphite

### 1.1 Matériel utilisé pour ce projet
Pour réaliser ce projet, nous avons utilisé les composants électroniques suivants :
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
* un filtre passif avec R1 et C1, pour éviter que les bruits en courant de haute fréquence ne causent de la distorsion dans l'étage d'entrée
* un filtre actif avec R3 et C4, pour un maximum d'efficacité sur l'amplificateur opérationnel
* un filtre passif avec C2 et R6, pour retirer le bruit introduit en cours de traitement

De plus, la résistance en R5 protège l'amplificateur opérationnel contre les décharges électrostatiques, et forme avec C1 un filtre pour les bruits en tension.

Pour ce projet, nous avons aussi décidé de faire varier la résistance R2 avec un potentiomètre digital cablé en résistance variable, pour pouvoir adapter le gain en fonction du courant entrant. En effet, le gain de ce montage, exprimé comme ci-dessous, est inversement proportionnel à R2 :

$V_{ADC}=(1+\frac{R_{3}}{R_{2}})\frac{R_{1}}{R_{1}+R_{c}+R_{5}}V_{cc}$


### 2.2 Simulations

## 3. Conception de la PCB avec Kicad

## 4. Réalisation de la Shield

## 5. Ecriture du code Arduino

## 6. Développement de l'application android couplée au code arduino avec MIT App Inventor

## 7. Banc de test

# Conclusion

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
