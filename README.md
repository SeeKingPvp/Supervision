# Supervision

Application Qt (C++) de supervision d'une voiture robotisée. Elle se connecte en TCP à la voiture et affiche en temps réel :

- le **Lidar 360°** (une distance par degré, tracée autour de la voiture) ;
- la **voiture** : vitesse, angle de braquage (roues avant orientées) et état des 4 capteurs **TFmini** avant ;
- la **batterie** : tension et niveau de charge sur 5 barres ;
- l'état de chaque connexion (connecté en vert, déconnecté en rouge).

Les boutons **ON** / **OFF** envoient les commandes `start` / `stop` à la voiture.

## Prérequis

- Qt 5 ou Qt 6 avec les modules `widgets` et `network`
- Un compilateur C++17
- Qt Creator (recommandé) ou `qmake` + `make`

## Compilation

Avec Qt Creator : ouvrir `Supervision.pro`, choisir un kit, puis **Exécuter**.

En ligne de commande :

```bash
mkdir build && cd build
qmake ../Supervision.pro
make
./Supervision
```

## Connexions TCP

L'adresse de la voiture et les ports sont définis dans `mainwindow.cpp` (constructeur de `MainWindow`) :

| Service   | Adresse        | Port | Sens               |
|-----------|----------------|------|--------------------|
| Lidar 360 | `192.168.1.12` | 9991 | voiture → IHM      |
| Voiture   | `192.168.1.12` | 9992 | voiture → IHM      |
| Commandes | `192.168.1.12` | 9993 | IHM → voiture      |
| Batterie  | `192.168.1.12` | 9994 | voiture → IHM      |

Chaque client se reconnecte automatiquement toutes les 2 s tant que le serveur n'est pas joignable. L'interface reste utilisable pendant ce temps.

## Format des messages

Chaque message est un `QString` sérialisé avec `QDataStream` (version `Qt_5_0`) : la taille de la chaîne est envoyée devant le texte. Le serveur doit utiliser le même encodage.

| Flux      | Contenu                                   | Exemple                       |
|-----------|-------------------------------------------|-------------------------------|
| Lidar     | 360 distances (mm) séparées par `;`, de 0° à 359° | `850;860;870;...`     |
| Voiture   | `vitesse;angle` ou `vitesse;angle;G;MG;MD;D` | `0.50;-0.20;1;1;0;1`       |
| Batterie  | tension en volts                          | `7.85`                        |
| Commandes | `start` ou `stop`                         | `start`                       |

- **Voiture** : `vitesse` et `angle` sont bornés entre -1 et 1. Les 4 valeurs suivantes (optionnelles) donnent l'état des TFmini gauche, milieu gauche, milieu droite et droite (`1` = connecté, `0` = déconnecté).
- **Lidar** : les distances sont plafonnées à 3000 (3 m) à l'affichage.
- **Batterie** : 6,0 V correspond à 0 % et 8,4 V à 100 %. Les barres passent en orange sous 30 % et en rouge sous 15 %.

## Structure du code

| Fichier                     | Rôle                                                        |
|-----------------------------|-------------------------------------------------------------|
| `main.cpp`                  | Point d'entrée                                              |
| `mainwindow.*`              | Fenêtre principale, boutons ON/OFF, labels d'état           |
| `tcpclient.*`               | Client TCP non bloquant avec reconnexion automatique        |
| `widgetlidar.*`             | Affichage du Lidar 360°                                     |
| `widgetvoiture.*`           | Affichage de la voiture et des capteurs TFmini              |
| `widgetbatterie.*`          | Affichage de la batterie                                    |
| `widgettfmini.*`            | Classe vide, pas encore utilisée                            |
