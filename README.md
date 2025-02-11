# Picsorter

Picsorter is designed to simplify the process of image editing (sorting, categorizing and naming).
For this purpose, Picsorter writes the date of the last modification at the beginning of the image
file to facilitate sorting into existing folder structures.

The naming is implemented in the format YYYY-MM-DD with the original naming and removal of other dates.
It is possible to apply this renaming recursively to several folders at the same time.
After renaming, they are distributed chronologically in folders.

## Dependencies

For compilation `libsmbclient-dev`,`make` and `gcc` are needed.

```
sudo apt-get install libsmbclient-dev make gcc
```

## Installation

Picsorter can be installed the following way:
```
git clone git@github.com:Chris1189/Picsorter.git picsorter  
cd picsorter
make
```
