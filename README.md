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
