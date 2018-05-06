# MQTT Broker

## Clone repository
To clone this repository execute following command:
```
git clone https://github.com/bbijelic/mqttbroker.git
```

## Dependencies

At the moment project has some dependencies which have to be resolved before building the project.

* libconfig
* libssl

Install dependencies
```
$ sudo apt-get install libconfig++-dev libssl-dev
```
`TODO` Integrated dependecy resolving into CMake

## Build project

To build project execute following commands:

```
$ cmake .
$ make
```

Build output will be placed into `build` directory.

## Start MQTT broker

```
./mqttbroker
```
