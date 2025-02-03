# Picsorter

Picsorter soll den Prozess der Bildbearbeitung (Sortierung, Einordnung und Benennung) erleichtern.
Dazu schreibt Picsorter das Datum der letzten Modifikation an den Anfang der Bilddatei, um eine
Einsortierung in bestehende Ordnerstrukturen zu erleichtern.

Die Benennung wird im Format YYYY-MM-DD umgesetzt mit Übernahme der ursprünglichen Benennung und
Entfernen anderer Datumsangaben.
Es ist möglich diese Umbenennung rekursiv auf mehrere gleichzeitig Ordner anzuwenden.
Nach Umbenennung werden sie chronologisch in Ordner verteilt.

## Abhängigkeiten

Zur Kompilierung werden `libsmbclient-dev`,`make` und `gcc` benötigt. Andere Abhängigkeiten sollten nicht bestehen.

```
sudo apt-get install libsmbclient-dev make gcc
```

## Installation

Picsorter kann folgendermaßen installiert werden:

```
git clone git@github.com:Chris1189/Picsorter.git picsorter  
cd picsorter
make
```

## Geplante Implementierungen

- Verteilen nach anderen Schemata (Benennung der Zielordner, Verschieben von Unterordnern)
- Gesichtserkennung und Sortierung nach Personen (Prüfung auf Realisierbarkeit)
- Sicherung in anderen Dateisystemen (via samba) (**in progress**)



