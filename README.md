# Picsorter

Picsorter soll den Prozess der Bildbearbeitung (Sortierung, Einordnung und Benennung) erleichtern.
Dazu schreibt Picsorter das Datum der letzten Modifikation an den Anfang der Bilddatei, um eine
Einsortierung in bestehende Ordnerstrukturen zu erleichtern.

Aktuell ist die Benennung im Format YYYY-MM-DD umgesetzt ohne Übernahme der ursprünglichen Benennung.
Es ist möglich diese Umbenennung rekursiv auf mehrere gleichzeitig Ordner anzuwenden.
Nach Umbenennung werden sie zeitlich in Ordner verteilt.

## Abhängigkeiten

Zur Kompilierung werden `make` und `gcc` benötigt. Andere Abhängigkeiten sollten nicht bestehen.

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
- Sicherung in anderen Dateisystemen (via samba)



